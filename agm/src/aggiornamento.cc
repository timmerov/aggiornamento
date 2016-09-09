/*
Copyright (C) 2012-2016 tim cotter. All rights reserved.
*/

/**
implementation of utilities and platform abstractions.
**/

#include <aggiornamento/aggiornamento.h>

#include <cstring>
#include <fstream>
#include <mutex>
#include <sstream>


agm::log::Lock agm::log::lock;
agm::log::Unlock agm::log::unlock;

namespace {
    class LogStreams {
    public:
        std::ofstream file_;
        std::stringstream str_;
    };

    LogStreams *getLogStreams() throw() {
        static LogStreams g_log_streams;
        return &g_log_streams;
    }

    std::mutex *getLogMutex() throw() {
        static std::mutex g_mutex;
        return &g_mutex;
    }

    void logLineOfBytes(
        int index,
        const char *ptr,
        int count
    ) throw() {
        std::string s;
        s.reserve(4+1+32*3+32);
        static const char hexdigits[] = "0123456789ABCDEF";
        auto ch1 = hexdigits[(index>>12)&0xf];
        auto ch2 = hexdigits[(index>>8)&0xf];
        auto ch3 = hexdigits[(index>>4)&0xf];
        auto ch4 = hexdigits[index&0xf];
        s += ch1;
        s += ch2;
        s += ch3;
        s += ch4;
        s += ' ';
        for (auto i = 0; i < count; ++i) {
            unsigned char x = ptr[i];
            auto ch1 = hexdigits[x>>4];
            auto ch2 = hexdigits[x&0xf];
            s += ch1;
            s += ch2;
            s += ' ';
        }
        for (auto i = 0; i < count; ++i) {
            char ch = ptr[i];
            unsigned char uch = ch; // wtf?
            if (std::isprint(uch) == false) {
                ch = '.';
            }
            s += ch;
        }
        LOG(s.c_str());
    }
}

void agm::log::init(const char *filename) throw() {
    auto ls = getLogStreams();
    if (ls->file_.is_open() == false) {
        ls->file_.open(filename, std::ios::out | std::ios::trunc);
    }
}

void agm::log::exit() throw() {
    auto ls = getLogStreams();
    ls->file_.close();
}

std::ostream *agm::log::getStream() throw() {
    auto ls = getLogStreams();
    return &ls->str_;
}

agm::log::AsHex::AsHex(
    int hex
) throw() :
    value_(hex) {
}

void agm::log::bytes(
    const void *vp,
    int size
) throw() {
    auto ptr = (const char *) vp;
    for (auto i = 0; size > 0; i += 24) {
        auto n = std::min(size, 24);
        logLineOfBytes(i, ptr, n);
        size -= n;
        ptr  += n;
    }
}

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

std::ostream & operator<<(
    std::ostream &s,
    const agm::log::Lock &lock
) throw() {
    (void) lock;
    auto m = getLogMutex();
    m->lock();
    return s;
}

std::ostream & operator<<(
    std::ostream &s,
    const agm::log::Unlock &unlock
) throw() {
    (void) unlock;
    auto ls = getLogStreams();
    // write to the file.
    ls->file_ << ls->str_.str();
    // write to the console.
#if defined(AGM_WINDOWS)
    OutputDebugStringA(ls->str_.str().c_str());
#endif
    std::cout << ls->str_.str();
    // clear the string.
    ls->str_.str(std::string());
    auto m = getLogMutex();
    m->unlock();
    return s;
}

std::ostream & operator<<(
    std::ostream &s,
    const agm::log::AsHex &hex
) throw() {
    s << "0x" << std::hex << hex.value_ << std::dec;
    return s;
}

#if defined(AGM_WINDOWS)
std::ostream & operator<<(
    std::ostream &s,
    const std::string &str
) throw() {
    s << str.c_str();
    return s;
}
#endif
