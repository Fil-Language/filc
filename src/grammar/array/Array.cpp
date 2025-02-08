/**
 * MIT License
 *
 * Copyright (c) 2025-Present Kevin Traini
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
#include "filc/grammar/array/Array.h"

using namespace filc;

Array::Array(const std::vector<std::shared_ptr<Expression>> &values)
    : _size(values.size()), _full_size(0), _values(values) {}

auto Array::getValues() const -> const std::vector<std::shared_ptr<Expression>> & {
    return _values;
}

auto Array::getSize() const -> unsigned long {
    return _size;
}

auto Array::setFullSize(const unsigned long full_size) -> void {
    _full_size = full_size;
}

auto Array::getFullSize() const -> unsigned long {
    return _full_size;
}

auto Array::acceptVoidVisitor(Visitor<void> *visitor) -> void {
    visitor->visitArray(this);
}

auto Array::acceptIRVisitor(Visitor<llvm::Value *> *visitor) -> llvm::Value * {
    return visitor->visitArray(this);
}
