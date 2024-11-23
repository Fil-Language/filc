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
#include <filc/validation/ValidationVisitor.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace ::testing;

TEST(ValidationContext, stack_unstack) {
    filc::ValidationContext context;
    context.set("key", 3);
    ASSERT_TRUE(context.has("key"));
    context.stack();
    ASSERT_FALSE(context.has("key"));
    context.unstack();
    ASSERT_TRUE(context.has("key"));
    ASSERT_EQ(3, context.get<int>("key"));
}

TEST(ValidationContext, get_non_existing) {
    filc::ValidationContext context;
    ASSERT_FALSE(context.has("non-existing"));
    ASSERT_THROW(context.get<int>("non-existing"), std::logic_error);
}

TEST(ValidationContext, get_set_scalar) {
    filc::ValidationContext context;
    context.set("int_value", 2);
    ASSERT_TRUE(context.has("int_value"));
    ASSERT_EQ(2, context.get<int>("int_value"));
}

TEST(ValidationContext, get_set_string) {
    filc::ValidationContext context;
    context.set("string_value", std::string("Hello"));
    ASSERT_TRUE(context.has("string_value"));
    ASSERT_STREQ("Hello", context.get<std::string>("string_value").c_str());
}

typedef struct {
    int _a;
    std::string _b;
    char _c[5];
} SomeStructure;

TEST(ValidationContext, get_set_structure) {
    filc::ValidationContext context;
    SomeStructure value = {
      2, "Hello World", {'a', 'b', 'c', 'd', 'e'}
    };
    context.set("struct_value", value);
    ASSERT_TRUE(context.has("struct_value"));
    auto found = context.get<SomeStructure>("struct_value");
    ASSERT_EQ(2, found._a);
    ASSERT_STREQ("Hello World", found._b.c_str());
    ASSERT_THAT(found._c, ElementsAre('a', 'b', 'c', 'd', 'e'));
}

class SomeClass {
  public:
    SomeClass(): _a(12), _b("foo") {}

    [[nodiscard]] auto equals(const SomeClass &other) const -> bool {
        return _a == other._a && _b == other._b;
    }

  private:
    int _a;
    std::string _b;
};

TEST(ValidationContext, get_set_object) {
    filc::ValidationContext context;
    SomeClass value;
    context.set("object_value", value);
    ASSERT_TRUE(context.has("object_value"));
    ASSERT_TRUE(value.equals(context.get<SomeClass>("object_value")));
}

TEST(ValidationContext, clear) {
    filc::ValidationContext context;
    context.set("key", "value");
    ASSERT_TRUE(context.has("key"));
    context.clear();
    ASSERT_FALSE(context.has("key"));
}
