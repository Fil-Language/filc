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
#include <filc/validation/CalculValidator.h>
#include <gtest/gtest.h>
#include <memory>

#define VALIDATOR                       \
    auto env = new filc::Environment(); \
    filc::CalculValidator validator(env)

TEST(CalculValidator, invalidDifferentType) {
    VALIDATOR;
    ASSERT_EQ(nullptr, validator.isCalculValid(env->getType("int"), "+", env->getType("f32")));
}

TEST(CalculValidator, validNumeric) {
    VALIDATOR;
    ASSERT_STREQ("i32", validator.isCalculValid(env->getType("i32"), "+", env->getType("i32"))->getName().c_str());
}

TEST(CalculValidator, validNumericComparison) {
    VALIDATOR;
    ASSERT_STREQ("bool", validator.isCalculValid(env->getType("i32"), "==", env->getType("i32"))->getName().c_str());
}

TEST(CalculValidator, validBool) {
    VALIDATOR;
    ASSERT_STREQ("bool", validator.isCalculValid(env->getType("bool"), "&&", env->getType("bool"))->getName().c_str());
}

TEST(CalculValidator, validPointer) {
    VALIDATOR;
    ASSERT_STREQ(
        "bool",
        validator
            .isCalculValid(
                std::make_shared<filc::PointerType>(env->getType("i32")),
                "==",
                std::make_shared<filc::PointerType>(env->getType("i32"))
            )
            ->getName()
            .c_str()
    );
}

TEST(CalculValidator, invalidUnknown) {
    VALIDATOR;
    ASSERT_EQ(
        nullptr, validator.isCalculValid(std::make_shared<filc::Type>("foo"), "+", std::make_shared<filc::Type>("foo"))
    );
}
