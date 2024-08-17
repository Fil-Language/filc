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
#include "filc/grammar/DumpVisitor.h"
#include "filc/grammar/expression/Expression.h"
#include "filc/grammar/literal/Literal.h"
#include "filc/grammar/program/Program.h"
#include "filc/grammar/variable/Variable.h"

using namespace filc;

DumpVisitor::DumpVisitor(std::ostream &out) : _out(out), _indent_level(0) {}

auto DumpVisitor::visitProgram(Program *program) -> void {
    _out << "=== Begin AST dump ===\n";
    for (const auto &expression : program->getExpressions()) {
        expression->accept(this);
    }
    _out << "=== End AST dump ===\n";
}

auto DumpVisitor::visitBooleanLiteral(BooleanLiteral *literal) -> void {
    printIdent();
    _out << "[Boolean:" << (literal->getValue() ? "true" : "false") << "]\n";
}

auto DumpVisitor::visitIntegerLiteral(IntegerLiteral *literal) -> void {
    printIdent();
    _out << "[Integer:" << literal->getValue() << "]\n";
}

auto DumpVisitor::visitFloatLiteral(FloatLiteral *literal) -> void {
    printIdent();
    _out << "[Float:" << literal->getValue() << "]\n";
}

auto DumpVisitor::visitCharacterLiteral(CharacterLiteral *literal) -> void {
    auto value = literal->getValue();
    std::string to_print;
    switch (value) {
    case '\'':
        to_print = "\\'";
        break;
    case '\"':
        to_print = "\\\"";
        break;
    case '\?':
        to_print = "\\?";
        break;
    case '\a':
        to_print = "\\a";
        break;
    case '\b':
        to_print = "\\b";
        break;
    case '\f':
        to_print = "\\f";
        break;
    case '\n':
        to_print = "\\n";
        break;
    case '\r':
        to_print = "\\r";
        break;
    case '\t':
        to_print = "\\t";
        break;
    case '\v':
        to_print = "\\v";
        break;
    case '\\':
        to_print = "\\\\";
        break;
    default:
        to_print = value;
    }

    printIdent();
    _out << "[Character:'" << to_print << "']\n";
}

auto DumpVisitor::visitStringLiteral(StringLiteral *literal) -> void {
    printIdent();
    _out << "[String:\"" << literal->getValue() << "\"]\n";
}

auto DumpVisitor::visitVariableDeclaration(VariableDeclaration *variable) -> void {
    printIdent();
    _out << "[Variable:" << (variable->isConstant() ? "val" : "var") << ":" << variable->getName();
    if (!variable->getTypeName().empty()) {
        _out << ":" << variable->getTypeName();
    }
    _out << "]\n";

    if (variable->getValue() != nullptr) {
        _indent_level++;
        variable->getValue()->accept(this);
        _indent_level--;
    }
}

auto DumpVisitor::printIdent() -> void {
    _out << std::string(_indent_level, '\t');
}
