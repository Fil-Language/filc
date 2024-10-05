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
#include "filc/grammar/assignation/Assignation.h"
#include "filc/grammar/calcul/Calcul.h"
#include "filc/grammar/identifier/Identifier.h"
#include "filc/grammar/variable/Variable.h"
#include <filc/grammar/literal/Literal.h>
#include <filc/grammar/program/Program.h>
#include <filc/llvm/IRGenerator.h>
#include <gtest/gtest.h>

TEST(IRGenerator, program_throw) {
    filc::IRGenerator generator("program");
    auto program = filc::Program({});
    ASSERT_THROW(program.acceptIRVisitor(&generator), std::logic_error);
}

TEST(IRGenerator, booleanLiteral_throw) {
    filc::IRGenerator generator("boolean_literal");
    auto literal = filc::BooleanLiteral(true);
    ASSERT_THROW(literal.acceptIRVisitor(&generator), std::logic_error);
}

TEST(IRGenerator, integerLiteral_throw) {
    filc::IRGenerator generator("integer_literal");
    auto literal = filc::IntegerLiteral(2);
    ASSERT_THROW(literal.acceptIRVisitor(&generator), std::logic_error);
}

TEST(IRGenerator, floatLiteral_throw) {
    filc::IRGenerator generator("float_literal");
    auto literal = filc::FloatLiteral(9.81);
    ASSERT_THROW(literal.acceptIRVisitor(&generator), std::logic_error);
}

TEST(IRGenerator, characterLiteral_throw) {
    filc::IRGenerator generator("character_literal");
    auto literal = filc::CharacterLiteral('a');
    ASSERT_THROW(literal.acceptIRVisitor(&generator), std::logic_error);
}

TEST(IRGenerator, stringLiteral_throw) {
    filc::IRGenerator generator("string_literal");
    auto literal = filc::StringLiteral("Hello");
    ASSERT_THROW(literal.acceptIRVisitor(&generator), std::logic_error);
}

TEST(IRGenerator, variableDeclaration_throw) {
    filc::IRGenerator generator("variable_declaration");
    auto variable = filc::VariableDeclaration(true, "my_val", "its_type", nullptr);
    ASSERT_THROW(variable.acceptIRVisitor(&generator), std::logic_error);
}

TEST(IRGenerator, identifier_throw) {
    filc::IRGenerator generator("identifier");
    auto identifier = filc::Identifier("identifier");
    ASSERT_THROW(identifier.acceptIRVisitor(&generator), std::logic_error);
}

TEST(IRGenerator, calcul_throw) {
    filc::IRGenerator generator("calcul");
    auto calcul = filc::BinaryCalcul(nullptr, "+", nullptr);
    ASSERT_THROW(calcul.acceptIRVisitor(&generator), std::logic_error);
}

TEST(IRGenerator, assignation_throw) {
    filc::IRGenerator generator("assignation");
    auto assignation = filc::Assignation("my_var", nullptr);
    ASSERT_THROW(assignation.acceptIRVisitor(&generator), std::logic_error);
}
