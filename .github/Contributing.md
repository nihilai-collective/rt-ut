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

### Declarations

- Namespace-scope constexpr values are `inline constexpr`.
- Class-scope constexpr values are `static constexpr`.
- Function-scope constexpr values are `static constexpr` once the project targets C++23 or later. While it targets C++20, leave them as plain `constexpr`.
- Mark every member function that can be `const` as `const`.
- Mark every function that can be `noexcept` as `noexcept`. Functions that allocate, write to streams, or throw by design stay unmarked.
- Mark every function that can be `consteval` as `consteval`. Functions that callers may reasonably invoke at runtime stay `constexpr`.
- Mark every function `inline` (or `RT_UT_INLINE`) unless it is already `consteval`.
- Every enum has an explicit underlying type: the smallest unsigned integer type that holds all of its values.

### Member Ordering

Order the contents of every struct and class like this:

1. Type aliases
2. Default constructor
3. Parameterized constructors
4. Move assignment operator
5. Move constructor
6. Copy assignment operator
7. Copy constructor
8. Other functions, in descending order of the character count of their longest line
9. Destructor
10. Data members, ordered first to optimize alignment, then in descending order of line length

Namespace-scope functions follow the same descending longest-line rule, after any ordering their dependencies require (a function used in another declaration's signature or default template argument comes first).

## Scope

rt-ut is deliberately small: assertions and pass/fail reporting, nothing more. Proposals for fixtures, mocks, test discovery, or registration macros are out of scope. Changes that keep the surface small and the header free of dependencies are the ones most likely to be accepted.

## Pull Request Checklist

- [ ] Builds and passes tests on all CI platforms
- [ ] Formatted with clang-format
- [ ] No added explanatory code comments (source attributions are permitted)
- [ ] Declarations and member ordering follow the Code Style rules
- [ ] No new dependencies
- [ ] Linked to the issue it addresses, if any

## Licensing

By contributing, you agree that your contributions are licensed under the project's MIT license (see `License.md`).
