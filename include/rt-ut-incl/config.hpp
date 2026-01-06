/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2026 Nihilai Collective Corp
 * https://github.com/nihilai-collective/rt-ut
 * include/rt-ut-incl/config.hpp
 */

#pragma once

#include <concepts>
#include <vector>

namespace rt_ut {

	template<typename v_type>
	concept std_exception_type = std::is_same_v<std::remove_cvref_t<v_type>, std::exception>;

	template<typename value_type>
	concept bool_t = std::same_as<std::remove_cvref_t<value_type>, bool> || std::same_as<std::remove_cvref_t<value_type>, std::vector<bool>::reference> ||
		std::same_as<std::remove_cvref_t<value_type>, std::vector<bool>::const_reference>;

	template<typename value_type>
	concept integer_types = std::integral<std::remove_cvref_t<value_type>> && !bool_t<value_type> && !std::floating_point<std::remove_cvref_t<value_type>>;

}
