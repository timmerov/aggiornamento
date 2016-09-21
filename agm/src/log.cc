/*
Copyright (C) 2012-2016 tim cotter. All rights reserved.
*/

/**
implementation of utilities and platform abstractions.
**/

#include <aggiornamento/aggiornamento.h>
#include <aggiornamento/log.h>

#include <algorithm>
#include <cctype>
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
            auto ch5 = hexdigits[x>>4];
            auto ch6 = hexdigits[x&0xf];
            s += ch5;
            s += ch6;
            s += ' ';
        }
        for (auto i = 0; i < count; ++i) {
            char ch7 = ptr[i];
            unsigned char uch = ch7; // wtf?
            if (std::isprint(uch) == false) {
                ch7 = '.';
            }
            s += ch7;
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
