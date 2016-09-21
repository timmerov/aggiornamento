/*
Copyright (C) 2012-2016 tim cotter. All rights reserved.
*/

/**
collection of utilities and platform wrappers
that should be part of the standard libraries
but aren't.

Note: if this file is very slow to open in codeblocks...
disable symbols browser.
[Menu] Settings -> Editor... -> [Icon] Code Completion ->
    [Tab] Symbols Browser -> [Checkbox] Disable symbols browser
**/

#pragma once

// platform dependencies
#if defined(AGM_WINDOWS)
#include <aggiornamento/platform/windows.h>
#elif defined(AGM_UNIX)
#include <aggiornamento/platform/unix.h>
#else
#error Please define your platform in cmake.
#endif

// c++
#include <cinttypes>
#include <ostream>


// sizeof returns size_t.
// use of size_t is discouraged.
// on some platforms size_t is not int.
// use this handy macro instead.
#define sizeofint(x) (int(sizeof(x)))

// handy macro for converting a #defined value to a string literal
#define STRINGIZE_(x) #x
#define STRINGIZE(x) STRINGIZE_(x)

// use it on AGM_TARGET
#if defined(AGM_TARGET)
#define AGM_TARGET_NAME STRINGIZE(AGM_TARGET)
#endif

// this is a good thing from microsoft.
#if defined(AGM_WINDOWS)
#define AGM_OVERRIDE override
#else
#define AGM_OVERRIDE
#endif

namespace agm {
// the stupid. it hurts.
#if defined(AGM_WINDOWS)
    typedef int8_t   int8;
    typedef uint8_t  uint8;
    typedef int16_t  int16;
    typedef uint16_t uint16;
    typedef int32_t  int32;
    typedef uint32_t uint32;
    typedef int64_t  int64;
    typedef uint64_t uint64;
#else
    typedef std::int8_t   int8;
    typedef std::uint8_t  uint8;
    typedef std::int16_t  int16;
    typedef std::uint16_t uint16;
    typedef std::int32_t  int32;
    typedef std::uint32_t uint32;
    typedef std::int64_t  int64;
    typedef std::uint64_t uint64;
#endif

    namespace sleep {
        void milliseconds(int ms) throw();
        void seconds(int secs) throw();
    }
}

// this isn't in microsoft's version of c++11. weird.
#if defined(AGM_WINDOWS)
std::ostream & operator<<(std::ostream &s, const std::string &str) throw();
#endif
