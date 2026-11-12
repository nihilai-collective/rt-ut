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

	struct rt_ut_exception : public std::runtime_error {
		explicit rt_ut_exception(const std::string& string);
		~rt_ut_exception() override;
	};

	rt_ut_exception::rt_ut_exception(const std::string& string) : std::runtime_error{ string } {
	}

	rt_ut_exception::~rt_ut_exception() = default;

}
