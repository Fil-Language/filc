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
#include <filc/utils/utils.h>
#include <gtest/gtest.h>

TEST(Utils, parseEscapedChar) {
    SCOPED_TRACE("Normal cases");
    ASSERT_EQ('\'', filc::parseEscapedChar("\\'"));
    ASSERT_EQ('\"', filc::parseEscapedChar("\\\""));
    ASSERT_EQ('\?', filc::parseEscapedChar("\\?"));
    ASSERT_EQ('\a', filc::parseEscapedChar("\\a"));
    ASSERT_EQ('\b', filc::parseEscapedChar("\\b"));
    ASSERT_EQ('\f', filc::parseEscapedChar("\\f"));
    ASSERT_EQ('\n', filc::parseEscapedChar("\\n"));
    ASSERT_EQ('\r', filc::parseEscapedChar("\\r"));
    ASSERT_EQ('\t', filc::parseEscapedChar("\\t"));
    ASSERT_EQ('\v', filc::parseEscapedChar("\\v"));
    ASSERT_EQ('\\', filc::parseEscapedChar("\\\\"));

    SCOPED_TRACE("Edge cases");
    ASSERT_EQ('a', filc::parseEscapedChar("ab"));
    ASSERT_EQ('\\', filc::parseEscapedChar("\\q"));
}
