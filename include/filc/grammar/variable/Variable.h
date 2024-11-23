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
#ifndef FILC_VARIABLE_H
#define FILC_VARIABLE_H

#include "filc/grammar/expression/Expression.h"
#include <string>
#include <memory>

namespace filc {
class VariableDeclaration final: public Expression {
  public:
    VariableDeclaration(bool is_constant, std::string name, std::string _type_name, std::shared_ptr<Expression> value);

    [[nodiscard]] auto isConstant() const -> bool;

    [[nodiscard]] auto getName() const -> std::string;

    [[nodiscard]] auto getTypeName() const -> std::string;

    [[nodiscard]] auto getValue() const -> std::shared_ptr<Expression>;

    auto acceptVoidVisitor(Visitor<void> *visitor) -> void override;

    auto acceptIRVisitor(Visitor<llvm::Value *> *visitor) -> llvm::Value * override;

  private:
    bool _constant;
    std::string _name;
    std::string _type_name;
    std::shared_ptr<Expression> _value;
};
}

#endif // FILC_VARIABLE_H
