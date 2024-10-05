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
#include "filc/llvm/IRGenerator.h"
#include "filc/grammar/literal/Literal.h"
#include "filc/grammar/program/Program.h"

using namespace filc;

IRGenerator::IRGenerator(const std::string &filename) {
    _module = std::make_unique<llvm::Module>(llvm::StringRef(filename), _llvm_context);
    _builder = std::make_unique<llvm::IRBuilder<>>(_llvm_context);
}

auto IRGenerator::visitProgram(Program *program) -> llvm::Value * { throw std::logic_error("Not implemented yet"); }

auto IRGenerator::visitBooleanLiteral(BooleanLiteral *literal) -> llvm::Value * {
    throw std::logic_error("Not implemented yet");
}

auto IRGenerator::visitIntegerLiteral(IntegerLiteral *literal) -> llvm::Value * {
    throw std::logic_error("Not implemented yet");
}

auto IRGenerator::visitFloatLiteral(FloatLiteral *literal) -> llvm::Value * {
    throw std::logic_error("Not implemented yet");
}

auto IRGenerator::visitCharacterLiteral(CharacterLiteral *literal) -> llvm::Value * {
    throw std::logic_error("Not implemented yet");
}

auto IRGenerator::visitStringLiteral(StringLiteral *literal) -> llvm::Value * {
    throw std::logic_error("Not implemented yet");
}

auto IRGenerator::visitVariableDeclaration(VariableDeclaration *variable) -> llvm::Value * {
    throw std::logic_error("Not implemented yet");
}

auto IRGenerator::visitIdentifier(Identifier *identifier) -> llvm::Value * {
    throw std::logic_error("Not implemented yet");
}

auto IRGenerator::visitBinaryCalcul(BinaryCalcul *calcul) -> llvm::Value * {
    throw std::logic_error("Not implemented yet");
}

auto IRGenerator::visitAssignation(Assignation *assignation) -> llvm::Value * {
    throw std::logic_error("Not implemented yet");
}
