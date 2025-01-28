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
#ifndef FILC_VALIDATIONVISITOR_H
#define FILC_VALIDATIONVISITOR_H

#include "filc/grammar/Position.h"
#include "filc/grammar/Visitor.h"
#include "filc/validation/Environment.h"
#include "filc/validation/TypeBuilder.h"

#include <memory>
#include <string>

namespace filc {
class ValidationVisitor final : public Visitor<void> {
  public:
    explicit ValidationVisitor(std::ostream &out);

    [[nodiscard]] auto getEnvironment() const -> const Environment *;

    [[nodiscard]] auto hasError() const -> bool;

    auto visitProgram(Program *program) -> void override;

    auto visitBooleanLiteral(BooleanLiteral *literal) -> void override;

    auto visitIntegerLiteral(IntegerLiteral *literal) -> void override;

    auto visitFloatLiteral(FloatLiteral *literal) -> void override;

    auto visitCharacterLiteral(CharacterLiteral *literal) -> void override;

    auto visitStringLiteral(StringLiteral *literal) -> void override;

    auto visitVariableDeclaration(VariableDeclaration *variable) -> void override;

    auto visitIdentifier(Identifier *identifier) -> void override;

    auto visitBinaryCalcul(BinaryCalcul *calcul) -> void override;

    auto visitAssignation(Assignation *assignation) -> void override;

    auto visitPointer(Pointer *pointer) -> void override;

    auto visitPointerDereferencing(PointerDereferencing *pointer) -> void override;

    auto visitVariableAddress(VariableAddress *address) -> void override;

    auto visitArray(Array *array) -> void override;

    auto visitArrayAccess(ArrayAccess *array_access) -> void override;

  private:
    std::unique_ptr<VisitorContext> _context;
    std::unique_ptr<Environment> _environment;
    TypeBuilder _type_builder;
    std::ostream &_out;
    bool _error;

    auto displayError(const std::string &message, const Position &position) -> void;

    auto displayWarning(const std::string &message, const Position &position) const -> void;
};
}

#endif // FILC_VALIDATIONVISITOR_H
