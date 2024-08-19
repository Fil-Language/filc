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
#ifndef FILC_TEST_TOOLS_H
#define FILC_TEST_TOOLS_H

#include <filc/grammar/program/Program.h>
#include <filc/grammar/Visitor.h>
#include <string>
#include <vector>
#include <memory>
#include <sstream>

auto toStringArray(const std::vector<std::string> &data) -> std::vector<char *>;

auto parseString(const std::string &content) -> std::shared_ptr<filc::Program>;

class PrinterVisitor final: public filc::Visitor {
  public:
    PrinterVisitor();

    auto getResult() -> std::string;

    auto visitProgram(filc::Program *program) -> void override;

    auto visitBooleanLiteral(filc::BooleanLiteral *literal) -> void override;

    auto visitIntegerLiteral(filc::IntegerLiteral *literal) -> void override;

    auto visitFloatLiteral(filc::FloatLiteral *literal) -> void override;

    auto visitCharacterLiteral(filc::CharacterLiteral *literal) -> void override;

    auto visitStringLiteral(filc::StringLiteral *literal) -> void override;

    auto visitVariableDeclaration(filc::VariableDeclaration *variable) -> void override;

    auto visitIdentifier(filc::Identifier *identifier) -> void override;

    auto visitBinaryCalcul(filc::BinaryCalcul *calcul) -> void override;

    auto visitAssignation(filc::Assignation *assignation) -> void override;

  private:
    std::stringstream _out;
};

#endif // FILC_TEST_TOOLS_H
