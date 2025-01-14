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

#include <llvm/IR/DerivedTypes.h>
#include <utility>

using namespace filc;

auto AbstractType::setLLVMType(llvm::Type *type) -> void {
    _llvm_type = type;
}

auto AbstractType::getLLVMType(llvm::LLVMContext *context) -> llvm::Type * {
    if (_llvm_type == nullptr) {
        generateLLVMType(context);
    }

    return _llvm_type;
}

Type::Type(std::string name): _name(std::move(name)) {}

auto Type::getName() const noexcept -> std::string {
    return _name;
}

auto Type::getDisplayName() const noexcept -> std::string {
    return getName();
}

auto Type::toDisplay() const noexcept -> std::string {
    return getName();
}

auto Type::generateLLVMType(llvm::LLVMContext *context) -> void {
    throw std::logic_error("Should not be called for scalar types");
}

PointerType::PointerType(std::shared_ptr<AbstractType> pointed_type): _pointed_type(std::move(pointed_type)) {}

auto PointerType::getName() const noexcept -> std::string {
    return _pointed_type->getName() + "*";
}

auto PointerType::getDisplayName() const noexcept -> std::string {
    return _pointed_type->getDisplayName() + "*";
}

auto PointerType::toDisplay() const noexcept -> std::string {
    if (_pointed_type->getName() != _pointed_type->getDisplayName()) {
        return getDisplayName() + " aka " + getName();
    }
    return getName();
}

auto PointerType::getPointedType() const noexcept -> std::shared_ptr<AbstractType> {
    return _pointed_type;
}

auto PointerType::generateLLVMType(llvm::LLVMContext *context) -> void {
    setLLVMType(llvm::PointerType::getUnqual(_pointed_type->getLLVMType(context)));
}

ArrayType::ArrayType(const unsigned int size, std::shared_ptr<AbstractType> contained_type)
    : _size(size), _contained_type(std::move(contained_type)) {}

auto ArrayType::getName() const noexcept -> std::string {
    return _contained_type->getName() + "[" + std::to_string(_size) + "]";
}

auto ArrayType::getDisplayName() const noexcept -> std::string {
    return _contained_type->getDisplayName() + "[" + std::to_string(_size) + "]";
}

auto ArrayType::getContainedType() const noexcept -> std::shared_ptr<AbstractType> {
    return _contained_type;
}

auto ArrayType::toDisplay() const noexcept -> std::string {
    if (_contained_type->getName() != _contained_type->getDisplayName()) {
        return getDisplayName() + " aka " + getName();
    }
    return getName();
}

auto ArrayType::generateLLVMType(llvm::LLVMContext *context) -> void {
    setLLVMType(llvm::ArrayType::get(_contained_type->getLLVMType(context), _size));
}

AliasType::AliasType(std::string name, std::shared_ptr<AbstractType> aliased_type)
    : _name(std::move(name)), _aliased_type(std::move(aliased_type)) {}

auto AliasType::getName() const noexcept -> std::string {
    return _aliased_type->getName();
}

auto AliasType::getDisplayName() const noexcept -> std::string {
    return _name;
}

auto AliasType::toDisplay() const noexcept -> std::string {
    return getDisplayName() + " aka " + getName();
}

auto AliasType::generateLLVMType(llvm::LLVMContext *context) -> void {
    throw std::logic_error("Should not be called for scalar alias types");
}

auto operator==(const std::shared_ptr<AbstractType> &a, const std::shared_ptr<AbstractType> &b) -> bool {
    return a->getName() == b->getName();
}

auto operator!=(const std::shared_ptr<AbstractType> &a, const std::shared_ptr<AbstractType> &b) -> bool {
    return ! (a == b);
}
