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
#include <filc/grammar/pointer/Pointer.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace ::testing;

TEST(Pointer, parsing) {
    const auto program     = parseString("new i32(3)");
    const auto expressions = program->getExpressions();
    ASSERT_THAT(expressions, SizeIs(1));
    const auto pointer = std::dynamic_pointer_cast<filc::Pointer>(expressions[0]);
    ASSERT_NE(nullptr, pointer);
    ASSERT_STREQ("i32", pointer->getTypeName().c_str());

    const auto value = std::dynamic_pointer_cast<filc::IntegerLiteral>(pointer->getValue());
    ASSERT_NE(nullptr, value);
    ASSERT_EQ(3, value->getValue());
}
