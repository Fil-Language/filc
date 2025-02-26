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

#include <filc/grammar/array/Array.h>
#include <filc/grammar/expression/Expression.h>
#include <filc/validation/ValidationVisitor.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>

using namespace ::testing;

#define VISITOR           \
    std::stringstream ss; \
    filc::ValidationVisitor visitor(ss)

TEST(ValidationVisitor, program_valid) {
    VISITOR;
    const auto program = parseString("0");
    program->acceptVoidVisitor(&visitor);
    ASSERT_THAT(std::string(std::istreambuf_iterator(ss), {}), IsEmpty());
    ASSERT_FALSE(visitor.hasError());
}

TEST(ValidationVisitor, program_invalid) {
    VISITOR;
    const auto program = parseString("3.4");
    program->acceptVoidVisitor(&visitor);
    ASSERT_THAT(std::string(std::istreambuf_iterator(ss), {}), HasSubstr("Expected type int aka i32 but got f64"));
    ASSERT_TRUE(visitor.hasError());
}

TEST(ValidationVisitor, program_finish_u8) {
    VISITOR;
    const auto program = parseString("val foo: u8 = 2\nfoo");
    program->acceptVoidVisitor(&visitor);
    ASSERT_THAT(std::string(std::istreambuf_iterator(ss), {}), IsEmpty());
    ASSERT_FALSE(visitor.hasError());
}

TEST(ValidationVisitor, program_finish_bool) {
    VISITOR;
    const auto program = parseString("true");
    program->acceptVoidVisitor(&visitor);
    ASSERT_THAT(std::string(std::istreambuf_iterator(ss), {}), IsEmpty());
    ASSERT_FALSE(visitor.hasError());
}

TEST(ValidationVisitor, boolean) {
    VISITOR;
    const auto program = parseString("true\n0");
    program->acceptVoidVisitor(&visitor);
    ASSERT_THAT(std::string(std::istreambuf_iterator(ss), {}), HasSubstr("Boolean value not used"));
    ASSERT_FALSE(visitor.hasError());
}

TEST(ValidationVisitor, integer) {
    VISITOR;
    const auto program = parseString("2\n0");
    program->acceptVoidVisitor(&visitor);
    ASSERT_THAT(std::string(std::istreambuf_iterator(ss), {}), HasSubstr("Integer value not used"));
    ASSERT_FALSE(visitor.hasError());
    ASSERT_STREQ("int", program->getExpressions()[0]->getType()->getDisplayName().c_str());
}

TEST(ValidationVisitor, float) {
    VISITOR;
    const auto program = parseString("3.14\n0");
    program->acceptVoidVisitor(&visitor);
    ASSERT_THAT(std::string(std::istreambuf_iterator(ss), {}), HasSubstr("Float value not used"));
    ASSERT_FALSE(visitor.hasError());
    ASSERT_STREQ("f64", program->getExpressions()[0]->getType()->getDisplayName().c_str());
}

TEST(ValidationVisitor, character) {
    VISITOR;
    const auto program = parseString("'a'\n0");
    program->acceptVoidVisitor(&visitor);
    ASSERT_THAT(std::string(std::istreambuf_iterator(ss), {}), HasSubstr("Character value not used"));
    ASSERT_FALSE(visitor.hasError());
    ASSERT_STREQ("char", program->getExpressions()[0]->getType()->getDisplayName().c_str());
}

TEST(ValidationVisitor, string) {
    VISITOR;
    const auto program = parseString("\"hello\"\n0");
    program->acceptVoidVisitor(&visitor);
    ASSERT_THAT(std::string(std::istreambuf_iterator(ss), {}), HasSubstr("String value not used"));
    ASSERT_FALSE(visitor.hasError());
    ASSERT_STREQ("char*", program->getExpressions()[0]->getType()->getDisplayName().c_str());
}

TEST(ValidationVisitor, variable_alreadyDefined) {
    VISITOR;
    const auto program = parseString("val my_constant = 2\nval my_constant = 3");
    program->acceptVoidVisitor(&visitor);
    ASSERT_THAT(std::string(std::istreambuf_iterator(ss), {}), HasSubstr("my_constant is already defined"));
    ASSERT_TRUE(visitor.hasError());
    ASSERT_STREQ("int", program->getExpressions()[0]->getType()->getDisplayName().c_str());
    ASSERT_EQ(nullptr, program->getExpressions()[1]->getType());
}

TEST(ValidationVisitor, variable_constantWithoutValue) {
    VISITOR;
    const auto program = parseString("val my_constant");
    program->acceptVoidVisitor(&visitor);
    ASSERT_THAT(
        std::string(std::istreambuf_iterator(ss), {}),
        HasSubstr("When declaring a constant, you must provide it a value")
    );
    ASSERT_TRUE(visitor.hasError());
    ASSERT_EQ(nullptr, program->getExpressions()[0]->getType());
}

TEST(ValidationVisitor, variable_unknowType) {
    VISITOR;
    const auto program = parseString("var my_var: foo");
    program->acceptVoidVisitor(&visitor);
    ASSERT_THAT(std::string(std::istreambuf_iterator(ss), {}), HasSubstr("Unknown type: foo"));
    ASSERT_TRUE(visitor.hasError());
    ASSERT_EQ(nullptr, program->getExpressions()[0]->getType());
}

TEST(ValidationVisitor, variable_differentValueType) {
    VISITOR;
    const auto program = parseString("var my_var: i32 = 'a'");
    program->acceptVoidVisitor(&visitor);
    ASSERT_THAT(
        std::string(std::istreambuf_iterator(ss), {}),
        HasSubstr("Cannot assign value of type char aka u8 to a variable of type i32")
    );
    ASSERT_TRUE(visitor.hasError());
    ASSERT_EQ(nullptr, program->getExpressions()[0]->getType());
}

TEST(ValidationVisitor, variable_assignAliasType) {
    VISITOR;
    const auto program = parseString("var my_var: u8 = 'a'\n0");
    program->acceptVoidVisitor(&visitor);
    ASSERT_THAT(std::string(std::istreambuf_iterator(ss), {}), IsEmpty());
    ASSERT_FALSE(visitor.hasError());
    ASSERT_STREQ("u8", program->getExpressions()[0]->getType()->getDisplayName().c_str());
}

TEST(ValidationVisitor, variable_withoutTypeAndValue) {
    VISITOR;
    const auto program = parseString("var my_var");
    program->acceptVoidVisitor(&visitor);
    ASSERT_THAT(
        std::string(std::istreambuf_iterator(ss), {}),
        HasSubstr("When declaring a variable, you must provide at least a type or a value")
    );
    ASSERT_TRUE(visitor.hasError());
    ASSERT_EQ(nullptr, program->getExpressions()[0]->getType());
}

TEST(ValidationVisitor, variable_valid) {
    VISITOR;
    const auto program = parseString("val foo: i32 = 45");
    program->acceptVoidVisitor(&visitor);
    ASSERT_THAT(std::string(std::istreambuf_iterator(ss), {}), IsEmpty());
    ASSERT_FALSE(visitor.hasError());
    ASSERT_STREQ("i32", program->getExpressions()[0]->getType()->getDisplayName().c_str());
}

TEST(ValidationVisitor, variable_castInteger) {
    VISITOR;
    const auto program = parseString("val bar: u8 = 4\n0");
    program->acceptVoidVisitor(&visitor);
    ASSERT_THAT(std::string(std::istreambuf_iterator(ss), {}), IsEmpty());
    ASSERT_FALSE(visitor.hasError());
    ASSERT_STREQ("u8", program->getExpressions()[0]->getType()->getDisplayName().c_str());
}

TEST(ValidationVisitor, identifier_nonExisting) {
    VISITOR;
    const auto program = parseString("bar");
    program->acceptVoidVisitor(&visitor);
    ASSERT_THAT(
        std::string(std::istreambuf_iterator(ss), {}), HasSubstr("Unknown name, don't know what it refers to: bar")
    );
    ASSERT_TRUE(visitor.hasError());
    ASSERT_EQ(nullptr, program->getExpressions()[0]->getType());
}

TEST(ValidationVisitor, identifier_noValue) {
    VISITOR;
    const auto program = parseString("var foo : i32\nfoo");
    program->acceptVoidVisitor(&visitor);
    ASSERT_THAT(
        std::string(std::istreambuf_iterator(ss), {}),
        HasSubstr("Variable foo has no value, please set one before accessing it")
    );
    ASSERT_TRUE(visitor.hasError());
    ASSERT_EQ(nullptr, program->getExpressions()[1]->getType());
}

TEST(ValidationVisitor, identifier_validAssignation) {
    VISITOR;
    const auto program = parseString("var foo : i32\nfoo = 2\nfoo");
    program->acceptVoidVisitor(&visitor);
    ASSERT_THAT(std::string(std::istreambuf_iterator(ss), {}), IsEmpty());
    ASSERT_FALSE(visitor.hasError());
    ASSERT_STREQ("i32", program->getExpressions()[2]->getType()->getDisplayName().c_str());
}

TEST(ValidationVisitor, identifier_valid) {
    VISITOR;
    const auto program = parseString("val foo = 1\nfoo");
    program->acceptVoidVisitor(&visitor);
    ASSERT_THAT(std::string(std::istreambuf_iterator(ss), {}), IsEmpty());
    ASSERT_FALSE(visitor.hasError());
    ASSERT_STREQ("int", program->getExpressions()[1]->getType()->getDisplayName().c_str());
}

TEST(ValidationVisitor, calcul_invalidLeft) {
    VISITOR;
    const auto program = parseString("(val foo) + 2");
    program->acceptVoidVisitor(&visitor);
    ASSERT_THAT(
        std::string(std::istreambuf_iterator(ss), {}),
        HasSubstr("When declaring a constant, you must provide it a value")
    );
    ASSERT_TRUE(visitor.hasError());
    ASSERT_EQ(nullptr, program->getExpressions()[0]->getType());
}

TEST(ValidationVisitor, calcul_invalidRight) {
    VISITOR;
    const auto program = parseString("2 + (val foo)");
    program->acceptVoidVisitor(&visitor);
    ASSERT_THAT(
        std::string(std::istreambuf_iterator(ss), {}),
        HasSubstr("When declaring a constant, you must provide it a value")
    );
    ASSERT_TRUE(visitor.hasError());
    ASSERT_EQ(nullptr, program->getExpressions()[0]->getType());
}

TEST(ValidationVisitor, calcul_invalid) {
    VISITOR;
    const auto program = parseString("'a' && 3");
    program->acceptVoidVisitor(&visitor);
    ASSERT_THAT(
        std::string(std::istreambuf_iterator(ss), {}),
        HasSubstr("You cannot use operator && with char aka u8 and int aka i32")
    );
    ASSERT_TRUE(visitor.hasError());
    ASSERT_EQ(nullptr, program->getExpressions()[0]->getType());
}

TEST(ValidationVisitor, calcul_valid) {
    VISITOR;
    const auto program = parseString("2 + 2");
    program->acceptVoidVisitor(&visitor);
    ASSERT_THAT(std::string(std::istreambuf_iterator(ss), {}), IsEmpty());
    ASSERT_FALSE(visitor.hasError());
    ASSERT_STREQ("int", program->getExpressions()[0]->getType()->getDisplayName().c_str());
}

TEST(ValidationVisitor, assignation_nonExisting) {
    VISITOR;
    const auto program = parseString("foo = 3");
    program->acceptVoidVisitor(&visitor);
    ASSERT_THAT(
        std::string(std::istreambuf_iterator(ss), {}), HasSubstr("Unknown name, don't know what it refers to: foo")
    );
    ASSERT_TRUE(visitor.hasError());
    ASSERT_EQ(nullptr, program->getExpressions()[0]->getType());
}

TEST(ValidationVisitor, assignation_constant) {
    VISITOR;
    const auto program = parseString("val foo = 3\nfoo = 4");
    program->acceptVoidVisitor(&visitor);
    ASSERT_THAT(std::string(std::istreambuf_iterator(ss), {}), HasSubstr("Cannot modify a constant"));
    ASSERT_TRUE(visitor.hasError());
    ASSERT_EQ(nullptr, program->getExpressions()[1]->getType());
}

TEST(ValidationVisitor, assignation_differentType) {
    VISITOR;
    const auto program = parseString("var foo = 3\nfoo = false");
    program->acceptVoidVisitor(&visitor);
    ASSERT_THAT(
        std::string(std::istreambuf_iterator(ss), {}),
        HasSubstr("Cannot assign value of type bool to a variable of type int aka i32")
    );
    ASSERT_TRUE(visitor.hasError());
    ASSERT_EQ(nullptr, program->getExpressions()[1]->getType());
}

TEST(ValidationVisitor, assignation_valid) {
    VISITOR;
    const auto program = parseString("var foo = 3\nfoo = 2");
    program->acceptVoidVisitor(&visitor);
    ASSERT_THAT(std::string(std::istreambuf_iterator(ss), {}), IsEmpty());
    ASSERT_FALSE(visitor.hasError());
    ASSERT_STREQ("int", program->getExpressions()[1]->getType()->getDisplayName().c_str());
}

TEST(ValidationVisitor, assignation_castInteger) {
    VISITOR;
    const auto program = parseString("var foo: u8 = 3\nfoo = 2\n0");
    program->acceptVoidVisitor(&visitor);
    ASSERT_THAT(std::string(std::istreambuf_iterator(ss), {}), IsEmpty());
    ASSERT_FALSE(visitor.hasError());
    ASSERT_STREQ("u8", program->getExpressions()[1]->getType()->getDisplayName().c_str());
}

TEST(ValidationVisitor, pointer_unknownType) {
    VISITOR;
    const auto program = parseString("new bla(2)");
    program->acceptVoidVisitor(&visitor);
    ASSERT_THAT(std::string(std::istreambuf_iterator(ss), {}), HasSubstr("Unknown type: bla"));
    ASSERT_TRUE(visitor.hasError());
    ASSERT_EQ(nullptr, program->getExpressions()[0]->getType());
}

TEST(ValidationVisitor, pointer_invalidValueType) {
    VISITOR;
    const auto program = parseString("new i32(true)");
    program->acceptVoidVisitor(&visitor);
    ASSERT_THAT(
        std::string(std::istreambuf_iterator(ss), {}),
        HasSubstr("Cannot assign a value of type bool to a pointer to type i32")
    );
    ASSERT_TRUE(visitor.hasError());
    ASSERT_EQ(nullptr, program->getExpressions()[0]->getType());
}

TEST(ValidationVisitor, pointer_notUsed) {
    VISITOR;
    const auto program = parseString("new i32(3);0");
    program->acceptVoidVisitor(&visitor);
    ASSERT_THAT(std::string(std::istreambuf_iterator(ss), {}), HasSubstr("Value not used"));
    ASSERT_FALSE(visitor.hasError());
    ASSERT_STREQ("i32*", program->getExpressions()[0]->getType()->getName().c_str());
}

TEST(ValidationVisitor, pointer_valid) {
    VISITOR;
    const auto program = parseString("val foo = new i32(3);0");
    program->acceptVoidVisitor(&visitor);
    ASSERT_THAT(std::string(std::istreambuf_iterator(ss), {}), IsEmpty());
    ASSERT_FALSE(visitor.hasError());
    ASSERT_STREQ("i32*", program->getExpressions()[0]->getType()->getName().c_str());
}

TEST(ValidationVisitor, pointerDereferencing_notAPointer) {
    VISITOR;
    const auto program = parseString("val foo = 'a';*foo");
    program->acceptVoidVisitor(&visitor);
    ASSERT_THAT(
        std::string(std::istreambuf_iterator(ss), {}), HasSubstr("Cannot dereference a variable which is not a pointer")
    );
    ASSERT_TRUE(visitor.hasError());
}

TEST(ValidationVisitor, pointerDereferencing_valid) {
    VISITOR;
    const auto program = parseString("val foo = new i32(3);*foo");
    program->acceptVoidVisitor(&visitor);
    ASSERT_THAT(std::string(std::istreambuf_iterator(ss), {}), IsEmpty());
    ASSERT_FALSE(visitor.hasError());
    ASSERT_STREQ("i32", program->getExpressions()[1]->getType()->getName().c_str());
}

TEST(ValidationVisitor, variableAddress_valid) {
    VISITOR;
    const auto program = parseString("val foo = 2;val bar = &foo;*bar");
    program->acceptVoidVisitor(&visitor);
    ASSERT_THAT(std::string(std::istreambuf_iterator(ss), {}), IsEmpty());
    ASSERT_FALSE(visitor.hasError());
    ASSERT_STREQ("i32*", program->getExpressions()[1]->getType()->getName().c_str());
    ASSERT_STREQ("i32", program->getExpressions()[2]->getType()->getName().c_str());
}

TEST(ValidationVisitor, array_differentType) {
    VISITOR;
    const auto program = parseString("[1, true];0");
    program->acceptVoidVisitor(&visitor);
    ASSERT_THAT(
        std::string(std::istreambuf_iterator(ss), {}), HasSubstr("All values of an array should be of the same type")
    );
    ASSERT_TRUE(visitor.hasError());
}

TEST(ValidationVisitor, array_valid) {
    VISITOR;
    const auto program = parseString("val foo = [1, 2, 3];0");
    program->acceptVoidVisitor(&visitor);
    ASSERT_THAT(std::string(std::istreambuf_iterator(ss), {}), IsEmpty());
    ASSERT_FALSE(visitor.hasError());
    ASSERT_STREQ("i32[3]", program->getExpressions()[0]->getType()->getName().c_str());
}

TEST(ValidationVisitor, array_validFullSize) {
    VISITOR;
    const auto program = parseString("[[1, 2], [3, 4]];0");
    program->acceptVoidVisitor(&visitor);
    ASSERT_FALSE(visitor.hasError());
    const auto array = std::dynamic_pointer_cast<filc::Array>(program->getExpressions()[0]);
    ASSERT_STREQ("i32[2][2]", array->getType()->getName().c_str());
    ASSERT_EQ(4, array->getFullSize());
}

TEST(ValidationVisitor, array_validCast) {
    VISITOR;
    const auto program = parseString("val foo: i8[3] = [1, 2, 3];0");
    program->acceptVoidVisitor(&visitor);
    ASSERT_THAT(std::string(std::istreambuf_iterator(ss), {}), IsEmpty());
    ASSERT_FALSE(visitor.hasError());
    ASSERT_STREQ("i8[3]", program->getExpressions()[0]->getType()->getName().c_str());
}

TEST(ValidationVisitor, array_invalidCast) {
    VISITOR;
    const auto program = parseString("val foo: u32 = [1, 2, 3]");
    program->acceptVoidVisitor(&visitor);
    ASSERT_THAT(
        std::string(std::istreambuf_iterator(ss), {}),
        HasSubstr("Cannot cast an array to a type not corresponding to an array: u32")
    );
    ASSERT_TRUE(visitor.hasError());
}

TEST(ValidationVisitor, array_empty) {
    VISITOR;
    const auto program = parseString("[];0");
    program->acceptVoidVisitor(&visitor);
    ASSERT_THAT(std::string(std::istreambuf_iterator(ss), {}), HasSubstr("Value not used"));
    ASSERT_FALSE(visitor.hasError());
    ASSERT_STREQ("void[0]", program->getExpressions()[0]->getType()->getName().c_str());
}

TEST(ValidationVisitor, array_emptyCast) {
    VISITOR;
    const auto program = parseString("val foo: char[0] = [];0");
    program->acceptVoidVisitor(&visitor);
    ASSERT_THAT(std::string(std::istreambuf_iterator(ss), {}), IsEmpty());
    ASSERT_FALSE(visitor.hasError());
    ASSERT_STREQ("char[0]", program->getExpressions()[0]->getType()->getDisplayName().c_str());
}

TEST(ValidationVisitor, arrayAccess_unknownName) {
    VISITOR;
    const auto program = parseString("foo[0]");
    program->acceptVoidVisitor(&visitor);
    ASSERT_THAT(
        std::string(std::istreambuf_iterator(ss), {}), HasSubstr("Unknown name, don't know what it refers to: foo")
    );
    ASSERT_TRUE(visitor.hasError());
}

TEST(ValidationVisitor, arrayAccess_notAnArray) {
    VISITOR;
    const auto program = parseString("val foo = 2;foo[0]");
    program->acceptVoidVisitor(&visitor);
    ASSERT_THAT(
        std::string(std::istreambuf_iterator(ss), {}), HasSubstr("Cannot access to offset on a variable of type int")
    );
    ASSERT_TRUE(visitor.hasError());
}

TEST(ValidationVisitor, arrayAccess_outOfBound) {
    VISITOR;
    const auto program = parseString("val foo = [25];foo[5]");
    program->acceptVoidVisitor(&visitor);
    ASSERT_THAT(
        std::string(std::istreambuf_iterator(ss), {}),
        HasSubstr("Out of bound access to an array. Array has a size of 1")
    );
    ASSERT_TRUE(visitor.hasError());
}

TEST(ValidationVisitor, arrayAccess_valid) {
    VISITOR;
    const auto program = parseString("val foo = [1];foo[0]");
    program->acceptVoidVisitor(&visitor);
    ASSERT_THAT(std::string(std::istreambuf_iterator(ss), {}), IsEmpty());
    ASSERT_FALSE(visitor.hasError());
    ASSERT_STREQ("i32", program->getExpressions()[1]->getType()->getName().c_str());
}
