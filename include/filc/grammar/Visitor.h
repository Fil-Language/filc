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
#ifndef FILC_VISITOR_H
#define FILC_VISITOR_H

#include "filc/grammar/ast.h"

#include <any>
#include <llvm/IR/Value.h>
#include <map>
#include <stack>
#include <stdexcept>

namespace filc {
template<typename Return> class Visitor {
  public:
    virtual ~Visitor() = default;

    virtual auto visitProgram(Program *program) -> Return = 0;

    virtual auto visitBooleanLiteral(BooleanLiteral *literal) -> Return = 0;

    virtual auto visitIntegerLiteral(IntegerLiteral *literal) -> Return = 0;

    virtual auto visitFloatLiteral(FloatLiteral *literal) -> Return = 0;

    virtual auto visitCharacterLiteral(CharacterLiteral *literal) -> Return = 0;

    virtual auto visitStringLiteral(StringLiteral *literal) -> Return = 0;

    virtual auto visitVariableDeclaration(VariableDeclaration *variable) -> Return = 0;

    virtual auto visitIdentifier(Identifier *identifier) -> Return = 0;

    virtual auto visitBinaryCalcul(BinaryCalcul *calcul) -> Return = 0;

    virtual auto visitAssignation(Assignation *assignation) -> Return = 0;

    virtual auto visitPointer(Pointer *pointer) -> Return = 0;

    virtual auto visitPointerDereferencing(PointerDereferencing *pointer) -> Return = 0;

    virtual auto visitVariableAddress(VariableAddress *address) -> Return = 0;

    virtual auto visitArray(Array *array) -> Return = 0;

    virtual auto visitArrayAccess(ArrayAccess *array_access) -> Return = 0;

  protected:
    Visitor() = default;
};

class Visitable {
  public:
    virtual ~Visitable() = default;

    virtual auto acceptVoidVisitor(Visitor<void> *visitor) -> void = 0;

    virtual auto acceptIRVisitor(Visitor<llvm::Value *> *visitor) -> llvm::Value * = 0;
};

class VisitorContext final {
  public:
    VisitorContext();

    auto stack() -> void;

    auto unstack() -> void;

    auto set(const std::string &key, const std::any &value) -> void;

    auto unset(const std::string &key) -> void;

    [[nodiscard]] auto has(const std::string &key) const -> bool;

    template<typename T>
    auto get(const std::string &key) const -> T {
        if (_values.top().find(key) == _values.top().end()) {
            throw std::logic_error("There is not value for key: " + key);
        }

        return std::any_cast<T>(_values.top().at(key));
    }

    auto clear() -> void;

  private:
    std::stack<std::map<std::string, std::any>> _values;
};
} // namespace filc

#endif // FILC_VISITOR_H
