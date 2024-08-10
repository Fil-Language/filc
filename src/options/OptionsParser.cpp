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
#include "filc/options/OptionsParser.h"
#include <utility>

using namespace filc;

#define NOT_PARSED_MESSAGE "You should call parse() before getting results"

OptionsParser::OptionsParser()
    : _options("filc", "Fil compiler"), _parsed(false) {
    _options.add_options("Troubleshooting")(
        "help", "Show this help message and exit")("version",
                                                   "Show version and exit");
}

auto OptionsParser::parse(int argc, char **argv) -> void {
    try {
        _result = _options.parse(argc, argv);
    } catch (std::exception &error) {
        // Just ignore it, it will be considered as showing help
    }
    _parsed = true;
}

auto OptionsParser::isHelp() -> bool {
    if (!_parsed) {
        throw OptionsParserException(NOT_PARSED_MESSAGE);
    }

    return _result.count("help") > 0 || _result.arguments().empty();
}

auto OptionsParser::showHelp(std::ostream &out) -> void {
    out << _options.help() << "\n";
}

auto OptionsParser::isVersion() -> bool {
    if (!_parsed) {
        throw OptionsParserException(NOT_PARSED_MESSAGE);
    }

    return _result.count("version") > 0;
}

auto OptionsParser::showVersion(std::ostream &out) -> void {
    out << FILC_VERSION << "\n";
}

OptionsParserException::OptionsParserException(std::string message)
    : _message(std::move(message)) {}

const char *OptionsParserException::what() const noexcept {
    return _message.c_str();
}
