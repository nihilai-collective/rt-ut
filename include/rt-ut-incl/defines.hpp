/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2026 Nihilai Collective Corp
 * https://github.com/nihilai-collective/rt-ut
 * include/rt-ut-incl/defines.hpp
 */

#pragma once

#ifndef RT_UT_ARCH_X64
	#if defined(__x86_64__) || defined(_M_X64) || defined(_M_AMD64)
		#define RT_UT_ARCH_X64 1
	#else
		#define RT_UT_ARCH_X64 0
	#endif
#endif

#ifndef RT_UT_ARCH_ARM64
	#if defined(__aarch64__) || defined(_M_ARM64)
		#define RT_UT_ARCH_ARM64 1
	#else
		#define RT_UT_ARCH_ARM64 0
	#endif
#endif

#ifndef RT_UT_PLATFORM_WINDOWS
	#if defined(_WIN32)
		#define RT_UT_PLATFORM_WINDOWS 1
	#else
		#define RT_UT_PLATFORM_WINDOWS 0
	#endif
#endif

#ifndef RT_UT_PLATFORM_LINUX
	#if defined(__linux__)
		#define RT_UT_PLATFORM_LINUX 1
	#else
		#define RT_UT_PLATFORM_LINUX 0
	#endif
#endif

#ifndef RT_UT_PLATFORM_MAC
	#if defined(__APPLE__)
		#define RT_UT_PLATFORM_MAC 1
	#else
		#define RT_UT_PLATFORM_MAC 0
	#endif
#endif

#ifndef RT_UT_COMPILER_CLANG
	#if defined(__clang__)
		#define RT_UT_COMPILER_CLANG 1
	#else
		#define RT_UT_COMPILER_CLANG 0
	#endif
#endif

#ifndef RT_UT_COMPILER_MSVC
	#if defined(_MSC_VER) && !defined(__clang__)
		#define RT_UT_COMPILER_MSVC 1
	#else
		#define RT_UT_COMPILER_MSVC 0
	#endif
#endif

#ifndef RT_UT_COMPILER_GCC
	#if defined(__GNUC__) && !defined(__clang__)
		#define RT_UT_COMPILER_GCC 1
	#else
		#define RT_UT_COMPILER_GCC 0
	#endif
#endif

#ifndef RT_UT_INLINE
	#if defined(NDEBUG)
		#if RT_UT_COMPILER_MSVC
			#define RT_UT_INLINE [[msvc::forceinline]] inline
		#else
			#define RT_UT_INLINE inline __attribute__((always_inline))
		#endif
	#else
		#if RT_UT_COMPILER_MSVC
			#define RT_UT_INLINE [[msvc::noinline]]
		#else
			#define RT_UT_INLINE __attribute__((noinline))
		#endif
	#endif
#endif

#ifndef RT_UT_LIFETIME_BOUND
	#if RT_UT_COMPILER_CLANG
		#define RT_UT_LIFETIME_BOUND [[clang::lifetimebound]]
	#elif RT_UT_COMPILER_MSVC
		#define RT_UT_LIFETIME_BOUND [[msvc::lifetimebound]]
	#else
		#define RT_UT_LIFETIME_BOUND
	#endif
#endif
