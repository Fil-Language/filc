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
#ifndef FILC_CALCUL_H
#define FILC_CALCUL_H

#include "filc/grammar/expression/Expression.h"
#include <memory>
#include <string>

namespace filc {
class BinaryCalcul final: public Expression {
  public:
    BinaryCalcul(std::shared_ptr<Expression> left_expression, std::string op, std::shared_ptr<Expression> right_expression);

    [[nodiscard]] auto getLeftExpression() const -> std::shared_ptr<Expression>;

    [[nodiscard]] auto getOperator() const -> std::string;

    [[nodiscard]] auto getRightExpression() const -> std::shared_ptr<Expression>;

    auto acceptVoidVisitor(Visitor<void> *visitor) -> void override;

    auto acceptIRVisitor(Visitor<llvm::Value *> *visitor) -> llvm::Value * override;

  private:
    std::shared_ptr<Expression> _left_expression;
    std::string _operator;
    std::shared_ptr<Expression> _right_expression;
};
}

#endif // FILC_CALCUL_H
