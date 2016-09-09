/*
Copyright (C) 2012-2016 tim cotter. All rights reserved.
*/

/**
log utilities and platform wrappers
that should be part of the standard libraries
but aren't.

Note: if this file is very slow to open in codeblocks...
disable symbols browser.
[Menu] Settings -> Editor... -> [Icon] Code Completion ->
    [Tab] Symbols Browser -> [Checkbox] Disable symbols browser
**/

#pragma once

#include <aggiornamento/aggiornamento.h>

// c++ stl
#include <iostream>
#include <cstdarg>


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


namespace agm {
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
}

/*
clever use of a lock to serialize logging.
we basically insert a lock and unlock into the stream.
*/
std::ostream & operator<<(std::ostream &s, const agm::log::Lock &lock) throw();
std::ostream & operator<<(std::ostream &s, const agm::log::Unlock &unlock) throw();

/*
log int's in hexadecimal format
*/
std::ostream & operator<<(std::ostream &s, const agm::log::AsHex &x) throw();
