/*
Copyright (C) 2012-2016 tim cotter. All rights reserved.
*/

/**
implementation of utilities and platform abstractions.
**/

#include <aggiornamento/aggiornamento.h>

#include <chrono>
#include <thread>


void agm::sleep::milliseconds(
    int ms
) noexcept {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

void agm::sleep::seconds(
    int secs
) noexcept {
    std::this_thread::sleep_for(std::chrono::seconds(secs));
}

agm::int64 agm::time::microseconds() noexcept {
    static bool set = false;
    static std::chrono::time_point<std::chrono::high_resolution_clock> start;
    auto now = std::chrono::high_resolution_clock::now();
    if (set == false) {
        set = true;
        start = now;
    }
    auto elapsed = now - start;
    agm::int64 us = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
    return us;
}


// this isn't in microsoft's version of c++11. weird.
#if defined(AGM_WINDOWS)
std::ostream & operator<<(
    std::ostream &s,
    const std::string &str
) noexcept {
    s << str.c_str();
    return s;
}
#endif
