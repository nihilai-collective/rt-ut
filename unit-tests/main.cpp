/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2026 Nihilai Collective Corp
 * https://github.com/nihilai-collective/rt-ut
 * unit-tests/main.cpp
 */

#include <rt-ut>
#include <functional>
#include <string_view>
#include <memory>
#include <cctype>
#include <tuple>
#include <cmath>

using namespace rt_ut;

template<typename callable_type> inline bool expect_rt_ut_throw(callable_type&& callable, std::string_view expected_substring) {
	try {
		callable();
	} catch (const rt_ut_exception& e) {
		return e.what().find(expected_substring) != std::string_view::npos;
	}
	return false;
}

inline int32_t ten() {
	return 10;
}

inline double halve(double x) {
	return x / 2.0;
}

inline std::string lowercase_greeting() {
	return "hello";
}

inline std::string lowercase_world() {
	return "world";
}

struct approx_equal_double {
	bool operator()(double a, double b) const {
		return std::abs(a - b) < 0.0001;
	}
};

struct case_insensitive_equal {
	bool operator()(const std::string& a, const std::string& b) const {
		if (a.size() != b.size()) {
			return false;
		}
		return std::equal(a.begin(), a.end(), b.begin(), [](char c1, char c2) {
			return std::tolower(static_cast<unsigned char>(c1)) == std::tolower(static_cast<unsigned char>(c2));
		});
	}
};

struct case_insensitive_not_equal {
	bool operator()(const std::string& a, const std::string& b) const {
		return !case_insensitive_equal{}(a, b);
	}
};

int32_t main([[maybe_unused]] int32_t argc, [[maybe_unused]] char** argv) {
	bool all_passed = true;
	try {
		all_passed &= unit_test<"run-true">::run([] {
			return true;
		});

		all_passed &= unit_test<"run-true-with-args">::run(
			[](int32_t a, int32_t b) {
				return a + b == 5;
			},
			2, 3);

		all_passed &= unit_test<"run-true-with-many-args">::run(
			[](int32_t a, int32_t b, int32_t c, int32_t d) {
				return a + b + c + d == 10;
			},
			1, 2, 3, 4);

		all_passed &= unit_test<"run-forwards-move-only-arg">::run(
			[](std::unique_ptr<int32_t> value) {
				return value != nullptr && *value == 7;
			},
			std::make_unique<int32_t>(7));

		all_passed &= unit_test<"run-no-print", false>::run([] {
			return true;
		});

		all_passed &= unit_test<"run-false-no-throw", true, false>::run([] {
			return false;
		}) == false;

		all_passed &= unit_test<"soft-fail-returns-false", false, false>::run([] {
			return false;
		}) == false;

		all_passed &= unit_test<"run-false-throws-verify">::run([] {
			return expect_rt_ut_throw(
				[] {
					unit_test<"inner-run-false">::run([] {
						return false;
					});
				},
				"[FAILED] inner-run-false");
		});

		all_passed &= unit_test<"run-throws-std-exception-handled">::run([] {
			return unit_test<"inner-run-throws-std">::run([]() -> bool {
				throw std::runtime_error("boom");
			}) == false;
		});

		all_passed &= unit_test<"run-throws-unknown-exception-handled">::run([] {
			return unit_test<"inner-run-throws-unknown">::run([]() -> bool {
				throw std::string("not-a-std-exception");
			}) == false;
		});

		all_passed &= unit_test<"assert_eq-nttp-int">::assert_eq<5>([] {
			return 5;
		});

		all_passed &= unit_test<"assert_eq-nttp-narrowing">::assert_eq<2>(
			[](int32_t) {
				return uint8_t(2);
			},
			23);

		all_passed &= unit_test<"assert_eq-nttp-with-args">::assert_eq<42>(
			[](int32_t x, int32_t y) {
				return x * y;
			},
			6, 7);

		all_passed &= unit_test<"assert_eq-nttp-bool">::assert_eq<true>([] {
			return 1 == 1;
		});

		all_passed &= unit_test<"assert_eq-nttp-char">::assert_eq<'z'>([] {
			return char('z');
		});

		all_passed &= unit_test<"assert_eq-nttp-negative-int">::assert_eq<-7>([] {
			return -7;
		});

		all_passed &= unit_test<"assert_eq-nttp-custom-comparator">::assert_eq<5, int32_t (*)(), std::greater_equal<>>(ten);

		all_passed &= unit_test<"assert_eq-nttp-mismatch-no-throw", true, false>::assert_eq<5>([] {
			return 4;
		}) == false;

		all_passed &= unit_test<"assert_eq-nttp-mismatch-throws-verify">::run([] {
			return expect_rt_ut_throw(
				[] {
					unit_test<"inner-assert-eq-nttp-mismatch">::assert_eq<5>([] {
						return 4;
					});
				},
				"expected:");
		});

		all_passed &= unit_test<"assert_eq-nttp-throwing-func-handled">::run([] {
			return unit_test<"inner-assert-eq-nttp-throws">::assert_eq<5>([]() -> int32_t {
				throw std::runtime_error("nope");
			}) == false;
		});

		all_passed &= unit_test<"assert_ne-nttp-int">::assert_ne<5>([] {
			return 4;
		});

		all_passed &= unit_test<"assert_ne-nttp-with-args">::assert_ne<0>(
			[](int32_t x) {
				return x + 1;
			},
			10);

		all_passed &= unit_test<"assert_ne-nttp-bool">::assert_ne<false>([] {
			return true;
		});

		all_passed &= unit_test<"assert_ne-nttp-custom-comparator">::assert_ne<3, int32_t (*)(), std::greater<>>(ten);

		all_passed &= unit_test<"assert_ne-nttp-mismatch-no-throw", true, false>::assert_ne<5>([] {
			return 5;
		}) == false;

		all_passed &= unit_test<"assert_ne-nttp-mismatch-throws-verify">::run([] {
			return expect_rt_ut_throw(
				[] {
					unit_test<"inner-assert-ne-nttp-mismatch">::assert_ne<5>([] {
						return 5;
					});
				},
				"expected:");
		});

		all_passed &= unit_test<"assert_eq-runtime-expected">::assert_eq(
			7,
			[](int32_t x) {
				return x + 2;
			},
			5);

		all_passed &= unit_test<"assert_eq-runtime-string">::assert_eq(std::string("hello"), [] {
			return std::string("hello");
		});

		all_passed &= unit_test<"assert_eq-runtime-double">::assert_eq(
			3.5,
			[](double x) {
				return x / 2.0;
			},
			7.0);

		all_passed &= unit_test<"assert_eq-runtime-custom-comparator">::assert_eq<std::string, std::string (*)(), case_insensitive_equal>(std::string("HELLO"), lowercase_greeting);

		all_passed &= unit_test<"assert_eq-runtime-custom-comparator-with-args">::assert_eq<double, double (*)(double), approx_equal_double>(1.75, halve, 3.5);

		all_passed &= unit_test<"assert_eq-runtime-mismatch-no-throw", true, false>::assert_eq(
			100,
			[](int32_t x) {
				return x;
			},
			1) == false;

		all_passed &= unit_test<"assert_eq-runtime-mismatch-throws-verify">::run([] {
			return expect_rt_ut_throw(
				[] {
					unit_test<"inner-assert-eq-runtime-mismatch">::assert_eq(100, [] {
						return 1;
					});
				},
				"Got:");
		});

		all_passed &= unit_test<"assert_eq-runtime-throwing-func-handled">::run([] {
			return unit_test<"inner-assert-eq-runtime-throws">::assert_eq(1, []() -> int32_t {
				throw std::runtime_error("nope");
			}) == false;
		});

		all_passed &= unit_test<"assert_ne-runtime-expected">::assert_ne(
			100,
			[](int32_t x) {
				return x;
			},
			99);

		all_passed &= unit_test<"assert_ne-runtime-string">::assert_ne(std::string("foo"), [] {
			return std::string("bar");
		});

		all_passed &= unit_test<"assert_ne-runtime-custom-comparator">::assert_ne<std::string, std::string (*)(), case_insensitive_not_equal>(std::string("HELLO"), lowercase_world);

		all_passed &= unit_test<"assert_ne-runtime-mismatch-no-throw", true, false>::assert_ne(
			5,
			[](int32_t x) {
				return x;
			},
			5) == false;

		all_passed &= unit_test<"assert_ne-runtime-mismatch-throws-verify">::run([] {
			return expect_rt_ut_throw(
				[] {
					unit_test<"inner-assert-ne-runtime-mismatch">::assert_ne(5, [] {
						return 5;
					});
				},
				"expected:");
		});

		all_passed &= unit_test<"assert_eq-no-print", false>::assert_eq<1>([] {
			return 1;
		});

		all_passed &= unit_test<"concept-bool-t-classification">::run([]() constexpr {
			return bool_t<bool> && !bool_t<int32_t> && !bool_t<double>;
		});

		all_passed &= unit_test<"concept-integer-types-classification">::run([]() constexpr {
			return integer_types<int32_t> && integer_types<uint8_t> && !integer_types<bool> && !integer_types<double>;
		});

		all_passed &= unit_test<"concept-std-exception-type-is-exact-match-only">::run([]() constexpr {
			return std_exception_type<std::exception> && !std_exception_type<std::runtime_error>;
		});

		all_passed &= unit_test<"string-literal-equality-true">::run([] {
			constexpr string_literal<4> a{ "abc" };
			constexpr string_literal<4> b{ "abc" };
			return a == b;
		});

		all_passed &= unit_test<"string-literal-equality-false-same-length">::run([] {
			constexpr string_literal<4> a{ "abc" };
			constexpr string_literal<4> b{ "abd" };
			return !(a == b);
		});

		all_passed &= unit_test<"string-literal-equality-false-different-length">::run([] {
			constexpr string_literal<4> a{ "abc" };
			constexpr string_literal<3> b{ "ab" };
			return !(a == b);
		});

		all_passed &= unit_test<"string-literal-size">::assert_eq<uint64_t(3)>([] {
			constexpr string_literal<4> a{ "abc" };
			return a.size();
		});

		all_passed &= unit_test<"string-literal-count-digits-zero">::assert_eq<uint64_t(1)>([] {
			return count_digits(0);
		});

		all_passed &= unit_test<"string-literal-count-digits-positive">::assert_eq<uint64_t(3)>([] {
			return count_digits(123);
		});

		all_passed &= unit_test<"string-literal-count-digits-negative">::assert_eq<uint64_t(4)>([] {
			return count_digits(-123);
		});

		all_passed &= unit_test<"string-literal-count-digits-unsigned">::assert_eq<uint64_t(3)>([] {
			return count_digits(255u);
		});

		all_passed &= unit_test<"print-generic-string">::assert_eq(std::string("hi"), [] {
			std::stringstream ss{};
			print(ss, std::string("hi"));
			return ss.str();
		});

		all_passed &= unit_test<"print-integer-types-uint8-not-char">::assert_eq(std::string("65"), [] {
			std::stringstream ss{};
			print(ss, uint8_t{ 65 });
			return ss.str();
		});

		all_passed &= unit_test<"print-integer-types-int8-not-char">::assert_eq(std::string("-5"), [] {
			std::stringstream ss{};
			print(ss, int8_t{ -5 });
			return ss.str();
		});

		all_passed &= unit_test<"print-bool-true-as-word">::assert_eq(std::string("true"), [] {
			std::stringstream ss{};
			print(ss, true);
			return ss.str();
		});

		all_passed &= unit_test<"print-bool-false-as-word">::assert_eq(std::string("false"), [] {
			std::stringstream ss{};
			print(ss, false);
			return ss.str();
		});

		all_passed &= unit_test<"print-tuple-operator-shift">::assert_eq(std::string("(1, 2.5)"), [] {
			std::stringstream ss{};
			ss << std::make_tuple(1, 2.5);
			return ss.str();
		});

	} catch (const rt_ut_exception& e) {
		std::cout << "Error: " << e.what() << std::endl;
		return 1;
	}
	return all_passed ? 0 : 1;
}
