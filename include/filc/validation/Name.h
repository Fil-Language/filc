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
#ifndef FILC_NAME_H
#define FILC_NAME_H

#include "filc/grammar/Type.h"
#include <string>
#include <memory>

namespace filc {
class Name {
  public:
    Name();

    Name(bool constant, std::string name, std::shared_ptr<AbstractType> type, bool has_value);

    [[nodiscard]] auto isConstant() const -> bool;

    [[nodiscard]] auto hasValue() const -> bool;

    [[nodiscard]] auto getName() const -> const std::string&;

    [[nodiscard]] auto getType() const -> std::shared_ptr<AbstractType>;

    auto hasValue(bool has_value) -> void;

  private:
    bool _constant;
    bool _has_value;
    std::string _name;
    std::shared_ptr<AbstractType> _type;
};
}

#endif // FILC_NAME_H
