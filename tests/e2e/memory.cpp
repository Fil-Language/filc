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
#include <gmock/gmock.h>
#include <gtest/gtest.h>

/**
 * This test file aims to check memory usage of filc
 * We should try to reduce memory leak to the minimum (all memory used must be
 * freed)
 */

#ifdef __linux__

#define VALGRIND_OUTPUT_ZERO "in use at exit: 0 bytes in 0 blocks"

#define valgrind_run(args) exec_output("valgrind " FILC_BIN " " args " 2>&1")

// FIXME: Memory can no longer be tested: LLVM doesn't free all its pointers and we do not have hand on that
// TEST(Memory, filc) {
//    const auto result = valgrind_run();
//    ASSERT_THAT(result, ::testing::HasSubstr(VALGRIND_OUTPUT_ZERO));
//}
//
// TEST(Memory, filc_help) {
//    const auto result = valgrind_run("--help");
//    ASSERT_THAT(result, ::testing::HasSubstr(VALGRIND_OUTPUT_ZERO));
//}
//
// TEST(Memory, filc_version) {
//    const auto result = valgrind_run("--version");
//    ASSERT_THAT(result, ::testing::HasSubstr(VALGRIND_OUTPUT_ZERO));
//}
//
// TEST(Memory, filc_dump_ast) {
//    const auto result = valgrind_run("--dump=ast " FIXTURES_PATH "/sample.fil");
//    ASSERT_THAT(result, ::testing::HasSubstr(VALGRIND_OUTPUT_ZERO));
//}
//
// TEST(Memory, filc_full) {
//    const auto result = valgrind_run(FIXTURES_PATH "/sample.fil");
//    ASSERT_THAT(result, ::testing::HasSubstr(VALGRIND_OUTPUT_ZERO));
//}

#endif
