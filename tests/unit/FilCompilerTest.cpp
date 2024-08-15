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
#include <filc/filc.h>
#include <gtest/gtest.h>

TEST(FilCompiler, run) {
    auto compiler =
        filc::FilCompiler(filc::OptionsParser(), filc::ParserProxy());

    SCOPED_TRACE("No argument");
    ASSERT_EQ(0, compiler.run(1, toStringArray({"filc"}).data()));

    SCOPED_TRACE("--help");
    ASSERT_EQ(0, compiler.run(2, toStringArray({"filc", "--help"}).data()));

    SCOPED_TRACE("--version");
    ASSERT_EQ(0, compiler.run(2, toStringArray({"filc", "--version"}).data()));
}