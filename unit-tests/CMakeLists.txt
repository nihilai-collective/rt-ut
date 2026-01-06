# SPDX-License-Identifier: MIT
# Copyright (c) 2026 Nihilai Collective Corp
# https://github.com/nihilai-collective/rt-ut
# unit-tests/CMakeLists.txt

add_executable(rt-ut-unit-tests main.cpp)

target_link_libraries(rt-ut-unit-tests
    PUBLIC
        rt-ut::rt-ut
)

option(RT_UT_ASAN  "Enable AddressSanitizer"            OFF)
option(RT_UT_UBSAN "Enable UndefinedBehaviorSanitizer"  OFF)

set(RT_UT_ASAN_EFFECTIVE  ${RT_UT_ASAN})
set(RT_UT_UBSAN_EFFECTIVE ${RT_UT_UBSAN})

if(APPLE AND CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(RT_UT_ASAN_EFFECTIVE OFF)
    set(RT_UT_UBSAN_EFFECTIVE OFF)
    message(STATUS "Disabling sanitizers: not supported with GCC on macOS.")
endif()

if(RT_UT_UBSAN_EFFECTIVE AND CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    message(WARNING
        "RT_UT_UBSAN has no effect with MSVC (no equivalent runtime). "
        "Use Clang-cl or a Clang/GCC build to get UBSan coverage.")
    set(RT_UT_UBSAN_EFFECTIVE OFF)
endif()

set(RT_UT_HOMEBREW_GCC_LIBDIR "")
if(APPLE AND CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    find_program(RT_UT_BREW_EXE brew)
    if(RT_UT_BREW_EXE)
        execute_process(
            COMMAND ${RT_UT_BREW_EXE} --prefix gcc
            OUTPUT_VARIABLE RT_UT_BREW_GCC_PREFIX
            OUTPUT_STRIP_TRAILING_WHITESPACE
            RESULT_VARIABLE RT_UT_BREW_RC
        )
        if(RT_UT_BREW_RC EQUAL 0 AND RT_UT_BREW_GCC_PREFIX)
            file(GLOB RT_UT_GCC_LIB_CANDIDATES
                "${RT_UT_BREW_GCC_PREFIX}/lib/gcc/*")
            if(RT_UT_GCC_LIB_CANDIDATES)
                list(SORT RT_UT_GCC_LIB_CANDIDATES)
                list(REVERSE RT_UT_GCC_LIB_CANDIDATES)
                list(GET RT_UT_GCC_LIB_CANDIDATES 0 RT_UT_HOMEBREW_GCC_LIBDIR)
            endif()
        endif()
    endif()
    if(NOT RT_UT_HOMEBREW_GCC_LIBDIR)
        message(STATUS
            "Could not auto-detect Homebrew GCC runtime path; link-time "
            "rpath for libstdc++ will not be added. Set "
            "RT_UT_HOMEBREW_GCC_LIBDIR manually if needed.")
    endif()
endif()

set(RT_UT_MSVC_COMPILER_OPTIONS
    $<$<AND:$<CONFIG:Debug>,$<NOT:$<BOOL:${RT_UT_ASAN_EFFECTIVE}>>>:/Od;/Zi;/RTC1>
    $<$<AND:$<CONFIG:Debug>,$<BOOL:${RT_UT_ASAN_EFFECTIVE}>>:/Od;/Zi>
    $<$<BOOL:${RT_UT_ASAN_EFFECTIVE}>:/fsanitize=address>
    /wd4820
    /wd5039
    /wd4710
    /wd5045
    /wd4324
    /wd4711
    /Wall
    /W4
    /WX
)

set(RT_UT_CLANG_COMPILER_OPTIONS
    $<$<AND:$<CONFIG:Release>,$<BOOL:${RT_UT_ASAN_EFFECTIVE}>>:-O1;-fno-rtti>
    $<$<CONFIG:Debug>:-O0;-g;-fno-omit-frame-pointer>
    $<$<BOOL:${RT_UT_ASAN_EFFECTIVE}>:-fsanitize=address;-fno-omit-frame-pointer;-fno-optimize-sibling-calls;-fsanitize-address-use-after-scope;-U_FORTIFY_SOURCE;-D_FORTIFY_SOURCE=0>
    $<$<BOOL:${RT_UT_UBSAN_EFFECTIVE}>:-fsanitize=undefined;-fno-sanitize-recover=all>
    -Weverything
    -Wpedantic
    -Werror
    -Wextra
    -Wall
    -Wno-c++98-compat-pedantic
    -Wno-c++98-compat
    -Wno-padded
)

set(RT_UT_GNU_COMPILER_OPTIONS
    $<$<CONFIG:Debug>:-O0;-g>
    $<$<BOOL:${RT_UT_ASAN_EFFECTIVE}>:-fsanitize=address;-fsanitize-address-use-after-scope;-U_FORTIFY_SOURCE;-D_FORTIFY_SOURCE=0>
    $<$<BOOL:${RT_UT_UBSAN_EFFECTIVE}>:-fsanitize=undefined;-fno-sanitize-recover=all>
    -Wpedantic
    -Werror
    -Wextra
    -Wall
)

set(RT_UT_COMPILER_OPTIONS
    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>:${RT_UT_CLANG_COMPILER_OPTIONS}>
    $<$<CXX_COMPILER_ID:GNU>:${RT_UT_GNU_COMPILER_OPTIONS}>
    $<$<CXX_COMPILER_ID:MSVC>:${RT_UT_MSVC_COMPILER_OPTIONS}>
)

target_compile_options(
    rt-ut-unit-tests PUBLIC
    ${RT_UT_COMPILER_OPTIONS}
)

set(RT_UT_MSVC_LINK_OPTIONS
    $<$<BOOL:${RT_UT_ASAN_EFFECTIVE}>:/DEBUG>
    $<$<BOOL:${RT_UT_UBSAN_EFFECTIVE}>:/DEBUG>
)

set(RT_UT_CLANG_LINK_OPTIONS
    $<$<BOOL:${RT_UT_ASAN_EFFECTIVE}>:-fsanitize=address>
    $<$<BOOL:${RT_UT_UBSAN_EFFECTIVE}>:-fsanitize=undefined>
)

set(RT_UT_GNU_LINK_OPTIONS
    $<$<BOOL:${RT_UT_ASAN_EFFECTIVE}>:-fsanitize=address>
    $<$<BOOL:${RT_UT_UBSAN_EFFECTIVE}>:-fsanitize=undefined>
    $<$<AND:$<PLATFORM_ID:Linux>,$<BOOL:${RT_UT_ASAN_EFFECTIVE}>>:-static-libasan>
)

set(RT_UT_LINK_OPTIONS
    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>:${RT_UT_CLANG_LINK_OPTIONS}>
    $<$<CXX_COMPILER_ID:GNU>:${RT_UT_GNU_LINK_OPTIONS}>
    $<$<CXX_COMPILER_ID:MSVC>:${RT_UT_MSVC_LINK_OPTIONS}>
)

target_link_options(rt-ut-unit-tests
    PUBLIC ${RT_UT_LINK_OPTIONS}
)

install(
    FILES
        $<TARGET_FILE:rt-ut-unit-tests>
    DESTINATION bin
    OPTIONAL
)
