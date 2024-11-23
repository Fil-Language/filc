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
#include "filc/grammar/program/Program.h"
#include "filc/grammar/variable/Variable.h"
#include "filc/llvm/CalculBuilder.h"
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
    _llvm_context = std::make_unique<llvm::LLVMContext>();
    _module = std::make_unique<llvm::Module>(llvm::StringRef(filename), *_llvm_context);
    _builder = std::make_unique<llvm::IRBuilder<>>(*_llvm_context);
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
    if (!target) {
        std::cerr << error;
        return 1;
    }
    const llvm::TargetOptions options;
    const auto target_machine = target->createTargetMachine(used_target_triple, "", "", options, llvm::Reloc::PIC_);

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
    auto function_type = llvm::FunctionType::get(llvm::Type::getInt32Ty(*_llvm_context), {}, false);
    auto function = llvm::Function::Create(function_type, llvm::Function::ExternalLinkage, "main", _module.get());
    auto basic_block = llvm::BasicBlock::Create(*_llvm_context, "entry", function);
    _builder->SetInsertPoint(basic_block);

    const auto &expressions = program->getExpressions();
    if (expressions.empty()) {
        const auto return_value = llvm::ConstantInt::get(*_llvm_context, llvm::APInt(32, 0, true));
        _builder->CreateRet(return_value);
    } else {
        for (auto it = expressions.begin(); it != expressions.end(); it++) {
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
    const auto size = stoi(type_name.substr(1));
    return llvm::ConstantInt::get(*_llvm_context, llvm::APInt(size, literal->getValue(), is_signed));
}

auto IRGenerator::visitFloatLiteral(FloatLiteral *literal) -> llvm::Value * {
    const auto type_name = literal->getType()->getName();
    if (type_name == "f32") {
        return llvm::ConstantFP::get(*_llvm_context, llvm::APFloat((float)literal->getValue()));
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
    const auto global = new llvm::GlobalVariable(variable->getType()->getLLVMType(), variable->isConstant(),
                                                 llvm::GlobalValue::InternalLinkage);
    global->setName(variable->getName());
    _module->insertGlobalVariable(global);

    if (variable->getValue() != nullptr) {
        const auto value = variable->getValue()->acceptIRVisitor(this);
        global->setInitializer((llvm::Constant *)value);
        return value;
    }

    return global;
}

auto IRGenerator::visitIdentifier(Identifier *identifier) -> llvm::Value * {
    const auto variable = _module->getNamedGlobal(identifier->getName());
    return _builder->CreateLoad(variable->getValueType(), variable);
}

auto IRGenerator::visitBinaryCalcul(BinaryCalcul *calcul) -> llvm::Value * {
    CalculBuilder builder(this, _builder.get());
    return builder.buildCalculValue(calcul);
}

auto IRGenerator::visitAssignation(Assignation *assignation) -> llvm::Value * {
    const auto variable = _module->getNamedGlobal(assignation->getIdentifier());
    const auto value = assignation->getValue()->acceptIRVisitor(this);
    _builder->CreateStore(value, variable);
    return value;
}
