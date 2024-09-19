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
#include "filc/grammar/assignation/Assignation.h"
#include "filc/grammar/calcul/Calcul.h"
#include "filc/grammar/identifier/Identifier.h"
#include "filc/grammar/literal/Literal.h"
#include "filc/grammar/program/Program.h"
#include "filc/grammar/variable/Variable.h"
#include "filc/utils/Message.h"
#include "filc/validation/CalculValidator.h"
#include <stdexcept>

using namespace filc;

ValidationVisitor::ValidationVisitor(std::ostream &out)
    : _context(new ValidationContext()), _environment(new Environment()), _out(out) {}

auto ValidationVisitor::visitProgram(Program *program) -> void {
    auto expressions = program->getExpressions();
    for (auto it = expressions.begin(); it != expressions.end(); it++) {
        if (it + 1 == expressions.end()) {
            _context->set("return", true);
        }

        (*it)->accept(this);

        if (it + 1 == expressions.end()) {
            const auto expected = _environment->getType("int");
            const auto found_type = (*it)->getType();
            if (found_type == nullptr) {
                return;
            }

            if (found_type != expected) {
                auto found_type_name = found_type->getDisplayName() != found_type->getName()
                                           ? found_type->getDisplayName() + " aka " + found_type->getName()
                                           : found_type->getDisplayName();
                _out << Message(ERROR,
                                "Expected type " + expected->getDisplayName() + " aka " + expected->getName() +
                                    " but got " + found_type_name,
                                (*it)->getPosition(), ERROR_COLOR);
            }
        }

        _context->clear();
    }
}

auto ValidationVisitor::visitBooleanLiteral(BooleanLiteral *literal) -> void {
    literal->setType(_environment->getType("bool"));

    if (!_context->has("return") || !_context->get<bool>("return")) {
        _out << Message(WARNING, "Boolean value not used", literal->getPosition(), WARNING_COLOR);
    }
}

auto ValidationVisitor::visitIntegerLiteral(IntegerLiteral *literal) -> void {
    literal->setType(_environment->getType("int"));

    if (!_context->has("return") || !_context->get<bool>("return")) {
        _out << Message(WARNING, "Integer value not used", literal->getPosition(), WARNING_COLOR);
    }
}

auto ValidationVisitor::visitFloatLiteral(FloatLiteral *literal) -> void {
    literal->setType(_environment->getType("f64"));

    if (!_context->has("return") || !_context->get<bool>("return")) {
        _out << Message(WARNING, "Float value not used", literal->getPosition(), WARNING_COLOR);
    }
}

auto ValidationVisitor::visitCharacterLiteral(CharacterLiteral *literal) -> void {
    literal->setType(_environment->getType("char"));

    if (!_context->has("return") || !_context->get<bool>("return")) {
        _out << Message(WARNING, "Character value not used", literal->getPosition(), WARNING_COLOR);
    }
}

auto ValidationVisitor::visitStringLiteral(StringLiteral *literal) -> void {
    literal->setType(_environment->getType("char*"));

    if (!_context->has("return") || !_context->get<bool>("return")) {
        _out << Message(WARNING, "String value not used", literal->getPosition(), WARNING_COLOR);
    }
}

auto ValidationVisitor::visitVariableDeclaration(VariableDeclaration *variable) -> void {
    if (_environment->hasName(variable->getName())) {
        _out << Message(ERROR, variable->getName() + " is already defined", variable->getPosition(), ERROR_COLOR);
        return;
    }

    if (variable->isConstant() && variable->getValue() == nullptr) {
        _out << Message(ERROR, "When declaring a constant, you must provide it a value", variable->getPosition(),
                        ERROR_COLOR);
        return;
    }

    std::shared_ptr<AbstractType> variable_type = nullptr;
    if (!variable->getTypeName().empty()) {
        if (!_environment->hasType(variable->getTypeName())) {
            _out << Message(ERROR, "Unknown type: " + variable->getTypeName(), variable->getPosition(), ERROR_COLOR);
            return;
        }
        variable_type = _environment->getType(variable->getTypeName());
        if (variable_type == nullptr) {
            return;
        }
    }

    if (variable->getValue() != nullptr) {
        _context->stack();
        _context->set("return", true);
        variable->getValue()->accept(this);
        _context->unstack();
        const auto value_type = variable->getValue()->getType();
        if (value_type == nullptr) {
            throw std::logic_error("Variable value has no type");
        }
        if (variable_type != nullptr && variable_type->getName() != value_type->getName()) {
            const auto variable_type_dump = variable_type->getDisplayName() != variable_type->getName()
                                                ? variable_type->getDisplayName() + " aka " + variable_type->getName()
                                                : variable_type->getDisplayName();
            const auto value_type_dump = value_type->getDisplayName() != value_type->getName()
                                             ? value_type->getDisplayName() + " aka " + value_type->getName()
                                             : value_type->getDisplayName();
            _out << Message(ERROR,
                            "Cannot assign value of type " + value_type_dump + " to a variable of type " +
                                variable_type_dump,
                            variable->getPosition(), ERROR_COLOR);
            return;
        } else if (variable_type == nullptr) {
            variable_type = value_type;
        }
    }

    if (variable_type == nullptr) {
        _out << Message(ERROR, "When declaring a variable, you must provide at least a type or a value",
                        variable->getPosition(), ERROR_COLOR);
        return;
    }

    variable->setType(variable_type);
    _environment->addName(Name(variable->isConstant(), variable->getName(), variable_type));
}

auto ValidationVisitor::visitIdentifier(Identifier *identifier) -> void {
    if (!_environment->hasName(identifier->getName())) {
        _out << Message(ERROR, "Unknown name, don't know what it refers to: " + identifier->getName(),
                        identifier->getPosition(), ERROR_COLOR);
        return;
    }

    const auto name = _environment->getName(identifier->getName());
    identifier->setType(name.getType());

    if (!_context->has("return") || !_context->get<bool>("return")) {
        _out << Message(WARNING, "Value not used", identifier->getPosition(), WARNING_COLOR);
    }
}

auto ValidationVisitor::visitBinaryCalcul(BinaryCalcul *calcul) -> void {
    _context->stack();
    _context->set("return", true);
    calcul->getLeftExpression()->accept(this);
    const auto left_type = calcul->getLeftExpression()->getType();
    _context->unstack();

    _context->stack();
    _context->set("return", true);
    calcul->getRightExpression()->accept(this);
    const auto right_type = calcul->getRightExpression()->getType();
    _context->unstack();

    if (left_type == nullptr || right_type == nullptr) {
        return;
    }

    if (!CalculValidator::isCalculValid(left_type, calcul->getOperator(), right_type)) {
        _out << Message(ERROR,
                        "You cannot use operator " + calcul->getOperator() + " with " + left_type->getDisplayName() +
                            " and " + right_type->getDisplayName(),
                        calcul->getPosition(), ERROR_COLOR);
        return;
    }

    calcul->setType(left_type);

    if (!_context->has("return") || !_context->get<bool>("return")) {
        _out << Message(WARNING, "Value not used", calcul->getPosition(), WARNING_COLOR);
    }
}

auto ValidationVisitor::visitAssignation(Assignation *assignation) -> void {
    if (!_environment->hasName(assignation->getIdentifier())) {
        _out << Message(ERROR, "Unknown name, don't know what it refers to: " + assignation->getIdentifier(),
                        assignation->getPosition(), ERROR_COLOR);
        return;
    }
    const auto name = _environment->getName(assignation->getIdentifier());
    if (name.isConstant()) {
        _out << Message(ERROR, "Cannot modify a constant", assignation->getPosition(), ERROR_COLOR);
        return;
    }

    _context->stack();
    _context->set("return", true);
    assignation->getValue()->accept(this);
    _context->unstack();
    const auto value_type = assignation->getValue()->getType();
    if (value_type == nullptr) {
        return;
    }
    if (value_type->getName() != name.getType()->getName()) {
        const auto variable_type_dump = name.getType()->getDisplayName() != name.getType()->getName()
                                            ? name.getType()->getDisplayName() + " aka " + name.getType()->getName()
                                            : name.getType()->getDisplayName();
        const auto value_type_dump = value_type->getDisplayName() != value_type->getName()
                                         ? value_type->getDisplayName() + " aka " + value_type->getName()
                                         : value_type->getDisplayName();
        _out << Message(
            ERROR, "Cannot assign value of type " + value_type_dump + " to a variable of type " + variable_type_dump,
            assignation->getPosition(), ERROR_COLOR);
        return;
    }

    assignation->setType(name.getType());
}
