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

#include <filc/grammar/calcul/Calcul.h>
#include <filc/grammar/literal/Literal.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace ::testing;

TEST(BinaryCalcul, parsingAddition) {
    const auto program     = parseString("1 + 2");
    const auto expressions = program->getExpressions();
    ASSERT_THAT(expressions, SizeIs(1));
    auto calcul = std::dynamic_pointer_cast<filc::BinaryCalcul>(expressions[0]);
    ASSERT_NE(nullptr, calcul);
    ASSERT_STREQ("+", calcul->getOperator().c_str());
    ASSERT_NE(nullptr, calcul->getLeftExpression());
    ASSERT_NE(nullptr, calcul->getRightExpression());
    auto left = std::dynamic_pointer_cast<filc::IntegerLiteral>(calcul->getLeftExpression());
    ASSERT_NE(nullptr, left);
    ASSERT_EQ(1, left->getValue());
    auto right = std::dynamic_pointer_cast<filc::IntegerLiteral>(calcul->getRightExpression());
    ASSERT_NE(nullptr, right);
    ASSERT_EQ(2, right->getValue());
}

TEST(BinaryCalcul, parsingPriorities) {
    PrinterVisitor visitor;
    {
        SCOPED_TRACE("1 + 2 * 3");
        const auto program = parseString("1 + 2 * 3");
        program->acceptVoidVisitor(&visitor);
        ASSERT_STREQ("(1 + (2 * 3))\n", visitor.getResult().c_str());
    }
    {
        SCOPED_TRACE("(1 + 2) * 3");
        const auto program = parseString("(1 + 2) * 3");
        program->acceptVoidVisitor(&visitor);
        ASSERT_STREQ("((1 + 2) * 3)\n", visitor.getResult().c_str());
    }
    {
        SCOPED_TRACE("1 * 2 + 3");
        const auto program = parseString("1 * 2 + 3");
        program->acceptVoidVisitor(&visitor);
        ASSERT_STREQ("((1 * 2) + 3)\n", visitor.getResult().c_str());
    }
    {
        SCOPED_TRACE("1 / 2 * 3");
        const auto program = parseString("1 / 2 * 3");
        program->acceptVoidVisitor(&visitor);
        ASSERT_STREQ("((1 / 2) * 3)\n", visitor.getResult().c_str());
    }
    {
        SCOPED_TRACE("1 * 2 / 3");
        const auto program = parseString("1 * 2 / 3");
        program->acceptVoidVisitor(&visitor);
        ASSERT_STREQ("((1 * 2) / 3)\n", visitor.getResult().c_str());
    }
    {
        SCOPED_TRACE("1 * 2 + 3 * 4");
        const auto program = parseString("1 * 2 + 3 * 4");
        program->acceptVoidVisitor(&visitor);
        ASSERT_STREQ("((1 * 2) + (3 * 4))\n", visitor.getResult().c_str());
    }
    {
        SCOPED_TRACE("1 + 2 * 3 + 4");
        const auto program = parseString("1 + 2 * 3 + 4");
        program->acceptVoidVisitor(&visitor);
        ASSERT_STREQ("((1 + (2 * 3)) + 4)\n", visitor.getResult().c_str());
    }
    {
        SCOPED_TRACE("false || 1 < 2 % 3 * 4 + 5");
        const auto program = parseString("false || 1 < 2 % 3 * 4 + 5");
        program->acceptVoidVisitor(&visitor);
        ASSERT_STREQ("(false || (1 < (((2 % 3) * 4) + 5)))\n", visitor.getResult().c_str());
    }
}
