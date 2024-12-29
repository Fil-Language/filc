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

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>

auto getProgramResult(const std::string &program) -> int {
    std::ofstream program_file(FIXTURES_PATH "/ir_test.fil");
    program_file << program;
    program_file.flush();
    program_file.close();

    const auto ir_output = run_with_args("--dump=ir " FIXTURES_PATH "/ir_test.fil 2>&1");
    std::ofstream ir_file(FIXTURES_PATH "/ir_test.ir");
    ir_file << ir_output;
    ir_file.flush();
    ir_file.close();

    const auto status = system(LLI_BIN " " FIXTURES_PATH "/ir_test.ir");

    std::filesystem::remove(FIXTURES_PATH "/ir_test.fil");
    std::filesystem::remove(FIXTURES_PATH "/ir_test.ir");

    return WEXITSTATUS(status);
}

TEST(ir_dump, calcul_program) {
    ASSERT_EQ(2, getProgramResult("1 + 1"));
    ASSERT_EQ(5, getProgramResult("(3 * 2 + 4) / 2"));
}

TEST(ir_dump, variable_program) {
    ASSERT_EQ(2, getProgramResult("val foo = 2\nfoo"));
}

TEST(ir_dump, pointer_program) {
    ASSERT_EQ(3, getProgramResult("val foo = new i32(3);*foo"));
}

TEST(ir_dump, address_program) {
    ASSERT_EQ(4, getProgramResult("val foo = 4;val bar = &foo;*bar"));
}
