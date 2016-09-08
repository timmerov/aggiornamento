/*
Copyright (C) 2012-2016 tim cotter. All rights reserved.
*/

#pragma once

// c++ stl
#include <iostream>
#include <cstdarg>

// platform dependencies
#define INCLUDED_AGM_HPP
#if defined(AGM_WINDOWS)
#include <aggiornamento/platform/windows.h>
#elif defined(AGM_UNIX)
#include <aggiornamento/platform/unix.h>
#else
#error Please define your platform in cmake.
#endif


// handy macro for logging
#define LOG(...) *agm::log::getStream() << agm::log::lock << \
    __FUNCTION__ << " " << __VA_ARGS__ << std::endl << agm::log::unlock

// handy macro for logging once
#define LOG_ONCE(...) { \
    static bool logged_once = false; \
    if (logged_once == false) { \
        logged_once = true; \
        LOG("(once) " << __VA_ARGS__); \
    } \
}

// handy macro for logging way too much
#if !defined(LOG_VERBOSE)
#define LOG_VERBOSE(...)
#endif

// sizeof returns size_t.
// use of size_t is discouraged.
// on some platforms size_t is not int.
// use this handy macro instead.
#define sizeofint(x) (int(sizeof(x)))

// handy macro for converting a #defined value to a string literal
#define STRINGIZE_(x) #x
#define STRINGIZE(x) STRINGIZE_(x)

// this is a good thing from microsoft.
#if defined(CASTAR_WINDOWS)
#define PAL_OVERRIDE override
#else
#define PAL_OVERRIDE
#endif

namespace agm {
// the stupid. it hurts.
#if defined(CASTAR_WINDOWS)
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

    // standardize logging.
    namespace log {
        void init(const char *filename) throw();
        void exit() throw();
        std::ostream *getStream() throw();

        // clever use of a lock to serialize logging.
        class Lock { public: };
        class Unlock { public: };
        extern Lock lock;
        extern Unlock unlock;

        // log int's in hexadecimal format
        class AsHex {
        public:
            AsHex(int hex) throw();
            int value_;
        };

        // log bytes in canonical form
        void bytes(const void *bytes, int size) throw();
    }

    namespace string {
        // safe string copy.
        // dst will be null terminated.
        // excess characters in src will be truncated.
        // custom implementation.
        void copy(char *dst, int dst_size, const char *src) throw();

        // safe string copy that uses template magic to divine the size parameter.
        // personally, i hate templates.
        // but microsoft does this.
        // and i'd hate to have to change every place we depend on it.
        template <int size>
        void copy(
           char (&dst)[size],
           const char *src
        ) throw() {
            agm::string::copy(dst, size, src);
        }

        // safe string concatenation.
        // dst will be null terminated.
        // excess characters in src will be truncated.
        // custom implementation.
        void cat(char *dst, int dst_size, const char *src) throw();

        // safe string concatenation that uses template magic to divine the size parameter.
        // personally, i hate templates.
        // but microsoft does this.
        // and i'd hate to have to change every place we depend on it.
        template <int size>
        void cat(
           char (&dst)[size],
           const char *src
        ) throw() {
            agm::string::cat(dst, size, src);
        }

        // case insensitve compare
        // on windows: calls _stricmp
        // on all other platforms: calls strcasecmp
        // because microsoft
        int compare_case(const char *s1, const char *s2) throw();

        // case insensitve compare
        // on windows: calls _strnicmp
        // on all other platforms: calls strncasecmp
        // because microsoft
        int compare_case(const char *s1, const char *s2, int len) throw();

        // tokenize a string
        // on windows: calls strtok_s
        // on all other platforms: calls strtok_r
        // because microsoft
        char *tokenize(char *token, const char *delimiters, char **context) throw();

        // formatted print a list of arguments
        // on windows: calls vsprintf_s and _vscprintf
        // on all other platforms: calls vsnprintf
        // because microsoft
        // the return value is the number of characters that would have been
        // written to sufficiently large buffer.
        // note this is different from what microsoft's vsprintf_s returns.
        int vprintf(char *buffer, int size, const char *format, va_list args) throw();

        // formatted print a list of arguments
        template <int size>
        int vprintf(
           char (&buffer)[size],
           const char *format,
           va_list args
        ) throw() {
            return agm::string::vprintf(buffer, size, format, args);
        }

        // formatted print
        // calls agm::string::vprintf on all platforms.
        // the return value is the number of characters that would have been
        // written to sufficiently large buffer.
        // note this is different from what microsoft's vsprintf_s returns.
        int printf(char *buffer, int size, const char *format, ...) throw();

        // formatted print a list of arguments
        template <int size>
        int printf(
           char (&buffer)[size],
           const char *format,
           ...
        ) throw() {
            va_list args;
            va_start(args, format);
            auto ret = agm::string::vprintf(buffer, size, format, args );
            va_end(args);
            return ret;
        }
    }

    class Thread {
        Thread() throw();
        Thread(const Thread &) = delete;
        virtual ~Thread() throw();

        // starts the thread and
        // waits for its begin function to return.
        // the thread is then blocked.
        void init() throw();

        // tells the thread to start producing.
        // the thread is unblocked.
        // its run function is called.
        void startProducing() throw();

        // tells the thread to stop producing data.
        // waits for the run function to return.
        // the thread's drain function is called.
        void stopProducing() throw();

        // tells the thread to stop draining data.
        // waits for the thread to exit.
        void stopCompletely() throw();
    };

    class Thread {
    protected:
        Thread() throw();
    public:
        Thread(const Thread &) = delete;
        virtual ~Thread() throw();

        // perform initialization.
        // like allocate memory.
        // do not produce nor consume data.
        virtual void begin() throw() {
        }

        // default implementation is to call
        // runOnce as long as isRunning is true;
        virtual void run() throw();

        // call by the default implementation of run
        // as long as isRunning is true.
        virtual void runOnce() throw() = 0;

        // default implementation is to call
        // drainOnce as long as isDraining is true;
        virtual void drain() throw();

        // call by the default implementation of drain
        // as long as isDraining is true.
        virtual void drainOnce() throw() = 0;

        // perform de-initialization.
        // like free memory.
        // do not produce nor consume data.
        virtual void end() throw() {
    };
}

// clever use of a lock to serialize logging.
// we basically throw a lock and unlock into the stream.
std::ostream & operator<<(std::ostream &s, const agm::log::Lock &lock) throw();
std::ostream & operator<<(std::ostream &s, const agm::log::Unlock &unlock) throw();

// log int's in hexadecimal format
std::ostream & operator<<(std::ostream &s, const agm::log::AsHex &x) throw();
