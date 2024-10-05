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
#include "test_tools.h"
#include <filc/grammar/Parser.h>
#include <filc/grammar/expression/Expression.h>
#include <filc/validation/ValidationVisitor.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>

using namespace ::testing;

#define VISITOR                                                                                                        \
    std::stringstream ss;                                                                                              \
    filc::ValidationVisitor visitor(ss)

#define VALIDATION_FIXTURES FIXTURES_PATH "/validation"

TEST(ValidationVisitor, program_valid) {
    VISITOR;
    const auto program = parseString("0");
    program->acceptVoidVisitor(&visitor);
    ASSERT_THAT(std::string(std::istreambuf_iterator<char>(ss), {}), IsEmpty());
    ASSERT_FALSE(visitor.hasError());
}

TEST(ValidationVisitor, program_invalid) {
    VISITOR;
    const auto program = parseString("'a'");
    program->acceptVoidVisitor(&visitor);
    ASSERT_THAT(std::string(std::istreambuf_iterator<char>(ss), {}),
                HasSubstr("Expected type int aka i32 but got char"));
    ASSERT_TRUE(visitor.hasError());
}

TEST(ValidationVisitor, boolean) {
    VISITOR;
    const auto program = parseString("true\n0");
    program->acceptVoidVisitor(&visitor);
    ASSERT_THAT(std::string(std::istreambuf_iterator<char>(ss), {}), HasSubstr("Boolean value not used"));
    ASSERT_FALSE(visitor.hasError());
}

TEST(ValidationVisitor, integer) {
    VISITOR;
    const auto program = parseString("2\n0");
    program->acceptVoidVisitor(&visitor);
    ASSERT_THAT(std::string(std::istreambuf_iterator<char>(ss), {}), HasSubstr("Integer value not used"));
    ASSERT_FALSE(visitor.hasError());
    ASSERT_STREQ("int", program->getExpressions()[0]->getType()->getDisplayName().c_str());
}

TEST(ValidationVisitor, float) {
    VISITOR;
    const auto program = parseString("3.14\n0");
    program->acceptVoidVisitor(&visitor);
    ASSERT_THAT(std::string(std::istreambuf_iterator<char>(ss), {}), HasSubstr("Float value not used"));
    ASSERT_FALSE(visitor.hasError());
    ASSERT_STREQ("f64", program->getExpressions()[0]->getType()->getDisplayName().c_str());
}

TEST(ValidationVisitor, character) {
    VISITOR;
    const auto program = parseString("'a'\n0");
    program->acceptVoidVisitor(&visitor);
    ASSERT_THAT(std::string(std::istreambuf_iterator<char>(ss), {}), HasSubstr("Character value not used"));
    ASSERT_FALSE(visitor.hasError());
    ASSERT_STREQ("char", program->getExpressions()[0]->getType()->getDisplayName().c_str());
}

TEST(ValidationVisitor, string) {
    VISITOR;
    const auto program = parseString("\"hello\"\n0");
    program->acceptVoidVisitor(&visitor);
    ASSERT_THAT(std::string(std::istreambuf_iterator<char>(ss), {}), HasSubstr("String value not used"));
    ASSERT_FALSE(visitor.hasError());
    ASSERT_STREQ("char*", program->getExpressions()[0]->getType()->getDisplayName().c_str());
}

TEST(ValidationVisitor, variable_alreadyDefined) {
    VISITOR;
    const auto program = parseString("val my_constant = 2\nval my_constant = 3");
    program->acceptVoidVisitor(&visitor);
    ASSERT_THAT(std::string(std::istreambuf_iterator<char>(ss), {}), HasSubstr("my_constant is already defined"));
    ASSERT_TRUE(visitor.hasError());
    ASSERT_STREQ("int", program->getExpressions()[0]->getType()->getDisplayName().c_str());
    ASSERT_EQ(nullptr, program->getExpressions()[1]->getType());
}

TEST(ValidationVisitor, variable_constantWithoutValue) {
    VISITOR;
    const auto program = parseString("val my_constant");
    program->acceptVoidVisitor(&visitor);
    ASSERT_THAT(std::string(std::istreambuf_iterator<char>(ss), {}),
                HasSubstr("When declaring a constant, you must provide it a value"));
    ASSERT_TRUE(visitor.hasError());
    ASSERT_EQ(nullptr, program->getExpressions()[0]->getType());
}

TEST(ValidationVisitor, variable_unknowType) {
    VISITOR;
    const auto program = parseString("var my_var: foo");
    program->acceptVoidVisitor(&visitor);
    ASSERT_THAT(std::string(std::istreambuf_iterator<char>(ss), {}), HasSubstr("Unknown type: foo"));
    ASSERT_TRUE(visitor.hasError());
    ASSERT_EQ(nullptr, program->getExpressions()[0]->getType());
}

TEST(ValidationVisitor, variable_differentValueType) {
    VISITOR;
    const auto program = parseString("var my_var: i32 = 'a'");
    program->acceptVoidVisitor(&visitor);
    ASSERT_THAT(std::string(std::istreambuf_iterator<char>(ss), {}),
                HasSubstr("Cannot assign value of type char aka u8 to a variable of type i32"));
    ASSERT_TRUE(visitor.hasError());
    ASSERT_EQ(nullptr, program->getExpressions()[0]->getType());
}

TEST(ValidationVisitor, variable_assignAliasType) {
    VISITOR;
    const auto program = parseString("var my_var: u8 = 'a'\n0");
    program->acceptVoidVisitor(&visitor);
    ASSERT_THAT(std::string(std::istreambuf_iterator<char>(ss), {}), IsEmpty());
    ASSERT_FALSE(visitor.hasError());
    ASSERT_STREQ("u8", program->getExpressions()[0]->getType()->getDisplayName().c_str());
}

TEST(ValidationVisitor, variable_withoutTypeAndValue) {
    VISITOR;
    const auto program = parseString("var my_var");
    program->acceptVoidVisitor(&visitor);
    ASSERT_THAT(std::string(std::istreambuf_iterator<char>(ss), {}),
                HasSubstr("When declaring a variable, you must provide at least a type or a value"));
    ASSERT_TRUE(visitor.hasError());
    ASSERT_EQ(nullptr, program->getExpressions()[0]->getType());
}

TEST(ValidationVisitor, variable_valid) {
    VISITOR;
    const auto program = parseString("val foo: i32 = 45");
    program->acceptVoidVisitor(&visitor);
    ASSERT_THAT(std::string(std::istreambuf_iterator<char>(ss), {}), IsEmpty());
    ASSERT_FALSE(visitor.hasError());
    ASSERT_STREQ("i32", program->getExpressions()[0]->getType()->getDisplayName().c_str());
}

TEST(ValidationVisitor, identifier_nonExisting) {
    VISITOR;
    const auto program = parseString("bar");
    program->acceptVoidVisitor(&visitor);
    ASSERT_THAT(std::string(std::istreambuf_iterator<char>(ss), {}),
                HasSubstr("Unknown name, don't know what it refers to: bar"));
    ASSERT_TRUE(visitor.hasError());
    ASSERT_EQ(nullptr, program->getExpressions()[0]->getType());
}

TEST(ValidationVisitor, identifier_valid) {
    VISITOR;
    const auto program = parseString("val foo = 1\nfoo");
    program->acceptVoidVisitor(&visitor);
    ASSERT_THAT(std::string(std::istreambuf_iterator<char>(ss), {}), IsEmpty());
    ASSERT_FALSE(visitor.hasError());
    ASSERT_STREQ("int", program->getExpressions()[1]->getType()->getDisplayName().c_str());
}

TEST(ValidationVisitor, calcul_invalidLeft) {
    VISITOR;
    const auto program = parseString("(val foo) + 2");
    program->acceptVoidVisitor(&visitor);
    ASSERT_THAT(std::string(std::istreambuf_iterator<char>(ss), {}),
                HasSubstr("When declaring a constant, you must provide it a value"));
    ASSERT_TRUE(visitor.hasError());
    ASSERT_EQ(nullptr, program->getExpressions()[0]->getType());
}

TEST(ValidationVisitor, calcul_invalidRight) {
    VISITOR;
    const auto program = parseString("2 + (val foo)");
    program->acceptVoidVisitor(&visitor);
    ASSERT_THAT(std::string(std::istreambuf_iterator<char>(ss), {}),
                HasSubstr("When declaring a constant, you must provide it a value"));
    ASSERT_TRUE(visitor.hasError());
    ASSERT_EQ(nullptr, program->getExpressions()[0]->getType());
}

TEST(ValidationVisitor, calcul_invalid) {
    VISITOR;
    const auto program = parseString("'a' && 3");
    program->acceptVoidVisitor(&visitor);
    ASSERT_THAT(std::string(std::istreambuf_iterator<char>(ss), {}),
                HasSubstr("You cannot use operator && with char aka u8 and int aka i32"));
    ASSERT_TRUE(visitor.hasError());
    ASSERT_EQ(nullptr, program->getExpressions()[0]->getType());
}

TEST(ValidationVisitor, calcul_valid) {
    VISITOR;
    const auto program = parseString("2 + 2");
    program->acceptVoidVisitor(&visitor);
    ASSERT_THAT(std::string(std::istreambuf_iterator<char>(ss), {}), IsEmpty());
    ASSERT_FALSE(visitor.hasError());
    ASSERT_STREQ("int", program->getExpressions()[0]->getType()->getDisplayName().c_str());
}

TEST(ValidationVisitor, assignation_nonExisting) {
    VISITOR;
    const auto program = parseString("foo = 3");
    program->acceptVoidVisitor(&visitor);
    ASSERT_THAT(std::string(std::istreambuf_iterator<char>(ss), {}),
                HasSubstr("Unknown name, don't know what it refers to: foo"));
    ASSERT_TRUE(visitor.hasError());
    ASSERT_EQ(nullptr, program->getExpressions()[0]->getType());
}

TEST(ValidationVisitor, assignation_constant) {
    VISITOR;
    const auto program = parseString("val foo = 3\nfoo = 4");
    program->acceptVoidVisitor(&visitor);
    ASSERT_THAT(std::string(std::istreambuf_iterator<char>(ss), {}), HasSubstr("Cannot modify a constant"));
    ASSERT_TRUE(visitor.hasError());
    ASSERT_EQ(nullptr, program->getExpressions()[1]->getType());
}

TEST(ValidationVisitor, assignation_differentType) {
    VISITOR;
    const auto program = parseString("var foo = 3\nfoo = false");
    program->acceptVoidVisitor(&visitor);
    ASSERT_THAT(std::string(std::istreambuf_iterator<char>(ss), {}),
                HasSubstr("Cannot assign value of type bool to a variable of type int aka i32"));
    ASSERT_TRUE(visitor.hasError());
    ASSERT_EQ(nullptr, program->getExpressions()[1]->getType());
}

TEST(ValidationVisitor, assignation_valid) {
    VISITOR;
    const auto program = parseString("var foo = 3\nfoo = 2");
    program->acceptVoidVisitor(&visitor);
    ASSERT_THAT(std::string(std::istreambuf_iterator<char>(ss), {}), IsEmpty());
    ASSERT_FALSE(visitor.hasError());
    ASSERT_STREQ("int", program->getExpressions()[1]->getType()->getDisplayName().c_str());
}
