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
#include <filc/grammar/Parser.h>
#include <filc/grammar/expression/Expression.h>
#include <filc/grammar/program/Program.h>
#include <filc/validation/ValidationVisitor.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>

using namespace ::testing;

#define VISITOR                                                                                                        \
    std::stringstream ss;                                                                                              \
    filc::ValidationVisitor visitor(ss)

#define VALIDATION_FIXTURES FIXTURES_PATH "/validation"

TEST(ValidationVisitor, visitProgram_valid) {
    VISITOR;
    const auto program = filc::ParserProxy::parse(VALIDATION_FIXTURES "/valid_program.fil");
    program->accept(&visitor);
    ASSERT_THAT(std::string(std::istreambuf_iterator<char>(ss), {}), IsEmpty());
}

TEST(ValidationVisitor, visitProgram_invalid) {
    VISITOR;
    const auto program = filc::ParserProxy::parse(VALIDATION_FIXTURES "/invalid_program.fil");
    program->accept(&visitor);
    ASSERT_THAT(std::string(std::istreambuf_iterator<char>(ss), {}),
                HasSubstr("Expected type int aka i32 but got char* aka u8*"));
}

TEST(ValidationVisitor, visitBooleanLiteral) {
    VISITOR;
    const auto program = filc::ParserProxy::parse(VALIDATION_FIXTURES "/boolean.fil");
    program->accept(&visitor);
    ASSERT_THAT(std::string(std::istreambuf_iterator<char>(ss), {}), HasSubstr("Boolean value not used"));
}

TEST(ValidationVisitor, visitIntegerLiteral) {
    VISITOR;
    const auto program = filc::ParserProxy::parse(VALIDATION_FIXTURES "/integer.fil");
    program->accept(&visitor);
    ASSERT_THAT(std::string(std::istreambuf_iterator<char>(ss), {}), HasSubstr("Integer value not used"));
    ASSERT_STREQ("int", program->getExpressions()[0]->getType()->getDisplayName().c_str());
}

TEST(ValidationVisitor, visitFloatLiteral) {
    VISITOR;
    const auto program = filc::ParserProxy::parse(VALIDATION_FIXTURES "/float.fil");
    program->accept(&visitor);
    ASSERT_THAT(std::string(std::istreambuf_iterator<char>(ss), {}), HasSubstr("Float value not used"));
    ASSERT_STREQ("f64", program->getExpressions()[0]->getType()->getDisplayName().c_str());
}

TEST(ValidationVisitor, visitCharacterLiteral) {
    VISITOR;
    const auto program = filc::ParserProxy::parse(VALIDATION_FIXTURES "/character.fil");
    program->accept(&visitor);
    ASSERT_THAT(std::string(std::istreambuf_iterator<char>(ss), {}), HasSubstr("Character value not used"));
    ASSERT_STREQ("char", program->getExpressions()[0]->getType()->getDisplayName().c_str());
}

TEST(ValidationVisitor, visitStringLiteral) {
    VISITOR;
    const auto program = filc::ParserProxy::parse(VALIDATION_FIXTURES "/string.fil");
    program->accept(&visitor);
    ASSERT_THAT(std::string(std::istreambuf_iterator<char>(ss), {}), HasSubstr("String value not used"));
    ASSERT_STREQ("char*", program->getExpressions()[0]->getType()->getDisplayName().c_str());
}

TEST(ValidationVisitor, visitVariableDeclaration) {
    VISITOR;
    const auto program = filc::ParserProxy::parse(VALIDATION_FIXTURES "/variable.fil");
    program->accept(&visitor);
    ASSERT_THAT(std::string(std::istreambuf_iterator<char>(ss), {}), IsEmpty());
    ASSERT_STREQ("i32", program->getExpressions()[0]->getType()->getDisplayName().c_str());
}

TEST(ValidationVisitor, visitIdentifier) {
    VISITOR;
    const auto program = filc::ParserProxy::parse(VALIDATION_FIXTURES "/identifier.fil");
    program->accept(&visitor);
    ASSERT_THAT(std::string(std::istreambuf_iterator<char>(ss), {}), IsEmpty());
    ASSERT_STREQ("int", program->getExpressions()[1]->getType()->getDisplayName().c_str());
}

TEST(ValidationVisitor, visitBinaryCalcul) {
    VISITOR;
    const auto program = filc::ParserProxy::parse(VALIDATION_FIXTURES "/binary.fil");
    ASSERT_THROW(program->accept(&visitor), std::logic_error);
}

TEST(ValidationVisitor, visitAssignation) {
    VISITOR;
    const auto program = filc::ParserProxy::parse(VALIDATION_FIXTURES "/assignation.fil");
    ASSERT_THROW(program->accept(&visitor), std::logic_error);
}
