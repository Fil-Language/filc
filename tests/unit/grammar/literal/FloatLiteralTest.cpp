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
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace ::testing;

TEST(FloatLiteral, parsingFullFloat) {
    const auto program     = parseString("3.14");
    const auto expressions = program->getExpressions();
    ASSERT_THAT(expressions, SizeIs(1));
    const auto literal = std::dynamic_pointer_cast<filc::FloatLiteral>(expressions[0]);
    ASSERT_NE(nullptr, literal);
    ASSERT_EQ(3.14, literal->getValue());
}

TEST(FloatLiteral, parsingSemiFloat) {
    const auto program     = parseString(".2");
    const auto expressions = program->getExpressions();
    ASSERT_THAT(expressions, SizeIs(1));
    const auto literal = std::dynamic_pointer_cast<filc::FloatLiteral>(expressions[0]);
    ASSERT_NE(nullptr, literal);
    ASSERT_EQ(.2, literal->getValue());
}

TEST(FloatLiteral, parsingSigned) {
    const auto program     = parseString("-3.14;+0.2");
    const auto expressions = program->getExpressions();
    ASSERT_THAT(expressions, SizeIs(2));
    const auto literal1 = std::dynamic_pointer_cast<filc::FloatLiteral>(expressions[0]);
    ASSERT_NE(nullptr, literal1);
    ASSERT_EQ(-3.14, literal1->getValue());
    const auto literal2 = std::dynamic_pointer_cast<filc::FloatLiteral>(expressions[1]);
    ASSERT_NE(nullptr, literal2);
    ASSERT_EQ(.2, literal2->getValue());
}
