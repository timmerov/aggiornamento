/*
Copyright (C) 2012-2016 tim cotter. All rights reserved.
*/

/**
string utilities and platform wrappers
that should be part of the standard libraries
but aren't.

Note: if this file is very slow to open in codeblocks...
disable symbols browser.
[Menu] Settings -> Editor... -> [Icon] Code Completion ->
    [Tab] Symbols Browser -> [Checkbox] Disable symbols browser
**/

#pragma once

#include <aggiornamento/aggiornamento.h>

// c++
#include <cstdarg>


namespace agm {
    namespace string {
        /*
        safe string copy.
        dst will be null terminated.
        excess characters in src will be truncated.
        custom implementation.
        */
        void copy(char *dst, int dst_size, const char *src) noexcept;

        /*
        safe string copy that uses template magic to divine the size parameter.
        personally, i hate templates.
        but microsoft does this.
        and i'd hate to have to change every place we depend on it.
        */
        template <int size>
        void copy(
           char (&dst)[size],
           const char *src
        ) noexcept {
            agm::string::copy(dst, size, src);
        }

        /*
        safe string concatenation.
        dst will be null terminated.
        excess characters in src will be truncated.
        custom implementation.
        */
        void cat(char *dst, int dst_size, const char *src) noexcept;

        /*
        safe string concatenation that uses template magic to divine the size parameter.
        personally, i hate templates.
        but microsoft does this.
        and i'd hate to have to change every place we depend on it.
        */
        template <int size>
        void cat(
           char (&dst)[size],
           const char *src
        ) noexcept {
            agm::string::cat(dst, size, src);
        }

        /*
        case insensitve compare
        on windows: calls _stricmp
        on all other platforms: calls strcasecmp
        because microsoft
        */
        int compare_case(const char *s1, const char *s2) noexcept;

        /*
        case insensitve compare
        on windows: calls _strnicmp
        on all other platforms: calls strncasecmp
        because microsoft
        */
        int compare_case(const char *s1, const char *s2, int len) noexcept;

        /*
        tokenize a string
        on windows: calls strtok_s
        on all other platforms: calls strtok_r
        because microsoft
        */
        char *tokenize(char *token, const char *delimiters, char **context) noexcept;

        /*
        formatted print a list of arguments
        on windows: calls vsprintf_s and _vscprintf
        on all other platforms: calls vsnprintf
        because microsoft
        the return value is the number of characters that would have been
        written to sufficiently large buffer.
        note this is different from what microsoft's vsprintf_s returns.
        */
        int vprintf(char *buffer, int size, const char *format, va_list args) noexcept;

        /*
        formatted print a list of arguments
        */
        template <int size>
        int vprintf(
           char (&buffer)[size],
           const char *format,
           va_list args
        ) noexcept {
            return agm::string::vprintf(buffer, size, format, args);
        }

        /*
        formatted print
        calls agm::string::vprintf on all platforms.
        the return value is the number of characters that would have been
        written to a sufficiently large buffer.
        note this is different from what microsoft's vsprintf_s returns.
        */
        int printf(char *buffer, int size, const char *format, ...) noexcept;

        /*
        formatted print a list of arguments
        the return value is the number of characters that would have been
        written to a sufficiently large buffer.
        note this is different from what microsoft's vsprintf_s returns.
        */
        template <int size>
        int printf(
           char (&buffer)[size],
           const char *format,
           ...
        ) noexcept {
            va_list args;
            va_start(args, format);
            auto ret = agm::string::vprintf(buffer, size, format, args );
            va_end(args);
            return ret;
        }
    }
}
