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

#include <filc/options/OptionsParser.h>
#include <gtest/gtest.h>
#include <sstream>

TEST(OptionsParser, parse) {
    auto options_parser = filc::OptionsParser();
    ASSERT_THROW(options_parser.isHelp(), filc::OptionsParserException);
    ASSERT_THROW(options_parser.isVersion(), filc::OptionsParserException);

    SCOPED_TRACE("No arguments");
    options_parser.parse(1, toStringArray({"filc"}).data());
    ASSERT_TRUE(options_parser.isHelp());
    ASSERT_FALSE(options_parser.isVersion());

    SCOPED_TRACE("--help");
    options_parser.parse(2, toStringArray({"filc", "--help"}).data());
    ASSERT_TRUE(options_parser.isHelp());
    ASSERT_FALSE(options_parser.isVersion());

    SCOPED_TRACE("--version");
    options_parser.parse(2, toStringArray({"filc", "--version"}).data());
    ASSERT_FALSE(options_parser.isHelp());
    ASSERT_TRUE(options_parser.isVersion());
}

TEST(OptionsParser, showHelp) {
    auto options_parser = filc::OptionsParser();
    std::stringstream stream;
    options_parser.showHelp(stream);
    std::string result;
    stream >> result;
    ASSERT_TRUE(! result.empty());
}

TEST(OptionsParser, showVersion) {
    std::stringstream stream;
    filc::OptionsParser::showVersion(stream);
    std::string result;
    stream >> result;
    ASSERT_STREQ(FILC_VERSION, result.c_str());
}

TEST(OptionsParser, getFile) {
    auto options_parser = filc::OptionsParser();
    SCOPED_TRACE("No file");
    options_parser.parse(1, toStringArray({"filc"}).data());
    ASSERT_STREQ("", options_parser.getFile().c_str());

    SCOPED_TRACE("--file test.txt");
    options_parser.parse(3, toStringArray({"filc", "--file", "test.txt"}).data());
    ASSERT_STREQ("test.txt", options_parser.getFile().c_str());

    SCOPED_TRACE("--file=test2.txt");
    options_parser.parse(2, toStringArray({"filc", "--file=test2.txt"}).data());
    ASSERT_STREQ("test2.txt", options_parser.getFile().c_str());

    SCOPED_TRACE("Positional argument");
    options_parser.parse(2, toStringArray({"filc", "test3.txt"}).data());
    ASSERT_STREQ("test3.txt", options_parser.getFile().c_str());
}

TEST(OptionsParser, getDump) {
    auto options_parser = filc::OptionsParser();
    SCOPED_TRACE("Default value");
    options_parser.parse(1, toStringArray({"filc"}).data());
    ASSERT_STREQ("none", options_parser.getDump().c_str());

    SCOPED_TRACE("Implicit value");
    options_parser.parse(2, toStringArray({"filc", "--dump"}).data());
    ASSERT_STREQ("all", options_parser.getDump().c_str());

    SCOPED_TRACE("Invalid value");
    options_parser.parse(2, toStringArray({"filc", "--dump=invalid"}).data());
    ASSERT_THROW(options_parser.getDump(), filc::OptionsParserException);

    SCOPED_TRACE("--dump=ast");
    options_parser.parse(2, toStringArray({"filc", "--dump=ast"}).data());
    ASSERT_STREQ("ast", options_parser.getDump().c_str());
}
