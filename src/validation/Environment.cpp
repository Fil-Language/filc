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
#include "filc/validation/Environment.h"

#include <llvm/IR/DerivedTypes.h>
#include <stdexcept>

using namespace filc;

Environment::Environment() {
    addType(std::make_shared<Type>("i8"));
    addType(std::make_shared<Type>("i16"));
    addType(std::make_shared<Type>("i32"));
    addType(std::make_shared<Type>("i64"));
    addType(std::make_shared<Type>("i128"));
    addType(std::make_shared<AliasType>("int", getType("i32")));

    addType(std::make_shared<Type>("u8"));
    addType(std::make_shared<Type>("u16"));
    addType(std::make_shared<Type>("u32"));
    addType(std::make_shared<Type>("u64"));
    addType(std::make_shared<Type>("u128"));
    addType(std::make_shared<AliasType>("uint", getType("u32")));

    addType(std::make_shared<Type>("f32"));
    addType(std::make_shared<Type>("f64"));

    addType(std::make_shared<Type>("bool"));

    addType(std::make_shared<AliasType>("char", getType("u8")));
    addType(std::make_shared<PointerType>(getType("char")));

    addType(std::make_shared<Type>("void"));
}

auto Environment::prepareLLVMTypes(llvm::LLVMContext *context) const -> void {
    getType("i8")->setLLVMType(llvm::Type::getIntNTy(*context, 8));
    getType("i16")->setLLVMType(llvm::Type::getIntNTy(*context, 16));
    getType("i32")->setLLVMType(llvm::Type::getIntNTy(*context, 32));
    getType("i64")->setLLVMType(llvm::Type::getIntNTy(*context, 64));
    getType("i128")->setLLVMType(llvm::Type::getIntNTy(*context, 128));
    getType("int")->setLLVMType(llvm::Type::getIntNTy(*context, 32));

    getType("u8")->setLLVMType(llvm::Type::getIntNTy(*context, 8));
    getType("u16")->setLLVMType(llvm::Type::getIntNTy(*context, 16));
    getType("u32")->setLLVMType(llvm::Type::getIntNTy(*context, 32));
    getType("u64")->setLLVMType(llvm::Type::getIntNTy(*context, 64));
    getType("u128")->setLLVMType(llvm::Type::getIntNTy(*context, 128));
    getType("uint")->setLLVMType(llvm::Type::getIntNTy(*context, 32));

    getType("f32")->setLLVMType(llvm::Type::getFloatTy(*context));
    getType("f64")->setLLVMType(llvm::Type::getDoubleTy(*context));

    getType("bool")->setLLVMType(llvm::Type::getInt1Ty(*context));

    getType("char")->setLLVMType(llvm::Type::getInt8Ty(*context));
    getType("char*")->setLLVMType(llvm::PointerType::get(llvm::Type::getInt8Ty(*context), 0));

    getType("void")->setLLVMType(llvm::Type::getVoidTy(*context));
}

auto Environment::hasType(const std::string &name) const -> bool {
    return _types.find(name) != _types.end();
}

auto Environment::getType(const std::string &name) const -> const std::shared_ptr<AbstractType> & {
    if (! hasType(name)) {
        throw std::logic_error("Environment doesn't have type " + name);
    }
    return _types.at(name);
}

auto Environment::addType(const std::shared_ptr<AbstractType> &type) -> void {
    if (hasType(type->getDisplayName())) {
        throw std::logic_error("Environment already have type " + type->getDisplayName() + " aka " + type->getName());
    }
    _types[type->getDisplayName()] = type;
}

auto Environment::hasName(const std::string &name) const -> bool {
    return _names.find(name) != _names.end();
}

auto Environment::getName(const std::string &name) const -> const Name & {
    if (! hasName(name)) {
        throw std::logic_error("Environment doesn't have name " + name);
    }
    return _names.at(name);
}

auto Environment::addName(const Name &name) -> void {
    if (hasName(name.getName())) {
        throw std::logic_error("Environment already have name " + name.getName());
    }
    _names[name.getName()] = name;
}

auto Environment::setName(const Name &name) -> void {
    if (! hasName(name.getName())) {
        throw std::logic_error("Cannot change a name which does not exists: " + name.getName());
    }
    _names[name.getName()] = name;
}
