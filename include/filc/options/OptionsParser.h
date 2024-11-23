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
#ifndef FILC_OPTIONSPARSER_H
#define FILC_OPTIONSPARSER_H

#include <cxxopts.hpp>
#include <exception>
#include <string>
#include <ostream>

namespace filc {
class OptionsParser final {
  public:
    OptionsParser();

    auto parse(int argc, char **argv) -> void;

    [[nodiscard]] auto isHelp() const -> bool;

    auto showHelp(std::ostream &out) const -> void;

    [[nodiscard]] auto isVersion() const -> bool;

    static auto showVersion(std::ostream &out) -> void;

    [[nodiscard]] auto getFile() const -> std::string;

    [[nodiscard]] auto getDump() const -> std::string;

    [[nodiscard]] auto getOutputFile() const -> std::string;

    [[nodiscard]] auto getTarget() const -> std::string;

  private:
    cxxopts::Options _options;
    bool _parsed;
    cxxopts::ParseResult _result;
};

class OptionsParserException : public std::exception {
  public:
    explicit OptionsParserException(std::string message);

    ~OptionsParserException() _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW override = default;

    [[nodiscard]] const char *what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW override;

  private:
    std::string _message;
};
}

#endif // FILC_OPTIONSPARSER_H
