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
#include "filc/grammar/Type.h"
#include <utility>

using namespace filc;

auto AbstractType::setLLVMType(llvm::Type *type) -> void { _llvm_type = type; }

auto AbstractType::getLLVMType() const -> llvm::Type * { return _llvm_type; }

Type::Type(std::string name) : _name(std::move(name)) {}

auto Type::getName() const noexcept -> std::string { return _name; }

auto Type::getDisplayName() const noexcept -> std::string { return getName(); }

auto Type::toDisplay() const noexcept -> std::string { return getName(); }

PointerType::PointerType(std::shared_ptr<AbstractType> pointed_type) : _pointed_type(std::move(pointed_type)) {}

auto PointerType::getName() const noexcept -> std::string { return _pointed_type->getName() + "*"; }

auto PointerType::getDisplayName() const noexcept -> std::string { return _pointed_type->getDisplayName() + "*"; }

auto PointerType::toDisplay() const noexcept -> std::string {
    if (_pointed_type->getName() != _pointed_type->getDisplayName()) {
        return getDisplayName() + " aka " + getName();
    }
    return getName();
}

AliasType::AliasType(std::string name, std::shared_ptr<AbstractType> aliased_type)
    : _name(std::move(name)), _aliased_type(std::move(aliased_type)) {}

auto AliasType::getName() const noexcept -> std::string { return _aliased_type->getName(); }

auto AliasType::getDisplayName() const noexcept -> std::string { return _name; }

auto AliasType::toDisplay() const noexcept -> std::string { return getDisplayName() + " aka " + getName(); }

auto operator==(const std::shared_ptr<AbstractType> &a, const std::shared_ptr<AbstractType> &b) -> bool {
    return a->getName() == b->getName();
}

auto operator!=(const std::shared_ptr<AbstractType> &a, const std::shared_ptr<AbstractType> &b) -> bool {
    return !(a == b);
}
