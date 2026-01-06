# rt-ut

[![License](https://img.shields.io/badge/license-MIT-blue.svg)](License)
[![C++](https://img.shields.io/badge/C%2B%2B-20-blue.svg)](https://isocpp.org/)
[![Header-only](https://img.shields.io/badge/header--only-yes-brightgreen.svg)](include/)

A small, header-only C++20 unit testing framework. Test names are carried as
compile-time string literals, assertions come in runtime and compile-time forms,
and exceptions thrown by a test are caught and reported rather than aborting the
run. One include, no build step.

## What It Is

rt-ut provides assertions and pass/fail reporting — nothing more. There are no
fixtures, no mocks, no test discovery, no registration macros. A test is a name
(a string literal template parameter) and a callable; the framework invokes it,
compares or evaluates the result, prints a line, and returns a `bool` you can
act on. That is the whole surface.

## Requirements

- A C++20 compiler with support for class-type non-type template parameters
  (used for the string-literal test names). Developed and tested against
  Clang 19; recent GCC and MSVC that implement the same feature set should work.
- CMake 3.28+

The library is header-only and has no dependencies.

The following operating systems and compilers are officially supported:

### Compiler Support
----
![MSVC](https://img.shields.io/github/actions/workflow/status/nihilai-collective/rt-ut/unit-tests.yml?style=plastic&logo=microsoft&logoColor=green&label=MSVC&labelColor=pewter&color=blue)
![GCC](https://img.shields.io/github/actions/workflow/status/nihilai-collective/rt-ut/unit-tests.yml?style=plastic&logo=linux&logoColor=green&label=GCC&labelColor=pewter&color=blue)
![CLANG](https://img.shields.io/github/actions/workflow/status/nihilai-collective/rt-ut/unit-tests.yml?style=plastic&logo=apple&logoColor=green&label=CLANG&labelColor=pewter&color=blue)

**Minimum Requirements:**
- **C++20** compliant compiler

### Operating System Support
----
![Windows](https://img.shields.io/github/actions/workflow/status/nihilai-collective/rt-ut/unit-tests.yml?style=plastic&logo=microsoft&logoColor=green&label=Windows&labelColor=pewter&color=blue)
![Linux](https://img.shields.io/github/actions/workflow/status/nihilai-collective/rt-ut/unit-tests.yml?style=plastic&logo=linux&logoColor=green&label=Linux&labelColor=pewter&color=blue)
![Mac](https://img.shields.io/github/actions/workflow/status/nihilai-collective/rt-ut/unit-tests.yml?style=plastic&logo=apple&logoColor=green&label=MacOS&labelColor=pewter&color=blue)

---

## Quick Start

```cpp
#include <rt-ut>
#include <cstdint>

int main() {
    // Runtime expected value
    rt_ut::unit_test<"add-eq">::assert_eq(
        42,
        [](int a, int b) { return a + b; },
        40, 2);

    // Compile-time expected value (non-type template parameter)
    rt_ut::unit_test<"add-eq-ct">::assert_eq<42>(
        [](int a, int b) { return a + b; },
        40, 2);

    // Inequality
    rt_ut::unit_test<"ne">::assert_ne<0>(
        [] { return 42; });

    // Predicate — passes when the callable returns true
    rt_ut::unit_test<"predicate">::run(
        [] { return 2 + 2 == 4; });

    return 0;
}
```

### Compile-Time Assertions

`compile_time_assert` lowers to a `static_assert`, so a failing test is a
compile error and there is no runtime component at all. The callable and its
arguments must be usable in a constant expression:

```cpp
rt_ut::unit_test<"constexpr-check">::compile_time_assert<true>(
    [] constexpr { return sizeof(int) > 0; });
```

## API

All methods live on `rt_ut::unit_test<name, print_all_results = true>`. When
`print_all_results` is `false`, passes are silent and only failures and errors
print.

**`run(func, args...)` → `bool`**
Invokes `func(args...)`; passes if it returns truthy. Prints
`[PASSED] <name>` on success, `[FAILED] <name> (Predicate returned false)` on a
false return.

**`assert_eq<expected>(func, args...)` → `bool`**
Compile-time expected value. Passes if `func(args...) == expected`.

**`assert_eq(expected, func, args...)` → `bool`**
Runtime expected value. Same comparison, value supplied at runtime.

**`assert_ne<expected>(func, args...)` → `bool`**
Passes if `func(args...) != expected`.

**`compile_time_assert<expected>(func, args...)`**
`static_assert` that `func(args...) == expected`. Compile error on failure,
zero runtime footprint. Requires a constant-expression-callable `func`.

All runtime methods catch exceptions: a `std::exception` prints
`[ERROR] <name> threw exception: <what>`, anything else prints
`[ERROR] <name> threw unknown exception.`, and both return `false`.

Integer results are promoted before printing (so `uint8_t`/`int8_t` show as
numbers, not characters), and `bool` results print as `true`/`false`.

## Output

```
[PASSED] add-eq
[FAILED] add-eq | expected: 100 | Got: 42
[FAILED] predicate (Predicate returned false)
[ERROR] alloc-test threw exception: bad allocation
```

Passes go to `stdout`; failures and errors go to `stderr`.

## Installation

### CMake FetchContent

```cmake
include(FetchContent)
FetchContent_Declare(
    rt-ut
    GIT_REPOSITORY https://github.com/nihilai-collective/rt-ut.git
    GIT_TAG main
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(rt-ut)

target_link_libraries(your_target PRIVATE rt-ut::rt-ut)
```

### Manual / install

```bash
git clone https://github.com/nihilai-collective/rt-ut.git
cd rt-ut
cmake -B build -DCMAKE_INSTALL_PREFIX=/path/to/install
cmake --build build --target install
```

Then `find_package(rt-ut CONFIG REQUIRED)` and link `rt-ut::rt-ut`.

## Building the Tests

```bash
cmake -S . -B build -DRT_UT_UNIT_TESTS=ON
cmake --build build
./build/test/rt-ut-unit-tests        # .\build\test\Release\rt-ut-unit-tests.exe on Windows
```

## Formatting

clang-format with the provided `.clang-format`:

```bash
find . -name '*.cpp' -o -name '*.hpp' | xargs clang-format -i
```

## Used In

[Jsonifier](https://github.com/hihilai-collective/jsonifier)  

[BenchmarkSuite](https://github.com/nihilai-collective/benchmarksuite)

## License

MIT © 2026 Nihilai Collective Corp — see [License](License.md).

