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
#ifndef FILC_IRGENERATOR_H
#define FILC_IRGENERATOR_H

#include "filc/grammar/Visitor.h"
#include "filc/validation/Environment.h"
#include "filc/llvm/GeneratorContext.h"
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <memory>

namespace filc {
class IRGenerator final: public Visitor<llvm::Value *> {
  public:
    explicit IRGenerator(const std::string &filename, const Environment *environment);

    ~IRGenerator() override = default;

    [[nodiscard]] auto dump() const -> std::string;

    [[nodiscard]] auto toTarget(const std::string &output_file, const std::string &target_triple) const -> int;

    auto visitProgram(Program *program) -> llvm::Value * override;

    auto visitBooleanLiteral(BooleanLiteral *literal) -> llvm::Value * override;

    auto visitIntegerLiteral(IntegerLiteral *literal) -> llvm::Value * override;

    auto visitFloatLiteral(FloatLiteral *literal) -> llvm::Value * override;

    auto visitCharacterLiteral(CharacterLiteral *literal) -> llvm::Value * override;

    auto visitStringLiteral(StringLiteral *literal) -> llvm::Value * override;

    auto visitVariableDeclaration(VariableDeclaration *variable) -> llvm::Value * override;

    auto visitIdentifier(Identifier *identifier) -> llvm::Value * override;

    auto visitBinaryCalcul(BinaryCalcul *calcul) -> llvm::Value * override;

    auto visitAssignation(Assignation *assignation) -> llvm::Value * override;

    auto visitPointer(Pointer *pointer) -> llvm::Value * override;

    auto visitPointerDereferencing(PointerDereferencing *pointer) -> llvm::Value * override;

    auto visitVariableAddress(VariableAddress *address) -> llvm::Value * override;

    auto visitArray(Array *array) -> llvm::Value * override;

    auto visitArrayAccess(ArrayAccess *array_access) -> llvm::Value * override;

  private:
    std::unique_ptr<VisitorContext> _visitor_context;
    std::unique_ptr<llvm::LLVMContext> _llvm_context;
    std::unique_ptr<llvm::Module> _module;
    std::unique_ptr<llvm::IRBuilder<>> _builder;
    GeneratorContext _context;
};
}

#endif // FILC_IRGENERATOR_H
