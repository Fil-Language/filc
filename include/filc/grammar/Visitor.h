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
#ifndef FILC_VISITOR_H
#define FILC_VISITOR_H

#include "filc/grammar/ast.h"

namespace filc {
class Visitor {
  public:
    virtual ~Visitor() = default;

    virtual auto visitProgram(Program *program) -> void = 0;

    virtual auto visitBooleanLiteral(BooleanLiteral *literal) -> void = 0;

    virtual auto visitIntegerLiteral(IntegerLiteral *literal) -> void = 0;

    virtual auto visitFloatLiteral(FloatLiteral *literal) -> void = 0;

    virtual auto visitCharacterLiteral(CharacterLiteral *literal) -> void = 0;

    virtual auto visitStringLiteral(StringLiteral *literal) -> void = 0;

    virtual auto visitVariableDeclaration(VariableDeclaration *literal) -> void = 0;

  protected:
    Visitor() = default;
};

class Visitable {
  public:
    virtual auto accept(Visitor *visitor) -> void = 0;
};
}

#endif // FILC_VISITOR_H
