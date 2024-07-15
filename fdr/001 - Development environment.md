# Development environment

> - Author: Kevin Traini
> - Status: Published
> - Publication date: 2024-07-15
> - License: MIT

#### Abstract

This FDR describe all choosen tools (and why them) for filc development.

#### Table of Contents

**TODO**

## 1. Development environment

To ease developer experience, we use nix-shell tool with nixpkgs to have the same tools environment for everyone. It also ease the ci development.

We also use Renovate bot to have automatic updates of nixpkgs and filc dependencies. GitHub dependabot is also configured to have some security updates.

## 2. Build system

We use Bazel build system to build whole project and manage dependencies. The preferred compiler is clang as it's a LLVM based tool which allow an easy cross platform compilation.

## Test system

Unit testing is performed with library googletest. Linting is also enabled through clang-format, clang-tidy and .editorconfig. Husky can be used to auto perform linting before commiting.

#### License

MIT License

Copyright (c) 2024-Present Kevin Traini

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.