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

#include "filc/grammar/Visitor.h"
#include <memory>
#include <map>
#include <any>
#include <string>
#include <stdexcept>

namespace filc {
class ValidationContext final {
  public:
    explicit ValidationContext(ValidationContext* parent = nullptr);

    [[nodiscard]] auto stack() -> ValidationContext*;

    [[nodiscard]] auto unstack() const -> ValidationContext*;

    auto set(const std::string &key, const std::any &value) -> void;

    [[nodiscard]] auto has(const std::string &key) const -> bool;

    template<typename T>
    auto get(const std::string &key) const -> T {
        if (_values.find(key) == _values.end()) {
            throw std::logic_error("There is not value for key: " + key);
        }

        return std::any_cast<T>(_values.at(key));
    }

  private:
    ValidationContext* _parent;
    std::map<std::string, std::any> _values;
};

class ValidationVisitor final : public Visitor {
  public:
    explicit ValidationVisitor(std::ostream &out);

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

  private:
    std::unique_ptr<ValidationContext> _context;
    std::ostream &_out;
};
}

#endif // FILC_VALIDATIONVISITOR_H
