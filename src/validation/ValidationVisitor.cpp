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
#include "filc/validation/ValidationVisitor.h"
#include "filc/grammar/literal/Literal.h"
#include "filc/grammar/program/Program.h"
#include "filc/utils/Message.h"
#include <stdexcept>

using namespace filc;

ValidationVisitor::ValidationVisitor(std::ostream &out) : _context(new ValidationContext()), _out(out) {}

auto ValidationVisitor::visitProgram(Program *program) -> void {
    for (const auto &expression : program->getExpressions()) {
        _context->set("return", true);
        expression->accept(this);
    }
}

auto ValidationVisitor::visitBooleanLiteral(BooleanLiteral *literal) -> void {
    if (_context->has("return") && _context->get<bool>("return")) {
        _out << Message(WARNING, "Boolean value not used", literal->getPosition(), WARNING_COLOR);
    }
}

auto ValidationVisitor::visitIntegerLiteral(IntegerLiteral *literal) -> void {
    if (_context->has("return") && _context->get<bool>("return")) {
        _out << Message(WARNING, "Integer value not used", literal->getPosition(), WARNING_COLOR);
    }
}

auto ValidationVisitor::visitFloatLiteral(FloatLiteral *literal) -> void {
    if (_context->has("return") && _context->get<bool>("return")) {
        _out << Message(WARNING, "Float value not used", literal->getPosition(), WARNING_COLOR);
    }
}

auto ValidationVisitor::visitCharacterLiteral(CharacterLiteral *literal) -> void {
    if (_context->has("return") && _context->get<bool>("return")) {
        _out << Message(WARNING, "Character value not used", literal->getPosition(), WARNING_COLOR);
    }
}

auto ValidationVisitor::visitStringLiteral(StringLiteral *literal) -> void {
    if (_context->has("return") && _context->get<bool>("return")) {
        _out << Message(WARNING, "String value not used", literal->getPosition(), WARNING_COLOR);
    }
}

auto ValidationVisitor::visitVariableDeclaration(VariableDeclaration *variable) -> void {
    throw std::logic_error("Not implemented yet");
}

auto ValidationVisitor::visitIdentifier(Identifier *identifier) -> void {
    throw std::logic_error("Not implemented yet");
}

auto ValidationVisitor::visitBinaryCalcul(BinaryCalcul *calcul) -> void {
    throw std::logic_error("Not implemented yet");
}

auto ValidationVisitor::visitAssignation(Assignation *assignation) -> void {
    throw std::logic_error("Not implemented yet");
}
