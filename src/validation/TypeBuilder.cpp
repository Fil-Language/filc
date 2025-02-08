/**
 * MIT License
 *
 * Copyright (c) 2025-Present Kevin Traini
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
#include "filc/validation/TypeBuilder.h"

#include "filc/grammar/Type.h"

#include <regex>

using namespace filc;

TypeBuilder::TypeBuilder(Environment *environment)
    : _environment(environment), _pointer_type_regex("^(.*)\\*$"), _array_type_regex("^(.*)\\[(\\d+)]$") {}

auto TypeBuilder::tryBuildType(const std::string &type) const noexcept -> bool {
    if (tryBuildPointerType(type)) {
        return true;
    }

    if (tryBuildArrayType(type)) {
        return true;
    }

    return false;
}

auto TypeBuilder::tryBuildPointerType(const std::string &type) const noexcept -> bool {
    std::smatch result;
    if (std::regex_match(type, result, _pointer_type_regex)) {
        const auto sub_type = result[1].str();
        if (! _environment->hasType(sub_type) && ! tryBuildType(sub_type)) {
            return false;
        }

        _environment->addType(std::make_shared<PointerType>(_environment->getType(sub_type)));
        return true;
    }

    return false;
}

auto TypeBuilder::tryBuildArrayType(const std::string &type) const noexcept -> bool {
    std::smatch result;
    if (std::regex_match(type, result, _array_type_regex)) {
        const auto sub_type = result[1].str();
        if (! _environment->hasType(sub_type) && ! tryBuildType(sub_type)) {
            return false;
        }

        const auto size = std::stoi(result[2].str());
        _environment->addType(std::make_shared<ArrayType>(size, _environment->getType(sub_type)));
        return true;
    }

    return false;
}
