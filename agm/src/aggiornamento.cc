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
