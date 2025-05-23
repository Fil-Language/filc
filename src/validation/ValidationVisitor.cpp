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

#include "filc/grammar/array/Array.h"
#include "filc/grammar/assignation/Assignation.h"
#include "filc/grammar/calcul/Calcul.h"
#include "filc/grammar/identifier/Identifier.h"
#include "filc/grammar/literal/Literal.h"
#include "filc/grammar/pointer/Pointer.h"
#include "filc/grammar/program/Program.h"
#include "filc/grammar/variable/Variable.h"
#include "filc/utils/Message.h"
#include "filc/validation/CalculValidator.h"

#include <llvm/IR/DerivedTypes.h>

using namespace filc;

ValidationVisitor::ValidationVisitor(std::ostream &out)
    : _context(new VisitorContext()), _environment(new Environment()), _type_builder(_environment.get()), _out(out),
      _error(false) {}

auto ValidationVisitor::getEnvironment() const -> const Environment * {
    return _environment.get();
}

auto ValidationVisitor::hasError() const -> bool {
    return _error;
}

auto ValidationVisitor::displayError(const std::string &message, const Position &position) -> void {
    _error = true;
    _out << Message(ERROR, message, position, ERROR_COLOR);
}

auto ValidationVisitor::displayWarning(const std::string &message, const Position &position) const -> void {
    _out << Message(WARNING, message, position, WARNING_COLOR);
}

auto ValidationVisitor::visitProgram(Program *program) -> void {
    auto expressions = program->getExpressions();
    for (auto it = expressions.begin(); it != expressions.end(); ++it) {
        if (it + 1 == expressions.end()) {
            _context->set("return", true);
        }

        (*it)->acceptVoidVisitor(this);

        if (it + 1 == expressions.end()) {
            const auto expected = _environment->getType("int");
            const std::vector<std::string> allowed_types
                = {"i8", "i16", "i32", "i64", "i128", "u8", "u16", "u32", "u64", "u128", "bool"};
            const auto found_type = (*it)->getType();
            if (found_type == nullptr) {
                return;
            }

            if (std::find(allowed_types.begin(), allowed_types.end(), found_type->getName()) == allowed_types.end()) {
                displayError(
                    "Expected type " + expected->toDisplay() + " but got " + found_type->toDisplay(),
                    (*it)->getPosition()
                );
            }
        }

        _context->clear();
    }
}

auto ValidationVisitor::visitBooleanLiteral(BooleanLiteral *literal) -> void {
    literal->setType(_environment->getType("bool"));

    if (! _context->has("return") || ! _context->get<bool>("return")) {
        displayWarning("Boolean value not used", literal->getPosition());
    }
}

auto ValidationVisitor::visitIntegerLiteral(IntegerLiteral *literal) -> void {
    if (_context->has("cast_type")) {
        const auto cast_type                   = _context->get<std::shared_ptr<AbstractType>>("cast_type");
        std::vector<std::string> allowed_casts = {"i8", "i16", "i32", "i64", "i128", "u8", "u16", "u32", "u64", "u128"};
        if (std::find(allowed_casts.begin(), allowed_casts.end(), cast_type->getName()) != allowed_casts.end()) {
            literal->setType(cast_type);
        } else {
            literal->setType(_environment->getType("int"));
        }
    } else {
        literal->setType(_environment->getType("int"));
    }

    if (! _context->has("return") || ! _context->get<bool>("return")) {
        displayWarning("Integer value not used", literal->getPosition());
    }
}

auto ValidationVisitor::visitFloatLiteral(FloatLiteral *literal) -> void {
    if (_context->has("cast_type")) {
        const auto cast_type                   = _context->get<std::shared_ptr<AbstractType>>("cast_type");
        std::vector<std::string> allowed_casts = {"f32", "f64"};
        if (std::find(allowed_casts.begin(), allowed_casts.end(), cast_type->getName()) != allowed_casts.end()) {
            literal->setType(cast_type);
        } else {
            literal->setType(_environment->getType("f64"));
        }
    } else {
        literal->setType(_environment->getType("f64"));
    }

    if (! _context->has("return") || ! _context->get<bool>("return")) {
        displayWarning("Float value not used", literal->getPosition());
    }
}

auto ValidationVisitor::visitCharacterLiteral(CharacterLiteral *literal) -> void {
    literal->setType(_environment->getType("char"));

    if (! _context->has("return") || ! _context->get<bool>("return")) {
        displayWarning("Character value not used", literal->getPosition());
    }
}

auto ValidationVisitor::visitStringLiteral(StringLiteral *literal) -> void {
    literal->setType(_environment->getType("char*"));

    if (! _context->has("return") || ! _context->get<bool>("return")) {
        displayWarning("String value not used", literal->getPosition());
    }
}

auto ValidationVisitor::visitVariableDeclaration(VariableDeclaration *variable) -> void {
    if (_environment->hasName(variable->getName())) {
        displayError(variable->getName() + " is already defined", variable->getPosition());
        return;
    }

    if (variable->isConstant() && variable->getValue() == nullptr) {
        displayError("When declaring a constant, you must provide it a value", variable->getPosition());
        return;
    }

    std::shared_ptr<AbstractType> variable_type = nullptr;
    if (! variable->getTypeName().empty()) {
        if (! _environment->hasType(variable->getTypeName()) && ! _type_builder.tryBuildType(variable->getTypeName())) {
            displayError("Unknown type: " + variable->getTypeName(), variable->getPosition());
            return;
        }
        variable_type = _environment->getType(variable->getTypeName());
    }

    if (variable->getValue() != nullptr) {
        _context->stack();
        _context->set("return", true);
        if (variable_type != nullptr) {
            _context->set("cast_type", variable_type);
        }
        variable->getValue()->acceptVoidVisitor(this);
        _context->unstack();
        const auto value_type = variable->getValue()->getType();
        if (value_type == nullptr) {
            return;
        }
        if (variable_type != nullptr && variable_type->getName() != value_type->getName()) {
            displayError(
                "Cannot assign value of type " + value_type->toDisplay() + " to a variable of type "
                    + variable_type->toDisplay(),
                variable->getPosition()
            );
            return;
        } else if (variable_type == nullptr) {
            variable_type = value_type;
        }
    }

    if (variable_type == nullptr) {
        displayError("When declaring a variable, you must provide at least a type or a value", variable->getPosition());
        return;
    }

    variable->setType(variable_type);
    _environment->addName(
        Name(variable->isConstant(), variable->getName(), variable_type, variable->getValue() != nullptr)
    );
}

auto ValidationVisitor::visitIdentifier(Identifier *identifier) -> void {
    if (! _environment->hasName(identifier->getName())) {
        displayError("Unknown name, don't know what it refers to: " + identifier->getName(), identifier->getPosition());
        return;
    }

    const auto name = _environment->getName(identifier->getName());
    if (! name.hasValue()) {
        displayError(
            "Variable " + identifier->getName() + " has no value, please set one before accessing it",
            identifier->getPosition()
        );
        return;
    }
    identifier->setType(name.getType());

    if (! _context->has("return") || ! _context->get<bool>("return")) {
        displayWarning("Value not used", identifier->getPosition());
    }
}

auto ValidationVisitor::visitBinaryCalcul(BinaryCalcul *calcul) -> void {
    _context->stack();
    _context->set("return", true);
    calcul->getLeftExpression()->acceptVoidVisitor(this);
    const auto left_type = calcul->getLeftExpression()->getType();
    _context->unstack();

    _context->stack();
    _context->set("return", true);
    calcul->getRightExpression()->acceptVoidVisitor(this);
    const auto right_type = calcul->getRightExpression()->getType();
    _context->unstack();

    if (left_type == nullptr || right_type == nullptr) {
        return;
    }

    const CalculValidator validator(_environment.get());
    const auto found_type = validator.isCalculValid(left_type, calcul->getOperator(), right_type);
    if (found_type == nullptr) {
        displayError(
            "You cannot use operator " + calcul->getOperator() + " with " + left_type->toDisplay() + " and "
                + right_type->toDisplay(),
            calcul->getPosition()
        );
        return;
    }

    calcul->setType(found_type);

    if (! _context->has("return") || ! _context->get<bool>("return")) {
        displayWarning("Value not used", calcul->getPosition());
    }
}

auto ValidationVisitor::visitAssignation(Assignation *assignation) -> void {
    if (! _environment->hasName(assignation->getIdentifier())) {
        displayError(
            "Unknown name, don't know what it refers to: " + assignation->getIdentifier(), assignation->getPosition()
        );
        return;
    }
    auto name = _environment->getName(assignation->getIdentifier());
    if (name.isConstant()) {
        displayError("Cannot modify a constant", assignation->getPosition());
        return;
    }

    _context->stack();
    _context->set("return", true);
    _context->set("cast_type", name.getType());
    assignation->getValue()->acceptVoidVisitor(this);
    _context->unstack();
    const auto value_type = assignation->getValue()->getType();
    if (value_type == nullptr) {
        return;
    }
    if (value_type->getName() != name.getType()->getName()) {
        displayError(
            "Cannot assign value of type " + value_type->toDisplay() + " to a variable of type "
                + name.getType()->toDisplay(),
            assignation->getPosition()
        );
        return;
    }

    name.hasValue(true);
    _environment->setName(name);
    assignation->setType(name.getType());
}

auto ValidationVisitor::visitPointer(Pointer *pointer) -> void {
    if (! _environment->hasType(pointer->getTypeName())) {
        displayError("Unknown type: " + pointer->getTypeName(), pointer->getPosition());
        return;
    }
    const auto pointed_type = _environment->getType(pointer->getTypeName());

    std::shared_ptr<AbstractType> pointer_type = nullptr;
    if (_environment->hasType(pointer->getTypeName() + "*")) {
        pointer_type = _environment->getType(pointer->getTypeName() + "*");
    } else {
        pointer_type = std::make_shared<PointerType>(pointed_type);
        _environment->addType(pointer_type);
    }

    _context->stack();
    _context->set("return", true);
    pointer->getValue()->acceptVoidVisitor(this);
    _context->unstack();

    const auto value_type = pointer->getValue()->getType();
    if (value_type->getName() != pointed_type->getName()) {
        displayError(
            "Cannot assign a value of type " + value_type->toDisplay() + " to a pointer to type "
                + pointed_type->toDisplay(),
            pointer->getPosition()
        );
        return;
    }

    pointer->setType(pointer_type);

    if (! _context->has("return") || ! _context->get<bool>("return")) {
        displayWarning("Value not used", pointer->getPosition());
    }
}

auto ValidationVisitor::visitPointerDereferencing(PointerDereferencing *pointer) -> void {
    _context->stack();
    _context->set("return", true);
    pointer->getPointer()->acceptVoidVisitor(this);
    _context->unstack();

    const auto pointer_type = pointer->getPointer()->getType();
    if (pointer_type == nullptr) {
        return;
    }
    const auto type = std::dynamic_pointer_cast<PointerType>(pointer_type);
    if (type == nullptr) {
        displayError("Cannot dereference a variable which is not a pointer", pointer->getPosition());
        return;
    }

    pointer->setType(type->getPointedType());

    if (! _context->has("return") || ! _context->get<bool>("return")) {
        displayWarning("Value not used", pointer->getPosition());
    }
}

auto ValidationVisitor::visitVariableAddress(VariableAddress *address) -> void {
    _context->stack();
    _context->set("return", true);
    address->getVariable()->acceptVoidVisitor(this);
    _context->unstack();

    const auto pointed_type = address->getVariable()->getType();
    if (pointed_type == nullptr) {
        return;
    }
    std::shared_ptr<AbstractType> type = nullptr;
    if (_environment->hasType(pointed_type->getName() + "*")) {
        type = _environment->getType(pointed_type->getName() + "*");
    } else {
        type = std::make_shared<PointerType>(pointed_type);
        _environment->addType(type);
    }

    address->setType(type);

    if (! _context->has("return") || ! _context->get<bool>("return")) {
        displayWarning("Value not used", address->getPosition());
    }
}

auto ValidationVisitor::visitArray(Array *array) -> void {
    if (array->getSize() == 0) {
        array->setFullSize(0);
        if (_context->has("cast_type")) {
            array->setType(_context->get<std::shared_ptr<AbstractType>>("cast_type"));
        } else {
            if (! _environment->hasType("void[0]")) {
                _environment->addType(std::make_shared<ArrayType>(0, _environment->getType("void")));
            }
            array->setType(_environment->getType("void[0]"));
        }
    } else {
        if (_context->has("cast_type")) {
            const auto cast_type  = _context->get<std::shared_ptr<AbstractType>>("cast_type");
            const auto array_type = std::dynamic_pointer_cast<ArrayType>(cast_type);
            if (array_type == nullptr) {
                displayError(
                    "Cannot cast an array to a type not corresponding to an array: " + cast_type->toDisplay(),
                    array->getPosition()
                );
                return;
            }

            _context->stack();
            _context->set("cast_type", array_type->getContainedType());
            _context->set("return", true);
        }

        std::vector<std::shared_ptr<AbstractType>> values_types;
        unsigned long full_size = 0;
        for (const auto &value : array->getValues()) {
            value->acceptVoidVisitor(this);

            const auto value_type = value->getType();
            if (value_type == nullptr) {
                return;
            }
            values_types.push_back(value_type);

            if (_context->has("array_size")) {
                full_size += _context->get<unsigned long>("array_size");
                _context->unset("array_size");
            } else {
                full_size++;
            }
        }
        array->setFullSize(full_size);

        if (_context->has("cast_type")) {
            _context->unstack();
        }

        const auto it = std::adjacent_find(values_types.begin(), values_types.end(), std::not_equal_to<>());
        if (it != values_types.end()) {
            displayError("All values of an array should be of the same type", array->getPosition());
            return;
        }

        const auto type_name = values_types[0]->getDisplayName() + "[" + std::to_string(array->getSize()) + "]";
        if (! _environment->hasType(type_name)) {
            _environment->addType(std::make_shared<ArrayType>(array->getSize(), values_types[0]));
        }
        array->setType(_environment->getType(type_name));
    }

    _context->set("array_size", array->getFullSize());

    if (! _context->has("return") || ! _context->get<bool>("return")) {
        displayWarning("Value not used", array->getPosition());
    }
}

auto ValidationVisitor::visitArrayAccess(ArrayAccess *array_access) -> void {
    const auto array = array_access->getArray();
    array->acceptVoidVisitor(this);

    const auto array_type = array->getType();
    if (array_type == nullptr) {
        return;
    }
    const auto type = std::dynamic_pointer_cast<ArrayType>(array_type);
    if (type == nullptr) {
        displayError(
            "Cannot access to offset on a variable of type " + array_type->toDisplay(), array_access->getPosition()
        );
        return;
    }

    if (array_access->getIndex() >= type->getSize()) {
        displayError(
            "Out of bound access to an array. Array has a size of " + std::to_string(type->getSize()),
            array_access->getPosition()
        );
        return;
    }

    array_access->setType(type->getContainedType());

    if (! _context->has("return") || ! _context->get<bool>("return")) {
        displayWarning("Value not used", array_access->getPosition());
    }
}
