/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2026 Nihilai Collective Corp
 * https://github.com/nihilai-collective/rt-ut
 * include/rt-ut-incl/printable.hpp
 */

#pragma once

#include <rt-ut-incl/config.hpp>
#include <concepts>
#include <vector>
#include <tuple>

namespace std {

	template<typename tuple_type, std::size_t... indices> void print_tuple_elements(std::ostream& os, const tuple_type& t, std::index_sequence<indices...>) {
		((os << (indices == 0 ? "" : ", ") << std::get<indices>(t)), ...);
	}

	template<typename... types> std::ostream& operator<<(std::ostream& os RT_UT_LIFETIME_BOUND, const std::tuple<types...>& t) {
		os << '(';
		print_tuple_elements(os, t, std::index_sequence_for<types...>{});
		os << ')';
		return os;
	}
}

namespace rt_ut {

	template<typename value_type> std::ostream& print(std::ostream& os RT_UT_LIFETIME_BOUND, value_type value) {
		os << value;
		return os;
	}

	template<bool_t value_type> std::ostream& print(std::ostream& os RT_UT_LIFETIME_BOUND, value_type value) noexcept {
		os << std::boolalpha << value << std::noboolalpha;
		return os;
	}

	template<rt_ut::integer_types value_type> std::ostream& print(std::ostream& os RT_UT_LIFETIME_BOUND, value_type value) noexcept {
		os << +value;
		return os;
	}

}
