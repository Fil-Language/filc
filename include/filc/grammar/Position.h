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
#ifndef FILC_POSITION_H
#define FILC_POSITION_H

#include "antlr4-runtime.h"
#include <utility>
#include <vector>

namespace filc {
class Position {
  public:
    Position();

    Position(const antlr4::Token* start_token, const antlr4::Token* end_token);

    [[nodiscard]] auto getFilename() const -> std::string;

    [[nodiscard]] auto getStartPosition() const -> std::pair<unsigned int, unsigned int>;

    [[nodiscard]] auto getEndPosition() const -> std::pair<unsigned int, unsigned int>;

    [[nodiscard]] auto getContent() const -> std::vector<std::string>;

    [[nodiscard]] auto dump(const std::string &color) const -> std::string;

  private:
    std::string _filename;
    std::pair<unsigned int, unsigned int> _start_position;
    std::pair<unsigned int, unsigned int> _end_position;
};
}

#endif // FILC_POSITION_H
