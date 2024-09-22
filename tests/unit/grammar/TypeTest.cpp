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
#include <filc/grammar/Type.h>
#include <gtest/gtest.h>

TEST(Type, getName) {
    filc::Type type("my_type");
    ASSERT_STREQ("my_type", type.getName().c_str());
}

TEST(Type, getDisplayName) {
    filc::Type type("another_type");
    ASSERT_STREQ("another_type", type.getDisplayName().c_str());
}

TEST(PointerType, getName) {
    filc::PointerType type(std::make_shared<filc::Type>("int"));
    ASSERT_STREQ("int*", type.getName().c_str());
}

TEST(PointerType, getDisplayName) {
    filc::PointerType type(std::make_shared<filc::Type>("int"));
    ASSERT_STREQ("int*", type.getDisplayName().c_str());
}

TEST(AliasType, getName) {
    filc::AliasType type("char", std::make_shared<filc::Type>("u8"));
    ASSERT_STREQ("u8", type.getName().c_str());
}

TEST(AliasType, getDisplayName) {
    filc::AliasType type("char", std::make_shared<filc::Type>("u8"));
    ASSERT_STREQ("char", type.getDisplayName().c_str());
}
