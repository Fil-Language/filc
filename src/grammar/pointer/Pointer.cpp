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
#include "filc/grammar/pointer/Pointer.h"

using namespace filc;

Pointer::Pointer(std::string type_name, const std::shared_ptr<Expression> &value)
    : _type_name(std::move(type_name)), _value(value) {}

auto Pointer::getTypeName() const -> std::string {
    return _type_name;
}

auto Pointer::getValue() const -> std::shared_ptr<Expression> {
    return _value;
}

auto Pointer::getPointedType() const -> std::shared_ptr<AbstractType> {
    const auto type = std::dynamic_pointer_cast<PointerType>(getType());
    if (type == nullptr) {
        return nullptr;
    }

    return type->getPointedType();
}

auto Pointer::acceptVoidVisitor(Visitor<void> *visitor) -> void {
    visitor->visitPointer(this);
}

auto Pointer::acceptIRVisitor(Visitor<llvm::Value *> *visitor) -> llvm::Value * {
    return visitor->visitPointer(this);
}
