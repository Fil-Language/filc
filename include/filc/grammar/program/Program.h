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
#ifndef FILC_PROGRAM_H
#define FILC_PROGRAM_H

#include "filc/grammar/ast.h"
#include "filc/grammar/Visitor.h"
#include <vector>
#include <memory>

namespace filc {
class Program final: public Visitable {
  public:
    explicit Program(const std::vector<std::shared_ptr<Expression>> &expressions);

    [[nodiscard]] auto getExpressions() const -> const std::vector<std::shared_ptr<Expression>> &;

    auto acceptVoidVisitor(Visitor<void> *visitor) -> void override;

    auto acceptIRVisitor(Visitor<llvm::Value *> *visitor) -> llvm::Value * override;

  private:
    std::vector<std::shared_ptr<Expression>> _expressions;
};
}

#endif // FILC_PROGRAM_H
