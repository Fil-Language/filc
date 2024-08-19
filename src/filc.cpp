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
#include "filc/filc.h"
#include "filc/grammar/program/Program.h"
#include <filesystem>
#include <iostream>
#include <utility>

using namespace filc;

FilCompiler::FilCompiler(OptionsParser options_parser, DumpVisitor ast_dump_visitor)
    : _options_parser(std::move(options_parser)), _ast_dump_visitor(std::move(ast_dump_visitor)) {}

auto FilCompiler::run(int argc, char **argv) -> int {
    _options_parser.parse(argc, argv);
    if (_options_parser.isHelp()) {
        _options_parser.showHelp(std::cout);
        return 0;
    }
    if (_options_parser.isVersion()) {
        OptionsParser::showVersion(std::cout);
        return 0;
    }

    const auto filename = _options_parser.getFile();
    if (!std::filesystem::exists(filename) || !std::filesystem::is_regular_file(filename)) {
        std::cerr << "File " << filename << " not found";
        return 1;
    }
    const auto dump_option = _options_parser.getDump();

    const auto program = ParserProxy::parse(filename);
    if (dump_option == "ast" || dump_option == "all") {
        program->accept(&_ast_dump_visitor);
        if (dump_option == "ast") {
            return 0;
        }
    }

    return 1;
}
