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
#include "FilLexer.h"
#include "FilParser.h"
#include "antlr4-runtime.h"
#include <filc/validation/ValidationVisitor.h>

auto toStringArray(const std::vector<std::string> &data) -> std::vector<char *> {
    std::vector<char *> strings;
    strings.reserve(data.size());
    for (auto &item : data) {
        strings.push_back(const_cast<char *>(item.c_str()));
    }

    return strings;
}

auto parseString(const std::string &content) -> std::shared_ptr<filc::Program> {
    antlr4::ANTLRInputStream input(content);
    filc::FilLexer lexer(&input);
    antlr4::CommonTokenStream tokens(&lexer);
    tokens.fill();

    filc::FilParser parser(&tokens);

    return parser.program()->tree;
}

auto parseAndValidateString(const std::string &content) -> std::shared_ptr<filc::Program> {
    const auto program = parseString(content);
    std::stringstream ss;
    filc::ValidationVisitor validation_visitor(ss);
    program->acceptVoidVisitor(&validation_visitor);
    return program;
}

PrinterVisitor::PrinterVisitor() : _out(std::stringstream()) {}

auto PrinterVisitor::getResult() -> std::string {
    std::string result(std::istreambuf_iterator<char>(_out), {});
    return result;
}

auto PrinterVisitor::visitProgram(filc::Program *program) -> void {
    for (const auto &expression : program->getExpressions()) {
        expression->acceptVoidVisitor(this);
        _out << "\n";
    }
}

auto PrinterVisitor::visitBooleanLiteral(filc::BooleanLiteral *literal) -> void {
    _out << (literal->getValue() ? "true" : "false");
}

auto PrinterVisitor::visitIntegerLiteral(filc::IntegerLiteral *literal) -> void { _out << literal->getValue(); }

auto PrinterVisitor::visitFloatLiteral(filc::FloatLiteral *literal) -> void { _out << literal->getValue(); }

auto PrinterVisitor::visitCharacterLiteral(filc::CharacterLiteral *literal) -> void {
    _out << "'" << literal->getValue() << "'";
}

auto PrinterVisitor::visitStringLiteral(filc::StringLiteral *literal) -> void {
    _out << "\"" << literal->getValue() << "\"";
}

auto PrinterVisitor::visitVariableDeclaration(filc::VariableDeclaration *variable) -> void {
    _out << (variable->isConstant() ? "val " : "var ") << variable->getName();
    if (!variable->getTypeName().empty()) {
        _out << ": " << variable->getTypeName();
    }
    if (variable->getValue() != nullptr) {
        _out << " = ";
        variable->getValue()->acceptVoidVisitor(this);
    }
}

auto PrinterVisitor::visitIdentifier(filc::Identifier *identifier) -> void { _out << identifier->getName(); }

auto PrinterVisitor::visitBinaryCalcul(filc::BinaryCalcul *calcul) -> void {
    _out << "(";
    calcul->getLeftExpression()->acceptVoidVisitor(this);
    _out << " " << calcul->getOperator() << " ";
    calcul->getRightExpression()->acceptVoidVisitor(this);
    _out << ")";
}

auto PrinterVisitor::visitAssignation(filc::Assignation *assignation) -> void {
    _out << assignation->getIdentifier() << " = ";
    assignation->getValue()->acceptVoidVisitor(this);
}

TokenSourceStub::TokenSourceStub(std::string filename) : _filename(std::move(filename)) {}

auto TokenSourceStub::nextToken() -> std::unique_ptr<antlr4::Token> { return nullptr; }

auto TokenSourceStub::getLine() const -> size_t { return 0; }

auto TokenSourceStub::getCharPositionInLine() -> size_t { return 0; }

auto TokenSourceStub::getInputStream() -> antlr4::CharStream * { return nullptr; }

auto TokenSourceStub::getSourceName() -> std::string { return _filename; }

auto TokenSourceStub::getTokenFactory() -> antlr4::TokenFactory<antlr4::CommonToken> * { return nullptr; }

TokenStub::TokenStub(const std::string &filename, const std::pair<unsigned int, unsigned int> &position)
    : _source(new TokenSourceStub(filename)), _position(position) {}

auto TokenStub::getText() const -> std::string { return ""; }

auto TokenStub::getType() const -> size_t { return 0; }

auto TokenStub::getLine() const -> size_t { return _position.first; }

auto TokenStub::getCharPositionInLine() const -> size_t { return _position.second; }

auto TokenStub::getChannel() const -> size_t { return 0; }

auto TokenStub::getTokenIndex() const -> size_t { return 0; }

auto TokenStub::getStartIndex() const -> size_t { return 0; }

auto TokenStub::getStopIndex() const -> size_t { return 0; }

auto TokenStub::getTokenSource() const -> antlr4::TokenSource * { return _source; }

auto TokenStub::getInputStream() const -> antlr4::CharStream * { return nullptr; }

auto TokenStub::toString() const -> std::string { return ""; }
