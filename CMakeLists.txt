# SPDX-License-Identifier: MIT
# Copyright (c) 2026 Nihilai Collective Corp
# https://github.com/nihilai-collective/rt-ut
# CMakeLists.txt

cmake_minimum_required(VERSION 3.28)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(rt-ut
    DESCRIPTION "A small, header-only C++20 unit testing framework."
    LANGUAGES CXX
)

include(cmake/library_setup.cmake)

include(cmake/installation_setup.cmake)

if(RT_UT_UNIT_TESTS)
    add_subdirectory(unit-tests)
endif()