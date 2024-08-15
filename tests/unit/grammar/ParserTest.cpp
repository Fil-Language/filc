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
#include <filc/grammar/literal/Literal.h>
#include <filc/grammar/program/Program.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace ::testing;

TEST(Parser, parseSample) {
    const auto program = filc::ParserProxy::parse(FIXTURES_PATH "/sample.fil");
    ASSERT_THAT(program->getExpressions(), SizeIs(3));

    SCOPED_TRACE("Expression 1");
    {
        const auto expression = std::dynamic_pointer_cast<filc::BooleanLiteral>(
            program->getExpressions()[0]);
        ASSERT_NE(nullptr, expression);
        ASSERT_TRUE(expression->getValue());
    }

    SCOPED_TRACE("Expression 2");
    {
        const auto expression = std::dynamic_pointer_cast<filc::FloatLiteral>(
            program->getExpressions()[1]);
        ASSERT_NE(nullptr, expression);
        ASSERT_EQ(6.82, expression->getValue());
    }

    SCOPED_TRACE("Expression 3");
    {
        const auto expression = std::dynamic_pointer_cast<filc::StringLiteral>(
            program->getExpressions()[2]);
        ASSERT_NE(nullptr, expression);
        ASSERT_STREQ("hEllO", expression->getValue().c_str());
    }
}
