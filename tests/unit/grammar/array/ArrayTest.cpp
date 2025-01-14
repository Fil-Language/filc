/**
 * MIT License
 *
 * Copyright (c) 2025-Present Kevin Traini
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
#include <filc/grammar/literal/Literal.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace ::testing;

TEST(Array, parsingEmpty) {
    const auto program     = parseString("[]");
    const auto expressions = program->getExpressions();
    ASSERT_THAT(expressions, SizeIs(1));
    const auto array = std::dynamic_pointer_cast<filc::Array>(expressions[0]);
    ASSERT_NE(nullptr, array);
    ASSERT_EQ(0, array->getSize());
    ASSERT_THAT(array->getValues(), IsEmpty());
}

TEST(Array, parsingSimple) {
    const auto program     = parseString("[1, 2, 3]");
    const auto expressions = program->getExpressions();
    ASSERT_THAT(expressions, SizeIs(1));
    const auto array = std::dynamic_pointer_cast<filc::Array>(expressions[0]);
    ASSERT_NE(nullptr, array);
    ASSERT_EQ(3, array->getSize());
    const auto values = array->getValues();
    for (unsigned int i = 0; i < array->getSize(); i++) {
        const auto value = std::dynamic_pointer_cast<filc::IntegerLiteral>(values[i]);
        ASSERT_NE(nullptr, value);
        ASSERT_EQ(i + 1, value->getValue());
    }
}
