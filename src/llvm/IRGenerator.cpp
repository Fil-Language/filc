/**
 * MIT License
 *
 * Copyright (c) 2024-Present Kevin Traini
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include "filc/llvm/IRGenerator.h"

#include "filc/grammar/assignation/Assignation.h"
#include "filc/grammar/calcul/Calcul.h"
#include "filc/grammar/identifier/Identifier.h"
#include "filc/grammar/literal/Literal.h"
#include "filc/grammar/pointer/Pointer.h"
#include "filc/grammar/program/Program.h"
#include "filc/grammar/variable/Variable.h"
#include "filc/llvm/CalculBuilder.h"

#include <filc/grammar/array/Array.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/Verifier.h>
#include <llvm/MC/MCTargetOptions.h>
#include <llvm/MC/TargetRegistry.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Target/TargetOptions.h>
#include <llvm/TargetParser/Host.h>

using namespace filc;

IRGenerator::IRGenerator(const std::string &filename, const Environment *environment) {
    _visitor_context = std::make_unique<VisitorContext>();
    _llvm_context    = std::make_unique<llvm::LLVMContext>();
    _module          = std::make_unique<llvm::Module>(llvm::StringRef(filename), *_llvm_context);
    _builder         = std::make_unique<llvm::IRBuilder<>>(*_llvm_context);
    environment->prepareLLVMTypes(_llvm_context.get());
}

auto IRGenerator::dump() const -> std::string {
    std::string ir_result;
    llvm::raw_string_ostream out(ir_result);
    out << *_module;
    out.flush();
    return ir_result;
}

auto IRGenerator::toTarget(const std::string &output_file, const std::string &target_triple) const -> int {
    const auto used_target_triple = target_triple.empty() ? llvm::sys::getDefaultTargetTriple() : target_triple;

    llvm::InitializeAllTargetInfos();
    llvm::InitializeAllTargets();
    llvm::InitializeAllTargetMCs();
    // FIXME: llvm::InitializeAllAsmParsers();
    llvm::InitializeAllAsmPrinters();

    std::string error;
    const auto target = llvm::TargetRegistry::lookupTarget(used_target_triple, error);
    if (! target) {
        std::cerr << error;
        return 1;
    }
    const llvm::TargetOptions options;
    const auto target_machine = target->createTargetMachine(
        used_target_triple, "", "", options, llvm::Reloc::PIC_, std::nullopt, llvm::CodeGenOptLevel::None
    );

    _module->setDataLayout(target_machine->createDataLayout());
    _module->setTargetTriple(used_target_triple);

    std::error_code ec;
    llvm::raw_fd_stream out(output_file, ec);
    if (ec) {
        std::cerr << "Could not open file: " << ec.message();
        return 1;
    }

    auto pass_manager = llvm::legacy::PassManager();
    if (target_machine->addPassesToEmitFile(pass_manager, out, nullptr, llvm::CodeGenFileType::ObjectFile)) {
        std::cerr << "Target machine can't emit an object file";
        return 1;
    }

    pass_manager.run(*_module);
    out.flush();

    return 0;
}

auto IRGenerator::visitProgram(Program *program) -> llvm::Value * {
    const auto function_type = llvm::FunctionType::get(llvm::Type::getInt32Ty(*_llvm_context), {}, false);
    const auto function = llvm::Function::Create(function_type, llvm::Function::ExternalLinkage, "main", _module.get());
    const auto basic_block = llvm::BasicBlock::Create(*_llvm_context, "entry", function);
    _builder->SetInsertPoint(basic_block);

    const auto &expressions = program->getExpressions();
    if (expressions.empty()) {
        const auto return_value = llvm::ConstantInt::get(*_llvm_context, llvm::APInt(32, 0, true));
        _builder->CreateRet(return_value);
    } else {
        for (auto it = expressions.begin(); it != expressions.end(); ++it) {
            if (it + 1 != expressions.end()) {
                (*it)->acceptIRVisitor(this);
            } else {
                const auto return_value = (*it)->acceptIRVisitor(this);
                _builder->CreateRet(return_value);
            }
        }
    }

    llvm::verifyFunction(*function);

    return nullptr;
}

auto IRGenerator::visitBooleanLiteral(BooleanLiteral *literal) -> llvm::Value * {
    return llvm::ConstantInt::get(*_llvm_context, llvm::APInt(1, literal->getValue(), false));
}

auto IRGenerator::visitIntegerLiteral(IntegerLiteral *literal) -> llvm::Value * {
    const auto type_name = literal->getType()->getName();
    const auto is_signed = type_name[0] == 'i';
    const auto size      = stoi(type_name.substr(1));
    return llvm::ConstantInt::get(*_llvm_context, llvm::APInt(size, literal->getValue(), is_signed));
}

auto IRGenerator::visitFloatLiteral(FloatLiteral *literal) -> llvm::Value * {
    if (literal->getType()->getName() == "f32") {
        return llvm::ConstantFP::get(*_llvm_context, llvm::APFloat(static_cast<float>(literal->getValue())));
    }
    return llvm::ConstantFP::get(*_llvm_context, llvm::APFloat(literal->getValue()));
}

auto IRGenerator::visitCharacterLiteral(CharacterLiteral *literal) -> llvm::Value * {
    return llvm::ConstantInt::get(*_llvm_context, llvm::APInt(8, literal->getValue(), false));
}

auto IRGenerator::visitStringLiteral(StringLiteral *literal) -> llvm::Value * {
    return _builder->CreateGlobalString(literal->getValue());
}

auto IRGenerator::visitVariableDeclaration(VariableDeclaration *variable) -> llvm::Value * {
    if (variable->getValue() != nullptr) {
        const auto value = variable->getValue()->acceptIRVisitor(this);
        _context.setValue(variable->getName(), value);
        return value;
    }

    _context.setValue(variable->getName(), nullptr);

    return nullptr;
}

auto IRGenerator::visitIdentifier(Identifier *identifier) -> llvm::Value * {
    const auto value = _context.getValue(identifier->getName());
    if (value == nullptr) {
        throw std::logic_error("Tried to access to a variable without a value set");
    }
    return value;
}

auto IRGenerator::visitBinaryCalcul(BinaryCalcul *calcul) -> llvm::Value * {
    const CalculBuilder builder(this, _builder.get());
    return builder.buildCalculValue(calcul);
}

auto IRGenerator::visitAssignation(Assignation *assignation) -> llvm::Value * {
    const auto value = assignation->getValue()->acceptIRVisitor(this);
    _context.setValue(assignation->getIdentifier(), value);
    return value;
}

auto IRGenerator::visitPointer(Pointer *pointer) -> llvm::Value * {
    const auto alloca = _builder->CreateAlloca(pointer->getPointedType()->getLLVMType(_llvm_context.get()));
    _builder->CreateStore(pointer->getValue()->acceptIRVisitor(this), alloca);

    return alloca;
}

auto IRGenerator::visitPointerDereferencing(PointerDereferencing *pointer) -> llvm::Value * {
    const auto pointer_value = pointer->getPointer()->acceptIRVisitor(this);
    return _builder->CreateLoad(pointer->getType()->getLLVMType(_llvm_context.get()), pointer_value);
}

auto IRGenerator::visitVariableAddress(VariableAddress *address) -> llvm::Value * {
    const auto value  = address->getVariable()->acceptIRVisitor(this);
    const auto alloca = _builder->CreateAlloca(address->getType()->getLLVMType(_llvm_context.get()));
    _builder->CreateStore(value, alloca);

    return alloca;
}

auto IRGenerator::visitArray(Array *array) -> llvm::Value * {
    const auto array_type   = array->getType()->getLLVMType(_llvm_context.get());
    const auto in_array_def = _visitor_context->has("in_array_def");
    const auto alloca
        = in_array_def
            ? nullptr
            : _builder->CreateAlloca(
                  array_type, llvm::ConstantInt::get(*_llvm_context, llvm::APInt(64, array->getFullSize(), false))
              );
    const auto &array_values = array->getValues();
    for (unsigned int i = 0; i < array_values.size(); ++i) {
        const auto array_value = in_array_def ? _visitor_context->get<llvm::Value *>("in_array_def") : alloca;
        _visitor_context->stack();

        const auto array_access = _builder->CreateConstInBoundsGEP2_64(array_type, array_value, 0, i);
        _visitor_context->set("in_array_def", array_access);
        const auto llvm_value = array_values[i]->acceptIRVisitor(this);

        if (! _visitor_context->has("was_in_array_def") || ! _visitor_context->get<bool>("was_in_array_def")) {
            _builder->CreateStore(llvm_value, array_access);
        }

        _visitor_context->unstack();
    }

    _visitor_context->set("was_in_array_def", true);

    return alloca;
}

auto IRGenerator::visitArrayAccess(ArrayAccess *array_access) -> llvm::Value * {
    _visitor_context->stack();
    _visitor_context->set("in_array_access", true);
    const auto value = array_access->getArray()->acceptIRVisitor(this);
    _visitor_context->unstack();
    const auto gep = _builder->CreateConstInBoundsGEP2_64(
        array_access->getArray()->getType()->getLLVMType(_llvm_context.get()), value, 0, array_access->getIndex()
    );

    if (_visitor_context->has("in_array_access") && _visitor_context->get<bool>("in_array_access")) {
        return gep;
    }

    return _builder->CreateLoad(array_access->getType()->getLLVMType(_llvm_context.get()), gep);
}
