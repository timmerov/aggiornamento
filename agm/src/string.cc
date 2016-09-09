/*
Copyright (C) 2012-2016 tim cotter. All rights reserved.
*/

/**
implementation of utilities and platform abstractions.
**/

#include <aggiornamento/aggiornamento.h>
#include <aggiornamento/string.h>

#include <cstring>
//#include <fstream>
//#include <mutex>
#include <sstream>


/*
safe string copy.
dst will be null terminated.
excess characters in src will be truncated.
custom implementation.
*/
void agm::string::copy(
    char *dst,
    int dst_size,
    const char *src
) throw() {
    char ch;
    do {
        --dst_size;
        ch = (dst_size <= 0) ? 0 : *src++;
        *dst++ = ch;
    } while (ch);
}

/*
safe string concatenation.
dst will be null terminated.
excess characters in src will be truncated.
custom implementation.
*/
void agm::string::cat(
    char *dst,
    int dst_size,
    const char *src
) throw() {
    auto len = strlen(dst);
    dst += len;
    dst_size -= (int) len;
    agm::string::copy(dst, dst_size, src);
}

/*
case insensitve compare
on windows: calls _stricmp
on all other platforms: calls strcasecmp
because microsoft
*/
int agm::string::compare_case(
    const char *s1,
    const char *s2
) throw() {
#if defined(AGM_WINDOWS)
    return _strcmpi(s1, s2);
#else
    return strcasecmp(s1, s2);
#endif
}

/*
case insensitve compare
on windows: calls _strnicmp
on all other platforms: calls strncasecmp
because microsoft
*/
int agm::string::compare_case(
    const char *s1,
    const char *s2,
    int len
) throw() {
#if defined(AGM_WINDOWS)
    return _strnicmp(s1, s2, len);
#else
    return strncasecmp(s1, s2, len);
#endif
}

/*
tokenize a string
on windows: calls strtok_s
on all other platforms: calls strtok_r
because microsoft
*/
char *agm::string::tokenize(
    char *token,
    const char *delimiters,
    char **context
) throw() {
#if defined(AGM_WINDOWS)
    return strtok_s(token, delimiters, context);
#else
    return strtok_r(token, delimiters, context);
#endif
}

/*
formatted print
calls agm::string::vprintf on all platforms.
the return value is the number of characters that would have been
written to sufficiently large buffer.
note this is different from what microsoft's vsprintf_s returns.
*/
int agm::string::printf(
    char *buffer,
    int size,
    const char *format,
    ...
) throw() {
    va_list args;
    va_start(args, format);
    auto ret = agm::string::vprintf(buffer, size, format, args );
    va_end(args);
    return ret;
}

/*
formatted print a list of arguments
on windows: calls vsprintf_s and _vscprintf
on all other platforms: calls vsnprintf
because microsoft
the return value is the number of characters that would have been
written to sufficiently large buffer.
note this is different from what microsoft's vsprintf_s returns.
*/
int agm::string::vprintf(
    char *buffer,
    int size,
    const char *format,
    va_list args
) throw() {
#if defined(AGM_WINDOWS)
    vsprintf_s(buffer, size, format, args);
    return _vscprintf(format, args);
#else
    return vsnprintf(buffer, size, format, args);
#endif
}
