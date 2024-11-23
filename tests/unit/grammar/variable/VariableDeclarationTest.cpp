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

#include <filc/grammar/literal/Literal.h>
#include <filc/grammar/variable/Variable.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace ::testing;

TEST(VariableDeclaration, parsingSimpleConstant) {
    const auto program     = parseString("val foo");
    const auto expressions = program->getExpressions();
    ASSERT_THAT(expressions, SizeIs(1));
    auto variable = std::dynamic_pointer_cast<filc::VariableDeclaration>(expressions[0]);
    ASSERT_NE(nullptr, variable);
    ASSERT_TRUE(variable->isConstant());
    ASSERT_STREQ("foo", variable->getName().c_str());
    ASSERT_STREQ("", variable->getTypeName().c_str());
    ASSERT_EQ(nullptr, variable->getValue());
}

TEST(VariableDeclaration, parsingSimpleVariable) {
    const auto program     = parseString("var bar");
    const auto expressions = program->getExpressions();
    ASSERT_THAT(expressions, SizeIs(1));
    auto variable = std::dynamic_pointer_cast<filc::VariableDeclaration>(expressions[0]);
    ASSERT_NE(nullptr, variable);
    ASSERT_FALSE(variable->isConstant());
    ASSERT_STREQ("bar", variable->getName().c_str());
    ASSERT_STREQ("", variable->getTypeName().c_str());
    ASSERT_EQ(nullptr, variable->getValue());
}

TEST(VariableDeclaration, parsingWithType) {
    const auto program     = parseString("var bar: i32");
    const auto expressions = program->getExpressions();
    ASSERT_THAT(expressions, SizeIs(1));
    auto variable = std::dynamic_pointer_cast<filc::VariableDeclaration>(expressions[0]);
    ASSERT_NE(nullptr, variable);
    ASSERT_FALSE(variable->isConstant());
    ASSERT_STREQ("bar", variable->getName().c_str());
    ASSERT_STREQ("i32", variable->getTypeName().c_str());
    ASSERT_EQ(nullptr, variable->getValue());
}

TEST(VariableDeclaration, parsingWithValue) {
    const auto program     = parseString("val foo = 'a'");
    const auto expressions = program->getExpressions();
    ASSERT_THAT(expressions, SizeIs(1));
    auto variable = std::dynamic_pointer_cast<filc::VariableDeclaration>(expressions[0]);
    ASSERT_NE(nullptr, variable);
    ASSERT_TRUE(variable->isConstant());
    ASSERT_STREQ("foo", variable->getName().c_str());
    ASSERT_STREQ("", variable->getTypeName().c_str());
    ASSERT_EQ('a', std::dynamic_pointer_cast<filc::CharacterLiteral>(variable->getValue())->getValue());
}

TEST(VariableDeclaration, parsingWithTypeAndValue) {
    const auto program     = parseString("val foo: f64 = 3.1415");
    const auto expressions = program->getExpressions();
    ASSERT_THAT(expressions, SizeIs(1));
    auto variable = std::dynamic_pointer_cast<filc::VariableDeclaration>(expressions[0]);
    ASSERT_NE(nullptr, variable);
    ASSERT_TRUE(variable->isConstant());
    ASSERT_STREQ("foo", variable->getName().c_str());
    ASSERT_STREQ("f64", variable->getTypeName().c_str());
    ASSERT_EQ(3.1415, std::dynamic_pointer_cast<filc::FloatLiteral>(variable->getValue())->getValue());
}
