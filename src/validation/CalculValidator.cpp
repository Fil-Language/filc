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
#include "filc/validation/CalculValidator.h"
#include <algorithm>
#include <vector>

using namespace filc;

CalculValidator::CalculValidator(Environment *environment) : _environment(environment) {}

auto CalculValidator::isCalculValid(const std::shared_ptr<AbstractType> &left_type, const std::string &op,
                                    const std::shared_ptr<AbstractType> &right_type) const
    -> std::shared_ptr<AbstractType> {
    if (left_type != right_type) {
        return nullptr;
    }
    const auto type = left_type->getName();

    const std::vector<std::string> numeric_type = {
        "i8", "i16", "i32", "i64", "i128", "u8", "u16", "u32", "u64", "u128", "f32", "f64",
    };
    if (std::find(numeric_type.begin(), numeric_type.end(), type) != numeric_type.end()) {
        return isNumericOperatorValid(left_type, op);
    }

    if (type == "bool") {
        return isBoolOperatorValid(op);
    }

    if (type[type.length() - 1] == '*') { // A pointer
        return isPointerOperatorValid(op);
    }

    // We don't know what it is, so we assert it cannot be done
    return nullptr;
}

auto CalculValidator::isNumericOperatorValid(const std::shared_ptr<AbstractType> &left_type,
                                             const std::string &op) const -> std::shared_ptr<AbstractType> {
    const std::vector<std::string> numeric_op = {"%", "+", "-", "/", "*"};
    const std::vector<std::string> boolean_op = {"<", "<=", ">", ">=", "==", "!="};

    if (std::find(numeric_op.begin(), numeric_op.end(), op) != numeric_op.end()) {
        return left_type;
    }

    if (std::find(boolean_op.begin(), boolean_op.end(), op) != boolean_op.end()) {
        return _environment->getType("bool");
    }

    return nullptr;
}

auto CalculValidator::isBoolOperatorValid(const std::string &op) const -> std::shared_ptr<AbstractType> {
    if (op == "&&" || op == "||" || op == "==" || op == "!=") {
        return _environment->getType("bool");
    }

    return nullptr;
}

auto CalculValidator::isPointerOperatorValid(const std::string &op) const -> std::shared_ptr<AbstractType> {
    if (op == "==" || op == "!=") {
        return _environment->getType("bool");
    }

    return nullptr;
}
