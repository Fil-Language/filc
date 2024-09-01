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
#include <filc/grammar/Position.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace ::testing;

#define FILENAME FIXTURES_PATH "/ipsum.txt"

TEST(Position, defaultConstructor) {
    filc::Position position;
    ASSERT_THAT(position.getStartPosition(), Pair(0, 0));
    ASSERT_THAT(position.getEndPosition(), Pair(0, 0));
    ASSERT_STREQ("", position.getFilename().c_str());
    ASSERT_THROW(position.getContent(), std::logic_error);
    ASSERT_THROW(position.dump(""), std::logic_error);
}

TEST(Position, tokenConstructor) {
    filc::Position position(new TokenStub("source", {2, 3}), new TokenStub("source", {5, 6}));
    ASSERT_THAT(position.getStartPosition(), Pair(2, 3));
    ASSERT_THAT(position.getEndPosition(), Pair(5, 6));
    ASSERT_STREQ("source", position.getFilename().c_str());
}

TEST(Position, tokenConstructor_throw) {
    ASSERT_THROW(filc::Position position(new TokenStub("source1", {2, 3}), new TokenStub("source2", {5, 6})),
                 std::logic_error);
}

TEST(Position, getContent) {
    filc::Position position(new TokenStub(FILENAME, {2, 3}), new TokenStub(FILENAME, {4, 6}));
    const auto content = position.getContent();
    ASSERT_THAT(
        content,
        ElementsAre("Phasellus fringilla sem vel lorem scelerisque, non commodo augue consectetur. Donec",          // 2
                    "malesuada erat vitae egestas euismod. Fusce in cursus justo, non egestas ipsum. Pellentesque", // 3
                    "at dui nunc. Nam ligula augue, tempus id vestibulum nec, rutrum porttitor nibh. Nullam non"    // 4
                    ));
}

TEST(Position, dump_multiline) {
    filc::Position position(new TokenStub(FILENAME, {2, 3}), new TokenStub(FILENAME, {4, 6}));
    const auto result = position.dump("");
    ASSERT_STREQ("  --> " FILENAME ":2:3\n"
                 "   |    v\x1B[0m\n"
                 " 2 | Phasellus fringilla sem vel lorem scelerisque, non commodo augue consectetur. Donec\n"
                 " 3 | malesuada erat vitae egestas euismod. Fusce in cursus justo, non egestas ipsum. Pellentesque\n"
                 " 4 | at dui nunc. Nam ligula augue, tempus id vestibulum nec, rutrum porttitor nibh. Nullam non\n"
                 "   |       ^\x1B[0m\n",
                 result.c_str());
}

TEST(Position, dump_oneline) {
    filc::Position position(new TokenStub(FILENAME, {1, 7}), new TokenStub(FILENAME, {1, 10}));
    const auto result = position.dump("");
    ASSERT_STREQ("  --> " FILENAME ":1:7\n"
                 " 1 | Lorem ipsum dolor sit amet, consectetur adipiscing elit. Ut pharetra volutpat fermentum.\n"
                 "   |        ^\x1B[0m\n",
                 result.c_str());
}

TEST(Position, dump_color) {
    filc::Position position(new TokenStub(FILENAME, {1, 7}), new TokenStub(FILENAME, {1, 10}));
    const auto result = position.dump("\033[31m");
    ASSERT_STREQ("  --> " FILENAME ":1:7\n"
                 " 1 | Lorem ipsum dolor sit amet, consectetur adipiscing elit. Ut pharetra volutpat fermentum.\n"
                 "   |        \x1B[31m^\x1B[0m\n",
                 result.c_str());
}
