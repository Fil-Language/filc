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
#ifndef FILC_POINTER_H
#define FILC_POINTER_H

#include "filc/grammar/expression/Expression.h"

#include <memory>
#include <string>

namespace filc {
class Pointer final : public Expression {
  public:
    Pointer(std::string type_name, const std::shared_ptr<Expression> &value);

    [[nodiscard]] auto getTypeName() const -> std::string;

    [[nodiscard]] auto getValue() const -> std::shared_ptr<Expression>;

    [[nodiscard]] auto getPointedType() const -> std::shared_ptr<AbstractType>;

    auto acceptVoidVisitor(Visitor<void> *visitor) -> void override;

    auto acceptIRVisitor(Visitor<llvm::Value *> *visitor) -> llvm::Value * override;

  private:
    std::string _type_name;
    std::shared_ptr<Expression> _value;
};

class PointerDereferencing final : public Expression {
  public:
    explicit PointerDereferencing(const std::shared_ptr<Expression> &pointer);

    [[nodiscard]] auto getPointer() const -> std::shared_ptr<Expression>;

    auto acceptVoidVisitor(Visitor<void> *visitor) -> void override;

    auto acceptIRVisitor(Visitor<llvm::Value *> *visitor) -> llvm::Value * override;

  private:
    std::shared_ptr<Expression> _pointer;
};

class VariableAddress final : public Expression {
  public:
    explicit VariableAddress(const std::shared_ptr<Expression> &variable);

    [[nodiscard]] auto getVariable() const -> std::shared_ptr<Expression>;

    auto acceptVoidVisitor(Visitor<void> *visitor) -> void override;

    auto acceptIRVisitor(Visitor<llvm::Value *> *visitor) -> llvm::Value * override;

  private:
    std::shared_ptr<Expression> _variable;
};
} // namespace filc

#endif // FILC_POINTER_H
