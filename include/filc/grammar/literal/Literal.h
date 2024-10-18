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
#ifndef FILC_LITERAL_H
#define FILC_LITERAL_H

#include "filc/grammar/ast.h"
#include "filc/grammar/expression/Expression.h"

namespace filc {
template<typename T>
class Literal: public Expression {
  public:
    [[nodiscard]] auto getValue() const -> T {
        return _value;
    }

  private:
    T _value;

  protected:
    explicit Literal(T value): _value(value) {};
};

class BooleanLiteral final: public Literal<bool> {
  public:
    explicit BooleanLiteral(bool value);

    auto acceptVoidVisitor(Visitor<void> *visitor) -> void override;

    auto acceptIRVisitor(Visitor<llvm::Value *> *visitor) -> llvm::Value * override;
};

class IntegerLiteral final: public Literal<int> {
  public:
    explicit IntegerLiteral(int value);

    auto acceptVoidVisitor(Visitor<void> *visitor) -> void override;

    auto acceptIRVisitor(Visitor<llvm::Value *> *visitor) -> llvm::Value * override;
};

class FloatLiteral final: public Literal<double> {
  public:
    explicit FloatLiteral(double value);

    auto acceptVoidVisitor(Visitor<void> *visitor) -> void override;

    auto acceptIRVisitor(Visitor<llvm::Value *> *visitor) -> llvm::Value * override;
};

class CharacterLiteral final: public Literal<char> {
  public:
    explicit CharacterLiteral(char value);

    static auto stringToChar(const std::string &snippet) -> char;

    auto acceptVoidVisitor(Visitor<void> *visitor) -> void override;

    auto acceptIRVisitor(Visitor<llvm::Value *> *visitor) -> llvm::Value * override;
};

class StringLiteral final: public Literal<std::string> {
  public:
    explicit StringLiteral(const std::string &value);

    auto acceptVoidVisitor(Visitor<void> *visitor) -> void override;

    auto acceptIRVisitor(Visitor<llvm::Value *> *visitor) -> llvm::Value * override;
};
}

#endif // FILC_LITERAL_H
