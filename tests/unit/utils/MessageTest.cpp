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
#include <filc/utils/Message.h>
#include <gtest/gtest.h>

#define FILENAME FIXTURES_PATH "/ipsum.txt"

TEST(Message, write) {
    filc::Message message(WARNING, "This is a warning message",
                          filc::Position(new TokenStub(FILENAME, {1, 7}), new TokenStub(FILENAME, {1, 7})),
                          WARNING_COLOR);
    std::stringstream ss;
    ss << message;
    std::string dump(std::istreambuf_iterator<char>(ss), {});
    ASSERT_STREQ("\x1B[1m\x1B[33m[WARNING] \x1B[0mThis is a warning message\n"
                 "  --> " FILENAME ":1:7\n"
                 " 1 | Lorem ipsum dolor sit amet, consectetur adipiscing elit. Ut pharetra volutpat fermentum.\n"
                 "   |        \x1B[33m^\x1B[0m\n",
                 dump.c_str());
}
