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
#include "filc/llvm/CalculBuilder.h"

#include "filc/grammar/calcul/Calcul.h"

using namespace filc;

CalculBuilder::CalculBuilder(IRGenerator *generator, llvm::IRBuilder<> *builder)
    : _generator(generator), _builder(builder) {}

auto CalculBuilder::buildCalculValue(const BinaryCalcul *calcul) const -> llvm::Value * {
    const auto left_type_name = calcul->getLeftExpression()->getType()->getName();
    const auto operation      = calcul->getOperator();

    const std::vector<std::string> signed_integers = {"i8", "i16", "i32", "i64", "i128"};
    if (std::find(signed_integers.begin(), signed_integers.end(), left_type_name) != signed_integers.end()) {
        return buildSignedInteger(calcul);
    }

    const std::vector<std::string> unsigned_integers = {"u8", "u16", "u32", "u64", "u128"};
    if (std::find(unsigned_integers.begin(), unsigned_integers.end(), left_type_name) != unsigned_integers.end()) {
        return buildUnsignedInteger(calcul);
    }

    if (left_type_name == "f32" || left_type_name == "f64") {
        return buildFloat(calcul);
    }

    if (left_type_name == "bool") {
        return buildBool(calcul);
    }

    if (left_type_name[left_type_name.length() - 1] == '*') {
        return buildPointer(calcul);
    }

    throw buildError(calcul);
}

auto CalculBuilder::buildSignedInteger(const BinaryCalcul *calcul) const -> llvm::Value * {
    const auto operation = calcul->getOperator();
    if (operation == "%") {
        return _builder->CreateSRem(
            calcul->getLeftExpression()->acceptIRVisitor(_generator),
            calcul->getRightExpression()->acceptIRVisitor(_generator),
            "int_mod"
        );
    }
    if (operation == "+") {
        return _builder->CreateAdd(
            calcul->getLeftExpression()->acceptIRVisitor(_generator),
            calcul->getRightExpression()->acceptIRVisitor(_generator),
            "int_add"
        );
    }
    if (operation == "-") {
        return _builder->CreateSub(
            calcul->getLeftExpression()->acceptIRVisitor(_generator),
            calcul->getRightExpression()->acceptIRVisitor(_generator),
            "int_sub"
        );
    }
    if (operation == "/") {
        return _builder->CreateSDiv(
            calcul->getLeftExpression()->acceptIRVisitor(_generator),
            calcul->getRightExpression()->acceptIRVisitor(_generator),
            "int_div"
        );
    }
    if (operation == "*") {
        return _builder->CreateMul(
            calcul->getLeftExpression()->acceptIRVisitor(_generator),
            calcul->getRightExpression()->acceptIRVisitor(_generator),
            "int_mul"
        );
    }
    if (operation == "<") {
        return _builder->CreateICmpSLT(
            calcul->getLeftExpression()->acceptIRVisitor(_generator),
            calcul->getRightExpression()->acceptIRVisitor(_generator),
            "int_lt"
        );
    }
    if (operation == "<=") {
        return _builder->CreateICmpSLE(
            calcul->getLeftExpression()->acceptIRVisitor(_generator),
            calcul->getRightExpression()->acceptIRVisitor(_generator),
            "int_le"
        );
    }
    if (operation == ">") {
        return _builder->CreateICmpSGT(
            calcul->getLeftExpression()->acceptIRVisitor(_generator),
            calcul->getRightExpression()->acceptIRVisitor(_generator),
            "int_gt"
        );
    }
    if (operation == ">=") {
        return _builder->CreateICmpSGE(
            calcul->getLeftExpression()->acceptIRVisitor(_generator),
            calcul->getRightExpression()->acceptIRVisitor(_generator),
            "int_ge"
        );
    }
    if (operation == "==") {
        return _builder->CreateICmpEQ(
            calcul->getLeftExpression()->acceptIRVisitor(_generator),
            calcul->getRightExpression()->acceptIRVisitor(_generator),
            "int_equality"
        );
    }
    if (operation == "!=") {
        return _builder->CreateICmpNE(
            calcul->getLeftExpression()->acceptIRVisitor(_generator),
            calcul->getRightExpression()->acceptIRVisitor(_generator),
            "int_inequality"
        );
    }
    throw buildError(calcul);
}

auto CalculBuilder::buildUnsignedInteger(const BinaryCalcul *calcul) const -> llvm::Value * {
    const auto operation = calcul->getOperator();
    if (operation == "%") {
        return _builder->CreateURem(
            calcul->getLeftExpression()->acceptIRVisitor(_generator),
            calcul->getRightExpression()->acceptIRVisitor(_generator),
            "int_mod"
        );
    }
    if (operation == "+") {
        return _builder->CreateAdd(
            calcul->getLeftExpression()->acceptIRVisitor(_generator),
            calcul->getRightExpression()->acceptIRVisitor(_generator),
            "int_add"
        );
    }
    if (operation == "-") {
        return _builder->CreateSub(
            calcul->getLeftExpression()->acceptIRVisitor(_generator),
            calcul->getRightExpression()->acceptIRVisitor(_generator),
            "int_sub"
        );
    }
    if (operation == "/") {
        return _builder->CreateUDiv(
            calcul->getLeftExpression()->acceptIRVisitor(_generator),
            calcul->getRightExpression()->acceptIRVisitor(_generator),
            "int_div"
        );
    }
    if (operation == "*") {
        return _builder->CreateMul(
            calcul->getLeftExpression()->acceptIRVisitor(_generator),
            calcul->getRightExpression()->acceptIRVisitor(_generator),
            "int_mul"
        );
    }
    if (operation == "<") {
        return _builder->CreateICmpULT(
            calcul->getLeftExpression()->acceptIRVisitor(_generator),
            calcul->getRightExpression()->acceptIRVisitor(_generator),
            "int_lt"
        );
    }
    if (operation == "<=") {
        return _builder->CreateICmpULE(
            calcul->getLeftExpression()->acceptIRVisitor(_generator),
            calcul->getRightExpression()->acceptIRVisitor(_generator),
            "int_le"
        );
    }
    if (operation == ">") {
        return _builder->CreateICmpUGT(
            calcul->getLeftExpression()->acceptIRVisitor(_generator),
            calcul->getRightExpression()->acceptIRVisitor(_generator),
            "int_gt"
        );
    }
    if (operation == ">=") {
        return _builder->CreateICmpUGE(
            calcul->getLeftExpression()->acceptIRVisitor(_generator),
            calcul->getRightExpression()->acceptIRVisitor(_generator),
            "int_ge"
        );
    }
    if (operation == "==") {
        return _builder->CreateICmpEQ(
            calcul->getLeftExpression()->acceptIRVisitor(_generator),
            calcul->getRightExpression()->acceptIRVisitor(_generator),
            "int_equality"
        );
    }
    if (operation == "!=") {
        return _builder->CreateICmpNE(
            calcul->getLeftExpression()->acceptIRVisitor(_generator),
            calcul->getRightExpression()->acceptIRVisitor(_generator),
            "int_inequality"
        );
    }
    throw buildError(calcul);
}

auto CalculBuilder::buildFloat(const BinaryCalcul *calcul) const -> llvm::Value * {
    const auto operation = calcul->getOperator();
    if (operation == "%") {
        return _builder->CreateFRem(
            calcul->getLeftExpression()->acceptIRVisitor(_generator),
            calcul->getRightExpression()->acceptIRVisitor(_generator),
            "float_mod"
        );
    }
    if (operation == "+") {
        return _builder->CreateFAdd(
            calcul->getLeftExpression()->acceptIRVisitor(_generator),
            calcul->getRightExpression()->acceptIRVisitor(_generator),
            "float_add"
        );
    }
    if (operation == "-") {
        return _builder->CreateFSub(
            calcul->getLeftExpression()->acceptIRVisitor(_generator),
            calcul->getRightExpression()->acceptIRVisitor(_generator),
            "float_sub"
        );
    }
    if (operation == "/") {
        return _builder->CreateFDiv(
            calcul->getLeftExpression()->acceptIRVisitor(_generator),
            calcul->getRightExpression()->acceptIRVisitor(_generator),
            "float_div"
        );
    }
    if (operation == "*") {
        return _builder->CreateFMul(
            calcul->getLeftExpression()->acceptIRVisitor(_generator),
            calcul->getRightExpression()->acceptIRVisitor(_generator),
            "float_mul"
        );
    }
    if (operation == "<") {
        return _builder->CreateFCmpOLT(
            calcul->getLeftExpression()->acceptIRVisitor(_generator),
            calcul->getRightExpression()->acceptIRVisitor(_generator),
            "float_lt"
        );
    }
    if (operation == "<=") {
        return _builder->CreateFCmpOLE(
            calcul->getLeftExpression()->acceptIRVisitor(_generator),
            calcul->getRightExpression()->acceptIRVisitor(_generator),
            "float_le"
        );
    }
    if (operation == ">") {
        return _builder->CreateFCmpOGT(
            calcul->getLeftExpression()->acceptIRVisitor(_generator),
            calcul->getRightExpression()->acceptIRVisitor(_generator),
            "float_gt"
        );
    }
    if (operation == ">=") {
        return _builder->CreateFCmpOGE(
            calcul->getLeftExpression()->acceptIRVisitor(_generator),
            calcul->getRightExpression()->acceptIRVisitor(_generator),
            "float_ge"
        );
    }
    if (operation == "==") {
        return _builder->CreateFCmpOEQ(
            calcul->getLeftExpression()->acceptIRVisitor(_generator),
            calcul->getRightExpression()->acceptIRVisitor(_generator),
            "float_equality"
        );
    }
    if (operation == "!=") {
        return _builder->CreateFCmpONE(
            calcul->getLeftExpression()->acceptIRVisitor(_generator),
            calcul->getRightExpression()->acceptIRVisitor(_generator),
            "float_inequality"
        );
    }
    throw buildError(calcul);
}

auto CalculBuilder::buildBool(const BinaryCalcul *calcul) const -> llvm::Value * {
    const auto operation = calcul->getOperator();
    if (operation == "&&") {
        return _builder->CreateAnd(
            calcul->getLeftExpression()->acceptIRVisitor(_generator),
            calcul->getRightExpression()->acceptIRVisitor(_generator),
            "bool_and"
        );
    }
    if (operation == "||") {
        return _builder->CreateOr(
            calcul->getLeftExpression()->acceptIRVisitor(_generator),
            calcul->getRightExpression()->acceptIRVisitor(_generator),
            "bool_or"
        );
    }
    if (operation == "==") {
        return _builder->CreateICmpEQ(
            calcul->getLeftExpression()->acceptIRVisitor(_generator),
            calcul->getRightExpression()->acceptIRVisitor(_generator),
            "bool_equality"
        );
    }
    if (operation == "!=") {
        return _builder->CreateICmpNE(
            calcul->getLeftExpression()->acceptIRVisitor(_generator),
            calcul->getRightExpression()->acceptIRVisitor(_generator),
            "bool_inequality"
        );
    }
    throw buildError(calcul);
}

auto CalculBuilder::buildPointer(const BinaryCalcul *calcul) const -> llvm::Value * {
    const auto operation = calcul->getOperator();
    if (operation == "==") {
        return _builder->CreateICmpEQ(
            calcul->getLeftExpression()->acceptIRVisitor(_generator),
            calcul->getRightExpression()->acceptIRVisitor(_generator),
            "pointer_equality"
        );
    }
    if (operation == "!=") {
        return _builder->CreateICmpNE(
            calcul->getLeftExpression()->acceptIRVisitor(_generator),
            calcul->getRightExpression()->acceptIRVisitor(_generator),
            "pointer_inequality"
        );
    }
    throw buildError(calcul);
}

auto CalculBuilder::buildError(const BinaryCalcul *calcul) -> std::logic_error {
    return std::logic_error(
        "Should be caught by validation, got operation (" + calcul->getOperator() + ") with:\n" + " - "
        + calcul->getLeftExpression()->getType()->toDisplay() + "\n" + " - "
        + calcul->getRightExpression()->getType()->toDisplay()
    );
}
