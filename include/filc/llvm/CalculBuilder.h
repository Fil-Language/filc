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
#ifndef FILC_CALCULBUILDER_H
#define FILC_CALCULBUILDER_H

#include "filc/llvm/IRGenerator.h"
#include <llvm/IR/IRBuilder.h>

namespace filc {
class CalculBuilder final {
  public:
    explicit CalculBuilder(IRGenerator *generator, llvm::IRBuilder<> *builder);

    auto buildCalculValue(const BinaryCalcul *calcul) const -> llvm::Value *;

  private:
    IRGenerator *_generator;
    llvm::IRBuilder<> *_builder;

    auto buildSignedInteger(const BinaryCalcul *calcul) const -> llvm::Value *;

    auto buildUnsignedInteger(const BinaryCalcul *calcul) const -> llvm::Value *;

    auto buildFloat(const BinaryCalcul *calcul) const -> llvm::Value *;

    auto buildBool(const BinaryCalcul *calcul) const -> llvm::Value *;

    auto buildPointer(const BinaryCalcul *calcul) const -> llvm::Value *;

    auto static buildError(const BinaryCalcul *calcul) -> std::logic_error;
};
}

#endif // FILC_CALCULBUILDER_H
