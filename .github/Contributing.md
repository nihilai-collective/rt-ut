# Contributing to rt-ut

Thanks for your interest in improving rt-ut. This guide covers how to report problems, propose changes, and get a pull request merged.

## Reporting Bugs

Open an issue and include:

- A minimal test that reproduces the problem.
- Compiler and version, operating system, and target architecture.
- Expected behavior versus actual behavior, including the reported output.

## Proposing Changes

For anything larger than a small fix, open an issue first so the approach can be discussed before you invest time in it.

1. Fork the repository and create a branch from `main`.
2. Make your change.
3. Build and run the unit tests locally (see below).
4. Open a pull request against `main` describing what changed and why.

## Building and Testing

rt-ut is built with CMake; presets are in `CMakePresets.json`. Tests live in `unit-tests` and are enabled with `-DRT_UT_UNIT_TESTS=TRUE`. CI runs them with `-DRT_UT_ASAN=TRUE -DRT_UT_UBSAN=TRUE`, so please do the same locally.

Every pull request is built by CI on the following matrix, and all of it must pass:

| OS      | Compilers |
|---------|-----------|
| Ubuntu  | Clang, GCC |
| macOS   | Clang, GCC |
| Windows | MSVC |

## Code Style

- Run `run-clang-format.ps1` before committing; formatting follows the repository `.clang-format`.
- Do not add explanatory comments to code. Write self-documenting code instead. Comments that attribute algorithms, techniques, or implementations to their original authors or sources are explicitly permitted. If current-us cannot write self-documenting code, then future-us does not deserve to understand it.
- Keep includes lean. Only include what the file actually uses.
- Keep changes focused. Unrelated refactors belong in their own pull request.

## Scope

rt-ut is deliberately small: assertions and pass/fail reporting, nothing more. Proposals for fixtures, mocks, test discovery, or registration macros are out of scope. Changes that keep the surface small and the header free of dependencies are the ones most likely to be accepted.

## Pull Request Checklist

- [ ] Builds and passes tests on all CI platforms
- [ ] Formatted with clang-format
- [ ] No added explanatory code comments (source attributions are permitted)
- [ ] No new dependencies
- [ ] Linked to the issue it addresses, if any

## Licensing

By contributing, you agree that your contributions are licensed under the project's MIT license (see `License.md`).
