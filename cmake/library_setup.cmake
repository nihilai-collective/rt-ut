# SPDX-License-Identifier: MIT
# Copyright (c) 2026 Nihilai Collective Corp
# https://github.com/nihilai-collective/rt-ut
# cmake/library_setup.cmake

add_library(${PROJECT_NAME} INTERFACE)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

set(RT_UT_COMPILE_DEFINITIONS
    RT_UT_ARCH_X64=$<IF:$<OR:$<STREQUAL:${CMAKE_SYSTEM_PROCESSOR},x86_64>,$<STREQUAL:${CMAKE_SYSTEM_PROCESSOR},AMD64>>,1,0>
    RT_UT_ARCH_ARM64=$<IF:$<OR:$<STREQUAL:${CMAKE_SYSTEM_PROCESSOR},aarch64>,$<STREQUAL:${CMAKE_SYSTEM_PROCESSOR},ARM64>,$<STREQUAL:${CMAKE_SYSTEM_PROCESSOR},arm64>>,1,0>
    RT_UT_PLATFORM_WINDOWS=$<IF:$<PLATFORM_ID:Windows>,1,0>
    RT_UT_PLATFORM_LINUX=$<IF:$<PLATFORM_ID:Linux>,1,0>
    RT_UT_PLATFORM_MAC=$<IF:$<PLATFORM_ID:Darwin>,1,0>
    RT_UT_COMPILER_CLANG=$<IF:$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>,1,0>
    RT_UT_COMPILER_MSVC=$<IF:$<CXX_COMPILER_ID:MSVC>,1,0>
    RT_UT_COMPILER_GCC=$<IF:$<CXX_COMPILER_ID:GNU>,1,0>
    "RT_UT_INLINE=$<IF:$<CONFIG:Release>,$<IF:$<CXX_COMPILER_ID:MSVC>,[[msvc::forceinline]] inline,inline __attribute__((always_inline))>,$<IF:$<CXX_COMPILER_ID:MSVC>,[[msvc::noinline]],__attribute__((noinline))>>"
    "RT_UT_LIFETIME_BOUND=$<IF:$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>,[[clang::lifetimebound]],$<IF:$<CXX_COMPILER_ID:MSVC>,[[msvc::lifetimebound]],>>"
    $<$<CXX_COMPILER_ID:MSVC>:NOMINMAX;WIN32_LEAN_AND_MEAN>
)

target_include_directories(${PROJECT_NAME}
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_compile_definitions(${PROJECT_NAME}
    INTERFACE ${RT_UT_COMPILE_DEFINITIONS}
)
