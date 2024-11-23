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
#include "filc/grammar/Position.h"

#include <fstream>

using namespace filc;

#define RESET "\033[0m"

Position::Position(): _start_position(0, 0), _end_position(0, 0) {}

Position::Position(const antlr4::Token *start_token, const antlr4::Token *end_token)
    : _filename(start_token->getTokenSource()->getSourceName()) {
    if (end_token->getTokenSource()->getSourceName() != _filename) {
        throw std::logic_error("start and end token are not from the same source file");
    }

    _start_position = std::make_pair(start_token->getLine(), start_token->getCharPositionInLine());
    _end_position   = std::make_pair(end_token->getLine(), end_token->getCharPositionInLine());
}

auto Position::getFilename() const -> std::string {
    return _filename;
}

auto Position::getStartPosition() const -> std::pair<unsigned int, unsigned int> {
    return _start_position;
}

auto Position::getEndPosition() const -> std::pair<unsigned int, unsigned int> {
    return _end_position;
}

auto Position::getContent() const -> std::vector<std::string> {
    if (_filename.empty() || _filename == "<unknown>") {
        return {};
    }

    std::ifstream file(_filename);
    if (! file.is_open() || ! file.good()) {
        throw std::logic_error("File '" + _filename + "' not found");
    }

    std::vector<std::string> content;

    const auto start_line = _start_position.first;
    unsigned int index    = 0;
    std::string line;
    for (; index < start_line; index++) {
        std::getline(file, line);
    }
    content.push_back(line);

    const auto end_line = _end_position.first;
    if (end_line > start_line) {
        for (; index < end_line; index++) {
            std::getline(file, line);
            content.push_back(line);
        }
    }

    file.close();

    return content;
}

auto Position::dump(const std::string &color) const -> std::string {
    const auto start_line   = _start_position.first;
    const auto start_column = _start_position.second;
    const auto end_line     = _end_position.first;
    const auto end_column   = _end_position.second;
    const auto content      = getContent();
    if (content.empty()) {
        return "";
    }

    if (content.size() == 1) { // Single line
        const auto nth    = " " + std::to_string(start_line) + " ";
        const auto spaces = start_column > 0 ? std::string(start_column, ' ') : "";
        return std::string(nth.length() - 1, ' ') + "--> " + _filename + ":" + std::to_string(start_line) + ":"
             + std::to_string(start_column) + "\n" + nth + "| " + content[0] + "\n" + std::string(nth.length(), ' ')
             + "| " + spaces + color + "^" + RESET + "\n";
    }

    if (content.size() > 1) { // Multi line
        std::vector<std::string> nths;
        const auto nth_end = " " + std::to_string(end_line) + " ";
        for (unsigned int i = start_line; i <= end_line; i++) {
            const auto line = " " + std::to_string(i) + " ";
            nths.push_back(line + std::string(nth_end.length() - line.length(), ' '));
        }
        const auto nth_spaces   = std::string(nth_end.length(), ' ') + "| ";
        const auto start_spaces = start_column > 0 ? std::string(start_column, ' ') : "";
        const auto end_spaces   = end_column > 0 ? std::string(end_column, ' ') : "";

        auto res = std::string(nth_end.length() - 1, ' ') + "--> " + _filename + ":" + std::to_string(start_line) + ":"
                 + std::to_string(start_column) + "\n" + nth_spaces + start_spaces + color + "v" + RESET + "\n";

        for (unsigned int i = 0; i < nths.size(); i++) {
            res += nths[i] + "| " + content[i] + "\n";
        }

        res += nth_spaces + end_spaces + color + "^" + RESET + "\n";

        return res;
    }

    throw std::logic_error("Position content is empty");
}
