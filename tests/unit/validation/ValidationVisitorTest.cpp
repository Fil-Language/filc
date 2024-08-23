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
#include <filc/grammar/assignation/Assignation.h>
#include <filc/grammar/calcul/Calcul.h>
#include <filc/grammar/identifier/Identifier.h>
#include <filc/grammar/literal/Literal.h>
#include <filc/grammar/program/Program.h>
#include <filc/grammar/variable/Variable.h>
#include <filc/validation/ValidationVisitor.h>
#include <gtest/gtest.h>
#include <memory>

TEST(ValidationVisitor, visitBooleanLiteral) {
    filc::ValidationVisitor visitor;
    filc::Program program({std::make_shared<filc::BooleanLiteral>(true)});
    ASSERT_THROW(program.accept(&visitor), std::logic_error);
}

TEST(ValidationVisitor, visitIntegerLiteral) {
    filc::ValidationVisitor visitor;
    filc::Program program({std::make_shared<filc::IntegerLiteral>(12)});
    ASSERT_THROW(program.accept(&visitor), std::logic_error);
}

TEST(ValidationVisitor, visitFloatLiteral) {
    filc::ValidationVisitor visitor;
    filc::Program program({std::make_shared<filc::FloatLiteral>(4.6)});
    ASSERT_THROW(program.accept(&visitor), std::logic_error);
}

TEST(ValidationVisitor, visitCharacterLiteral) {
    filc::ValidationVisitor visitor;
    filc::Program program({std::make_shared<filc::CharacterLiteral>('g')});
    ASSERT_THROW(program.accept(&visitor), std::logic_error);
}

TEST(ValidationVisitor, visitStringLiteral) {
    filc::ValidationVisitor visitor;
    filc::Program program({std::make_shared<filc::StringLiteral>("hello")});
    ASSERT_THROW(program.accept(&visitor), std::logic_error);
}

TEST(ValidationVisitor, visitVariableDeclaration) {
    filc::ValidationVisitor visitor;
    filc::Program program(
        {std::make_shared<filc::VariableDeclaration>(true, "name", "i32", std::make_shared<filc::IntegerLiteral>(45))});
    ASSERT_THROW(program.accept(&visitor), std::logic_error);
}

TEST(ValidationVisitor, visitIdentifier) {
    filc::ValidationVisitor visitor;
    filc::Program program({std::make_shared<filc::Identifier>("foo")});
    ASSERT_THROW(program.accept(&visitor), std::logic_error);
}

TEST(ValidationVisitor, visitBinaryCalcul) {
    filc::ValidationVisitor visitor;
    filc::Program program({std::make_shared<filc::BinaryCalcul>(std::make_shared<filc::IntegerLiteral>(2), "+",
                                                                std::make_shared<filc::IntegerLiteral>(3))});
    ASSERT_THROW(program.accept(&visitor), std::logic_error);
}

TEST(ValidationVisitor, visitAssignation) {
    filc::ValidationVisitor visitor;
    filc::Program program({std::make_shared<filc::Assignation>("id", std::make_shared<filc::IntegerLiteral>(478))});
    ASSERT_THROW(program.accept(&visitor), std::logic_error);
}
