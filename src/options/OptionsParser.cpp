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

OptionsParser::OptionsParser(): _options("filc", "Fil compiler"), _parsed(false) {
    auto main_options = _options.add_options();
    main_options("file", "Path of file to compile.", cxxopts::value<std::string>()->default_value(""));
    _options.parse_positional("file");
    _options.positional_help("file");

    auto general_options = _options.add_options("General");
    general_options("out,o", "Write output to file", cxxopts::value<std::string>()->default_value("a.out"), "<file>");
    general_options("target", "Generate code for the given target", cxxopts::value<std::string>(), "<value>");

    auto trouble_options = _options.add_options("Troubleshooting");
    trouble_options("help", "Show this help message and exit.");
    trouble_options("version", "Show version and exit.");

    auto debug_options = _options.add_options("Debug");
    debug_options(
        "dump",
        "Dump some data. One of these values: ast, ir.",
        cxxopts::value<std::string>()->implicit_value("all")->default_value("none")
    );
}

auto OptionsParser::parse(const int argc, char **argv) -> void {
    try {
        _result = _options.parse(argc, argv);
    } catch (std::exception &error) {
        // Just ignore it, it will be considered as showing help
    }
    _parsed = true;
}

auto OptionsParser::isHelp() const -> bool {
    if (! _parsed) {
        throw OptionsParserException(NOT_PARSED_MESSAGE);
    }

    return _result.count("help") > 0 || _result.arguments().empty();
}

auto OptionsParser::showHelp(std::ostream &out) const -> void {
    out << _options.help() << "\n";
}

auto OptionsParser::isVersion() const -> bool {
    if (! _parsed) {
        throw OptionsParserException(NOT_PARSED_MESSAGE);
    }

    return _result.count("version") > 0;
}

auto OptionsParser::getFile() const -> std::string {
    if (! _parsed) {
        throw OptionsParserException(NOT_PARSED_MESSAGE);
    }

    return _result["file"].as<std::string>();
}

auto OptionsParser::getDump() const -> std::string {
    if (! _parsed) {
        throw OptionsParserException(NOT_PARSED_MESSAGE);
    }

    auto dump        = _result["dump"].as<std::string>();
    const auto valid = {"none", "all", "ast", "ir"};
    if (std::find(valid.begin(), valid.end(), dump) == valid.end()) {
        throw OptionsParserException("Dump option value '" + dump + "' is not a valid value");
    }

    return dump;
}

auto OptionsParser::showVersion(std::ostream &out) -> void {
    out << FILC_VERSION << "\n";
}

auto OptionsParser::getOutputFile() const -> std::string {
    if (! _parsed) {
        throw OptionsParserException(NOT_PARSED_MESSAGE);
    }

    return _result["out"].as<std::string>();
}

auto OptionsParser::getTarget() const -> std::string {
    if (! _parsed) {
        throw OptionsParserException(NOT_PARSED_MESSAGE);
    }

    if (_result.count("target") == 0) {
        return "";
    }
    return _result["target"].as<std::string>();
}

OptionsParserException::OptionsParserException(std::string message): _message(std::move(message)) {}

const char *OptionsParserException::what() const noexcept {
    return _message.c_str();
}
