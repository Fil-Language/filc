# Contributing

First of all, thanks for taking time to contribute! ❤️

Anyone is welcome to contribute to this project. And if you not have time to contribute, don't worry. You can also :

- Star the project ⭐
- Mention the project to your friends/colleagues 🗣

## Code of conduct

The code of conduct is described in [CODE_OF_CONDUCT.md](CODE_OF_CONDUCT.md).

## Issues

filc uses GitHub issues to track issues and development.

Before opening a new one, check that there is not an existing [issue](https://github.com/Fil-Language/filc/issues)
answering to your problem.

Then, you can open a [new issue](https://github.com/Fil-Language/filc/issues/new/choose) and choose the corresponding
template. Please ensure your description is clear and has sufficient details to be able to reproduce the issue.

## Contribute

Here is a small workflow to follow for contributing to filc:

### Open an issue

Before any contribution you should open a [new issue](https://github.com/Fil-Language/filc/issues/new/choose). An 
integrator will then discuss with you about your question. 

### Made changes

You can then [fork the repository](https://github.com/Fil-Language/filc/fork) and made your changes.

There is some guidelines to follow.

**Principles**

- No dead code: If you create a function, it's to use it
- All functions, class, types should be as strictly as possible
- Comment, sometimes it's painful to comment, but it helps you and us to understand the code, improve it, maintain it

**Conventions**

C++ conventions and rules are all listed in `.clang-tidy` and `.clang-format` files. Modern IDEs all supports
formatting from these files. If you not use an IDE, please call `clang-tidy` as often as possible to keep your code
clear.

A husky pre-commit hook is available to run `clang-format` before each commit. 

**Commits**

Please follow [Conventional commits](https://www.conventionalcommits.org/en/v1.0.0/) guidelines for your commit message.
A [template](commit-template) is available, you can use by running `git config commit.template commit-template`.

All your commit *MUST* work. If your feature is too big to be in one commit, add exceptions, early returns to keep the
code working.

In your commit message, reference the id of your issue. It helps us to track which commit concern what and why.

**Tests**

We try to keep our test coverage as high as possible. It means that everything *MUST* be tested.

If you submit a bug fix, add a test case reproducing the bug. If it's a new feature, add tests covering all code paths.

Testing code paths is not just the happy path, tests as many case as possible. This software needs to be stable.

**License**

By contributing to filc, you agree that your contributions will be licensed under the [LICENSE](LICENSE.md) file in the
root directory of this source tree.

Each code file *MUST* begin with the license text.

To help you to contribute, we recommend you to use [`nix-shell`](https://nixos.org/download/). It will help you to have 
the same tools at the same version as all others filc developers. Then you can use scripts present in `bin` directory to
build and test filc.

### Open a pull request

Once all your changes are done, you can open a pull request on top of `master` branch. An integrator will review your 
code. When it's all good, the integrator will merge your pull request. Congratulations 🎉 you've contributed to filc! 

## Security

You can read more about how to report security issues in our [Security Policy](SECURITY.md).
