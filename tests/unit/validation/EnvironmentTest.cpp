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
#include <filc/validation/Environment.h>
#include <gtest/gtest.h>

TEST(Environment, constructor) {
    filc::Environment env;
    ASSERT_TRUE(env.hasType("i8"));
    ASSERT_TRUE(env.hasType("i8"));
    ASSERT_TRUE(env.hasType("i16"));
    ASSERT_TRUE(env.hasType("i32"));
    ASSERT_TRUE(env.hasType("i64"));
    ASSERT_TRUE(env.hasType("i128"));
    ASSERT_TRUE(env.hasType("int"));
    ASSERT_TRUE(env.hasType("u8"));
    ASSERT_TRUE(env.hasType("u16"));
    ASSERT_TRUE(env.hasType("u32"));
    ASSERT_TRUE(env.hasType("u64"));
    ASSERT_TRUE(env.hasType("u128"));
    ASSERT_TRUE(env.hasType("uint"));
    ASSERT_TRUE(env.hasType("f32"));
    ASSERT_TRUE(env.hasType("f64"));
    ASSERT_TRUE(env.hasType("bool"));
    ASSERT_TRUE(env.hasType("char"));
    ASSERT_TRUE(env.hasType("char*"));
}

TEST(Environment, Type) {
    filc::Environment env;
    ASSERT_FALSE(env.hasType("custom"));
    ASSERT_THROW(env.getType("custom"), std::logic_error);
    env.addType(std::make_shared<filc::Type>("custom"));
    ASSERT_THROW(env.addType(std::make_shared<filc::Type>("custom")), std::logic_error);
    ASSERT_TRUE(env.hasType("custom"));
    ASSERT_STREQ("custom", env.getType("custom")->getName().c_str());
}

TEST(Environment, Name) {
    filc::Environment env;
    ASSERT_FALSE(env.hasName("my_name"));
    ASSERT_THROW(env.getName("my_name"), std::logic_error);
    env.addName(filc::Name(false, "my_name", env.getType("i32"), true));
    ASSERT_THROW(env.addName(filc::Name(true, "my_name", env.getType("i64"), true)), std::logic_error);
    ASSERT_TRUE(env.hasName("my_name"));
    ASSERT_FALSE(env.getName("my_name").isConstant());
    ASSERT_STREQ("my_name", env.getName("my_name").getName().c_str());
    ASSERT_STREQ("i32", env.getName("my_name").getType()->getName().c_str());
}

TEST(Environment, setName) {
    filc::Environment env;
    env.addName(filc::Name(false, "my_name", env.getType("i32"), true));
    ASSERT_TRUE(env.hasName("my_name"));
    ASSERT_FALSE(env.getName("my_name").isConstant());
    ASSERT_STREQ("my_name", env.getName("my_name").getName().c_str());
    ASSERT_STREQ("i32", env.getName("my_name").getType()->getName().c_str());
    ASSERT_TRUE(env.getName("my_name").hasValue());
    auto name = env.getName("my_name");
    name.hasValue(false);
    ASSERT_TRUE(env.getName("my_name").hasValue());
    env.setName(name);
    ASSERT_FALSE(env.getName("my_name").hasValue());
}
