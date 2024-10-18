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
#include <sstream>

TEST(FilCompiler, run) {
    auto compiler =
        filc::FilCompiler(filc::OptionsParser(), filc::DumpVisitor(std::cout), filc::ValidationVisitor(std::cout));

    SCOPED_TRACE("No argument");
    ASSERT_EQ(0, compiler.run(1, toStringArray({"filc"}).data()));

    SCOPED_TRACE("--help");
    ASSERT_EQ(0, compiler.run(2, toStringArray({"filc", "--help"}).data()));

    SCOPED_TRACE("--version");
    ASSERT_EQ(0, compiler.run(2, toStringArray({"filc", "--version"}).data()));
}

TEST(FilCompiler, dumpAST) {
    std::stringstream ss;
    auto compiler = filc::FilCompiler(filc::OptionsParser(), filc::DumpVisitor(ss), filc::ValidationVisitor(std::cout));
    ASSERT_EQ(0, compiler.run(3, toStringArray({"filc", "--dump=ast", FIXTURES_PATH "/sample.fil"}).data()));
    std::string result(std::istreambuf_iterator<char>(ss), {});
    ASSERT_STREQ("=== Begin AST dump ===\n"
                 "[Boolean:true]\n"
                 "[Float:6.82]\n"
                 "[String:\"hEllO\"]\n"
                 "[Variable:val:some_constant_73]\n"
                 "[Variable:var:myAweSOMeVariable:i32]\n"
                 "[Variable:val:anotherConst]\n"
                 "\t[Integer:73]\n"
                 "[Variable:var:my_var:char]\n"
                 "\t[Character:'c']\n"
                 "[Identifier:_some_varWhichUses_Some_CHARACTERS]\n"
                 "[BinaryCalcul:<=]\n"
                 "\t[BinaryCalcul:+]\n"
                 "\t\t[Integer:2]\n"
                 "\t\t[Integer:4]\n"
                 "\t[BinaryCalcul:*]\n"
                 "\t\t[Integer:3]\n"
                 "\t\t[Integer:2]\n"
                 "[Assignation:my_var]\n"
                 "\t[Integer:2]\n"
                 "[Assignation:my_var]\n"
                 "\t[BinaryCalcul:+]\n"
                 "\t\t[Identifier:my_var]\n"
                 "\t\t[Integer:2]\n"
                 "=== End AST dump ===\n",
                 result.c_str());
}

TEST(FilCompiler, fullRun) {
    std::stringstream ss;
    auto compiler = filc::FilCompiler(filc::OptionsParser(), filc::DumpVisitor(ss), filc::ValidationVisitor(std::cout));
    ASSERT_EQ(0, compiler.run(2, toStringArray({"filc", FIXTURES_PATH "/valid.fil"}).data()));
}
