/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2026 Nihilai Collective Corp
 * https://github.com/nihilai-collective/rt-ut
 * include/rt-ut-incl/exception.hpp
 */

#pragma once

#include <rt-ut-incl/string_literal.hpp>
#include <type_traits>
#include <functional>
#include <concepts>
#include <sstream>
#include <string>

namespace rt_ut {

	struct rt_ut_exception {
		inline rt_ut_exception(const std::string& string) : error{ string } {
		}

		[[maybe_unused]] const std::string_view what() const RT_UT_LIFETIME_BOUND {
			return error;
		}

	  protected:
		std::string error{};
	};

}
