/**
 * MIT License
 *
 * Copyright (c) 2025-Present Kevin Traini
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

#include <filc/validation/Environment.h>
#include <filc/validation/TypeBuilder.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

TEST(TypeBuilder, false_empty) {
    const filc::TypeBuilder type_builder(new filc::Environment());

    ASSERT_FALSE(type_builder.tryBuildType(""));
}

TEST(TypeBuilder, false_notAType) {
    const filc::TypeBuilder type_builder(new filc::Environment());

    ASSERT_FALSE(type_builder.tryBuildType("foo_bar"));
}

TEST(TypeBuilder, true_simplePointer) {
    filc::Environment environment;
    const filc::TypeBuilder type_builder(&environment);

    ASSERT_FALSE(environment.hasType("int*"));
    ASSERT_TRUE(type_builder.tryBuildType("int*"));
    ASSERT_TRUE(environment.hasType("int*"));
}

TEST(TypeBuilder, true_doublePointer) {
    filc::Environment environment;
    const filc::TypeBuilder type_builder(&environment);

    ASSERT_FALSE(environment.hasType("int**"));
    ASSERT_TRUE(type_builder.tryBuildType("int**"));
    ASSERT_TRUE(environment.hasType("int**"));
}

TEST(TypeBuilder, true_simpleArray) {
    filc::Environment environment;
    const filc::TypeBuilder type_builder(&environment);

    ASSERT_FALSE(environment.hasType("int[1]"));
    ASSERT_TRUE(type_builder.tryBuildType("int[1]"));
    ASSERT_TRUE(environment.hasType("int[1]"));
}

TEST(TypeBuilder, true_doubleArray) {
    filc::Environment environment;
    const filc::TypeBuilder type_builder(&environment);

    ASSERT_FALSE(environment.hasType("int[1][2]"));
    ASSERT_TRUE(type_builder.tryBuildType("int[1][2]"));
    ASSERT_TRUE(environment.hasType("int[1][2]"));
}

TEST(TypeBuilder, true_pointerArray) {
    filc::Environment environment;
    const filc::TypeBuilder type_builder(&environment);

    ASSERT_FALSE(environment.hasType("int*[1]"));
    ASSERT_TRUE(type_builder.tryBuildType("int*[1]"));
    ASSERT_TRUE(environment.hasType("int*[1]"));
}

TEST(TypeBuilder, true_arrayPointer) {
    filc::Environment environment;
    const filc::TypeBuilder type_builder(&environment);

    ASSERT_FALSE(environment.hasType("int[1]*"));
    ASSERT_TRUE(type_builder.tryBuildType("int[1]*"));
    ASSERT_TRUE(environment.hasType("int[1]*"));
}
