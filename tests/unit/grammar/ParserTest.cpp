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
#include <filc/grammar/calcul/Calcul.h>
#include <filc/grammar/identifier/Identifier.h>
#include <filc/grammar/literal/Literal.h>
#include <filc/grammar/variable/Variable.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace ::testing;

TEST(Parser, nonExistingFile) { ASSERT_THROW(filc::ParserProxy::parse("non-existing-file"), std::logic_error); }

TEST(Parser, parseSample) {
    const auto program = filc::ParserProxy::parse(FIXTURES_PATH "/sample.fil");
    ASSERT_THAT(program->getExpressions(), SizeIs(11));

    {
        SCOPED_TRACE("true");
        const auto expression = std::dynamic_pointer_cast<filc::BooleanLiteral>(program->getExpressions()[0]);
        ASSERT_NE(nullptr, expression);
        ASSERT_TRUE(expression->getValue());
    }

    {
        SCOPED_TRACE("6.82");
        const auto expression = std::dynamic_pointer_cast<filc::FloatLiteral>(program->getExpressions()[1]);
        ASSERT_NE(nullptr, expression);
        ASSERT_EQ(6.82, expression->getValue());
    }

    {
        SCOPED_TRACE("\"hEllO\"");
        const auto expression = std::dynamic_pointer_cast<filc::StringLiteral>(program->getExpressions()[2]);
        ASSERT_NE(nullptr, expression);
        ASSERT_STREQ("hEllO", expression->getValue().c_str());
    }

    {
        SCOPED_TRACE("val some_constant_73");
        const auto expression = std::dynamic_pointer_cast<filc::VariableDeclaration>(program->getExpressions()[3]);
        ASSERT_NE(nullptr, expression);
        ASSERT_TRUE(expression->isConstant());
        ASSERT_STREQ("some_constant_73", expression->getName().c_str());
        ASSERT_STREQ("", expression->getTypeName().c_str());
        ASSERT_EQ(nullptr, expression->getValue());
    }

    {
        SCOPED_TRACE("var myAweSOMeVariable: i32");
        const auto expression = std::dynamic_pointer_cast<filc::VariableDeclaration>(program->getExpressions()[4]);
        ASSERT_NE(nullptr, expression);
        ASSERT_FALSE(expression->isConstant());
        ASSERT_STREQ("myAweSOMeVariable", expression->getName().c_str());
        ASSERT_STREQ("i32", expression->getTypeName().c_str());
        ASSERT_EQ(nullptr, expression->getValue());
    }

    {
        SCOPED_TRACE("val anotherConst = 73");
        const auto expression = std::dynamic_pointer_cast<filc::VariableDeclaration>(program->getExpressions()[5]);
        ASSERT_NE(nullptr, expression);
        ASSERT_TRUE(expression->isConstant());
        ASSERT_STREQ("anotherConst", expression->getName().c_str());
        ASSERT_STREQ("", expression->getTypeName().c_str());
        ASSERT_EQ(73, std::dynamic_pointer_cast<filc::IntegerLiteral>(expression->getValue())->getValue());
    }

    {
        SCOPED_TRACE("var my_var: char = 'c'");
        const auto expression = std::dynamic_pointer_cast<filc::VariableDeclaration>(program->getExpressions()[6]);
        ASSERT_NE(nullptr, expression);
        ASSERT_FALSE(expression->isConstant());
        ASSERT_STREQ("my_var", expression->getName().c_str());
        ASSERT_STREQ("char", expression->getTypeName().c_str());
        ASSERT_EQ('c', std::dynamic_pointer_cast<filc::CharacterLiteral>(expression->getValue())->getValue());
    }

    {
        SCOPED_TRACE("_some_varWhichUses_Some_CHARACTERS");
        const auto expression = std::dynamic_pointer_cast<filc::Identifier>(program->getExpressions()[7]);
        ASSERT_NE(nullptr, expression);
        ASSERT_STREQ("_some_varWhichUses_Some_CHARACTERS", expression->getName().c_str());
    }

    {
        SCOPED_TRACE("2 + 4 <= 3 * 2");
        const auto expression = program->getExpressions()[8];
        PrinterVisitor visitor;
        expression->accept(&visitor);
        ASSERT_STREQ("((2 + 4) <= (3 * 2))", visitor.getResult().c_str());
    }

    {
        SCOPED_TRACE("my_var = 2");
        const auto expression = program->getExpressions()[9];
        PrinterVisitor visitor;
        expression->accept(&visitor);
        ASSERT_STREQ("my_var = 2", visitor.getResult().c_str());
    }

    {
        SCOPED_TRACE("my_var += 2");
        const auto expression = program->getExpressions()[10];
        PrinterVisitor visitor;
        expression->accept(&visitor);
        ASSERT_STREQ("my_var = (my_var + 2)", visitor.getResult().c_str());
    }
}
