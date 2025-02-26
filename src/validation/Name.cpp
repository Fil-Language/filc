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
#include "filc/validation/Name.h"

#include <utility>

using namespace filc;

Name::Name(): _constant(true), _has_value(false) {}

Name::Name(const bool constant, std::string name, std::shared_ptr<AbstractType> type, const bool has_value)
    : _constant(constant), _has_value(has_value), _name(std::move(name)), _type(std::move(type)) {}

auto Name::isConstant() const -> bool {
    return _constant;
}

auto Name::hasValue() const -> bool {
    return _has_value;
}

auto Name::getName() const -> const std::string & {
    return _name;
}

auto Name::getType() const -> std::shared_ptr<AbstractType> {
    return _type;
}

auto Name::hasValue(const bool has_value) -> void {
    _has_value = has_value;
}
