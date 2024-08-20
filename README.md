# filc

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
![Version](version.svg)

[![Tests](https://github.com/Fil-Language/filc/actions/workflows/tests.yml/badge.svg)](https://github.com/Fil-Language/filc/actions/workflows/tests.yml)
[![Codacy Badge](https://app.codacy.com/project/badge/Grade/6e3d7ce34c884e0abb9769896667b3e7)](https://app.codacy.com/gh/Fil-Language/filc/dashboard?utm_source=gh&utm_medium=referral&utm_content=&utm_campaign=Badge_grade)
[![Codacy Badge](https://app.codacy.com/project/badge/Coverage/6e3d7ce34c884e0abb9769896667b3e7)](https://app.codacy.com/gh/Fil-Language/filc/dashboard?utm_source=gh&utm_medium=referral&utm_content=&utm_campaign=Badge_coverage)

Fil compiler: [https://fil.ktraini.com](https://fil.ktraini.com).

[Roadmap](roadmap.md)

## Install

You can retrieve rpm/deb package attached to [latest release](https://github.com/fil-language/filc/releases/latest).

Devel packages are also build for each push to master branch in [package workflow](https://github.com/Fil-Language/filc/actions/workflows/package.yml?query=branch%3Amaster).

You can also clone the repository then [build it](#building).

## Building

All following commands should be run inside a `nix-shell`, it allows you to have all needed tools.

Entire project is based on cmake. So you can just run cmake as for every cmake project.

Or you can run a pre-defined script:

```shell
./bin/build_release.sh
```

`filc` executable is copied at the root of repository, you can then copy it in a directory of your `$PATH`.

## Testing

There is 2 tests suites:

**Unit tests**

```shell
./bin/run_unit_tests.sh
```

You can then get coverage of these tests with:

```shell
./bin/build_coverage.sh
```

**e2e tests**

They run directly filc

```shell
./bin/run_e2e_tests.sh
```

## Contributing

You want to contribute, please read the [Code of conduct](CODE_OF_CONDUCT.md) and [Contributing](CONTRIBUTING.md).

## Security

Needed information is in [Security](SECURITY.md).

## Acknowledgements

Thanks to JetBrains for helping me develop this project by offering me
an [open source license](https://jb.gg/OpenSourceSupport)

![JetBrains Logo (Main) logo](https://resources.jetbrains.com/storage/products/company/brand/logos/jb_beam.svg)
