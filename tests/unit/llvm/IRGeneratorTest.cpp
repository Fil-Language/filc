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
#include "filc/grammar/assignation/Assignation.h"
#include "filc/validation/ValidationVisitor.h"
#include "test_tools.h"

#include <filc/grammar/program/Program.h>
#include <filc/llvm/IRGenerator.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace ::testing;

auto getIR(const std::string &content) -> std::string {
    const auto program = parseString(content);
    std::stringstream ss;
    filc::ValidationVisitor validation_visitor(ss);
    program->acceptVoidVisitor(&validation_visitor);
    filc::IRGenerator generator("main", validation_visitor.getEnvironment());
    program->acceptIRVisitor(&generator);
    return generator.dump();
}

TEST(IRGenerator, program_empty) {
    const auto ir = getIR("");
    ASSERT_THAT(
        ir,
        HasSubstr("define i32 @main() {\n"
                  "entry:\n"
                  "  ret i32 0\n"
                  "}")
    );
}

TEST(IRGenerator, program_nonEmpty) {
    const auto ir = getIR("1");
    ASSERT_THAT(
        ir,
        HasSubstr("define i32 @main() {\n"
                  "entry:\n"
                  "  ret i32 1\n"
                  "}")
    );
}

TEST(IRGenerator, booleanLiteral_true) {
    const auto ir = getIR("true");
    ASSERT_THAT(ir, HasSubstr("ret i1 true"));
}

TEST(IRGenerator, booleanLiteral_false) {
    const auto ir = getIR("false");
    ASSERT_THAT(ir, HasSubstr("ret i1 false"));
}

TEST(IRGenerator, integerLiteral) {
    const auto ir = getIR("0");
    ASSERT_THAT(ir, HasSubstr("ret i32 0"));
}

TEST(IRGenerator, floatLiteral_notThrow) {
    const auto ir = getIR("3.5\n0");
    ASSERT_THAT(ir, HasSubstr("ret i32 0"));
}

TEST(IRGenerator, characterLiteral_notThrow) {
    const auto ir = getIR("'a'\n0");
    ASSERT_THAT(ir, HasSubstr("ret i32 0"));
}

TEST(IRGenerator, stringLiteral_notThrow) {
    const auto ir = getIR("\"hello\"\n0");
    ASSERT_THAT(ir, HasSubstr("ret i32 0"));
}

TEST(IRGenerator, variableDeclaration_value) {
    const auto ir = getIR("val bar = 3");
    ASSERT_THAT(ir, HasSubstr("ret i32 3"));
}

TEST(IRGenerator, variableDeclaration_identifier) {
    const auto ir = getIR("val foo = 2\nfoo");
    ASSERT_THAT(ir, HasSubstr("ret i32 2")); // Returns the value directly
}

TEST(IRGenerator, calcul_integer) {
    ASSERT_THAT(getIR("1 + 1"), HasSubstr("ret i32 2"));
    ASSERT_THAT(getIR("1 - 1"), HasSubstr("ret i32 0"));
    ASSERT_THAT(getIR("2 * 3"), HasSubstr("ret i32 6"));
    ASSERT_THAT(getIR("6 / 2"), HasSubstr("ret i32 3"));
    ASSERT_THAT(getIR("3 % 2"), HasSubstr("ret i32 1"));
}

TEST(IRGenerator, assignation_notThrow) {
    const auto ir = getIR("var bar = 3\nbar = 0");
    ASSERT_THAT(ir, HasSubstr("ret i32 0"));
}

TEST(IRGenerator, pointer_notThrow) {
    const auto ir = getIR("val foo = new i32(3);foo;0");
    ASSERT_THAT(ir, HasSubstr("alloca i32"));
    ASSERT_THAT(ir, HasSubstr("store i32 3, ptr %"));
}

TEST(IRGenerator, pointerDereferencing_notThrow) {
    const auto ir = getIR("val foo = new i32(0);*foo");
    ASSERT_THAT(ir, HasSubstr("ret i32 %1")); // Register %1 contains pointed value
}
