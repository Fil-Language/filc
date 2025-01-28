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

#include <filc/grammar/DumpVisitor.h>
#include <filc/grammar/literal/Literal.h>
#include <fstream>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <sstream>

using namespace ::testing;

auto dumpProgram(const std::string &content) -> std::vector<std::string> {
    const auto program = parseString(content);
    std::stringstream ss;
    filc::DumpVisitor visitor(ss);
    program->acceptVoidVisitor(&visitor);
    std::string dump(std::istreambuf_iterator<char>(ss), {});

    std::vector<std::string> result;
    std::string collector;
    for (const auto c : dump) {
        if (c == '\n') {
            result.push_back(collector);
            collector = "";
        } else {
            collector += c;
        }
    }

    if (result.size() < 2) {
        throw std::logic_error("Size of dump lines is lesser than 2, it's not normal!");
    }

    return {result.begin() + 1, result.end() - 1};
}

TEST(DumpVisitor, dump) {
    const auto program = parseString("");
    std::stringstream ss;
    auto visitor = filc::DumpVisitor(ss);
    program->acceptVoidVisitor(&visitor);
    const std::string result(std::istreambuf_iterator<char>(ss), {});
    ASSERT_STREQ("=== Begin AST dump ===\n=== End AST dump ===\n", result.c_str());
}

TEST(DumpVisitor, BooleanLiteral) {
    const auto dump = dumpProgram("true\nfalse");
    ASSERT_THAT(dump, SizeIs(2));
    ASSERT_STREQ("[Boolean:true]", dump[0].c_str());
    ASSERT_STREQ("[Boolean:false]", dump[1].c_str());
}

TEST(DumpVisitor, IntegerLiteral) {
    const auto dump = dumpProgram("2");
    ASSERT_THAT(dump, SizeIs(1));
    ASSERT_STREQ("[Integer:2]", dump[0].c_str());
}

TEST(DumpVisitor, FloatLiteral) {
    const auto dump = dumpProgram("1.81\n.56");
    ASSERT_THAT(dump, SizeIs(2));
    ASSERT_STREQ("[Float:1.81]", dump[0].c_str());
    ASSERT_STREQ("[Float:0.56]", dump[1].c_str());
}

TEST(DumpVisitor, CharacterLiteral) {
    const auto dump = dumpProgram("'a'\n'\\n'");
    ASSERT_THAT(dump, SizeIs(2));
    ASSERT_STREQ("[Character:'a']", dump[0].c_str());
    ASSERT_STREQ("[Character:'\\n']", dump[1].c_str());
}

TEST(DumpVisitor, StringLiteral) {
    const auto dump = dumpProgram("\"\"\n\"Hello\"");
    ASSERT_THAT(dump, SizeIs(2));
    ASSERT_STREQ("[String:\"\"]", dump[0].c_str());
    ASSERT_STREQ("[String:\"Hello\"]", dump[1].c_str());
}

TEST(DumpVisitor, visitCharacterLiteral_Special) {
    for (const auto c : {'\'', '\"', '\?', '\a', '\b', '\f', '\n', '\r', '\t', '\v', '\\'}) {
        SCOPED_TRACE("Check for special " + std::string(1, c));
        std::stringstream ss;
        filc::DumpVisitor visitor(ss);
        filc::CharacterLiteral literal(c);
        visitor.visitCharacterLiteral(&literal);
        std::string dump(std::istreambuf_iterator<char>(ss), {});
        ASSERT_THAT(dump, StartsWith("[Character:'\\"));
    }
}

TEST(DumpVisitor, visitCharacterLiteral_Classic) {
    std::stringstream ss;
    filc::DumpVisitor visitor(ss);
    filc::CharacterLiteral literal('a');
    visitor.visitCharacterLiteral(&literal);
    const std::string dump(std::istreambuf_iterator<char>(ss), {});
    ASSERT_STREQ("[Character:'a']\n", dump.c_str());
}

TEST(DumpVisitor, VariableDeclaration_SimpleConstant) {
    const auto dump = dumpProgram("val foo");
    ASSERT_THAT(dump, SizeIs(1));
    ASSERT_STREQ("[Variable:val:foo]", dump[0].c_str());
}

TEST(DumpVisitor, VariableDeclaration_SimpleVariable) {
    const auto dump = dumpProgram("var bar");
    ASSERT_THAT(dump, SizeIs(1));
    ASSERT_STREQ("[Variable:var:bar]", dump[0].c_str());
}

TEST(DumpVisitor, VariableDeclaration_WithType) {
    const auto dump = dumpProgram("var bar: i32");
    ASSERT_THAT(dump, SizeIs(1));
    ASSERT_STREQ("[Variable:var:bar:i32]", dump[0].c_str());
}

TEST(DumpVisitor, VariableDeclaration_WithValue) {
    const auto dump = dumpProgram("val foo = 'a'");
    ASSERT_THAT(dump, SizeIs(2));
    ASSERT_STREQ("[Variable:val:foo]", dump[0].c_str());
    ASSERT_STREQ("\t[Character:'a']", dump[1].c_str());
}

TEST(DumpVisitor, VariableDeclaration_WithTypeAndValue) {
    const auto dump = dumpProgram("val foo: f64 = 3.1415");
    ASSERT_THAT(dump, SizeIs(2));
    ASSERT_STREQ("[Variable:val:foo:f64]", dump[0].c_str());
    ASSERT_STREQ("\t[Float:3.1415]", dump[1].c_str());
}

TEST(DumpVisitor, BinaryCalcul_SimpleDivision) {
    const auto dump = dumpProgram("6 / 3");
    ASSERT_THAT(dump, SizeIs(3));
    ASSERT_STREQ("[BinaryCalcul:/]", dump[0].c_str());
    ASSERT_STREQ("\t[Integer:6]", dump[1].c_str());
    ASSERT_STREQ("\t[Integer:3]", dump[2].c_str());
}

TEST(DumpVisitor, BinaryCalcul_MultiplicationOfAddition) {
    const auto dump = dumpProgram("(1 + 2) * (3 + 4)");
    ASSERT_THAT(dump, SizeIs(7));
    ASSERT_STREQ("[BinaryCalcul:*]", dump[0].c_str());
    ASSERT_STREQ("\t[BinaryCalcul:+]", dump[1].c_str());
    ASSERT_STREQ("\t\t[Integer:1]", dump[2].c_str());
    ASSERT_STREQ("\t\t[Integer:2]", dump[3].c_str());
    ASSERT_STREQ("\t[BinaryCalcul:+]", dump[4].c_str());
    ASSERT_STREQ("\t\t[Integer:3]", dump[5].c_str());
    ASSERT_STREQ("\t\t[Integer:4]", dump[6].c_str());
}

TEST(DumpVisitor, Identifier) {
    const auto dump = dumpProgram("hello");
    ASSERT_THAT(dump, SizeIs(1));
    ASSERT_STREQ("[Identifier:hello]", dump[0].c_str());
}

TEST(DumpVisitor, Assignation) {
    const auto dump = dumpProgram("foo = 'a'");
    ASSERT_THAT(dump, SizeIs(2));
    ASSERT_STREQ("[Assignation:foo]", dump[0].c_str());
    ASSERT_STREQ("\t[Character:'a']", dump[1].c_str());
}

TEST(DumpVisitor, Pointer) {
    const auto dump = dumpProgram("new i32(3)");
    ASSERT_THAT(dump, SizeIs(2));
    ASSERT_STREQ("[Pointer:i32]", dump[0].c_str());
    ASSERT_STREQ("\t[Integer:3]", dump[1].c_str());
}

TEST(DumpVisitor, PointerDereferencing) {
    const auto dump = dumpProgram("*foo");
    ASSERT_THAT(dump, SizeIs(2));
    ASSERT_STREQ("[PointerDereferencing]", dump[0].c_str());
    ASSERT_STREQ("\t[Identifier:foo]", dump[1].c_str());
}

TEST(DumpVisitor, VariableAddress) {
    const auto dump = dumpProgram("&foo");
    ASSERT_THAT(dump, SizeIs(2));
    ASSERT_STREQ("[VariableAddress]", dump[0].c_str());
    ASSERT_STREQ("\t[Identifier:foo]", dump[1].c_str());
}
