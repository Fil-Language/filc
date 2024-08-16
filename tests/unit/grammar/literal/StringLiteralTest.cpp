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

TEST(StringLiteral, parsing) {
    SCOPED_TRACE("Empty string");
    {
        const auto program = parseString("\"\"");
        const auto expressions = program->getExpressions();
        ASSERT_THAT(expressions, SizeIs(1));
        auto literal = std::dynamic_pointer_cast<filc::StringLiteral>(expressions[0]);
        ASSERT_NE(nullptr, literal);
        ASSERT_STREQ("", literal->getValue().c_str());
    }

    SCOPED_TRACE("Some string");
    {
        const auto program = parseString("\"Hello World!\"");
        const auto expressions = program->getExpressions();
        ASSERT_THAT(expressions, SizeIs(1));
        auto literal = std::dynamic_pointer_cast<filc::StringLiteral>(expressions[0]);
        ASSERT_NE(nullptr, literal);
        ASSERT_STREQ("Hello World!", literal->getValue().c_str());
    }
}
