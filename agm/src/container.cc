/*
Copyright (C) 2012-2016 tim cotter. All rights reserved.
*/

/**
container class implementation.

init is called after everything is created but before
threads are started.

unblock is called to unblock any thread that might
be waiting for data to appear in the container.
containers should not block threads after unblock is called.

exit is called after all threads have stopped but before
any container has been deleted.
**/

#include <aggiornamento/aggiornamento.h>
#include <aggiornamento/log.h>
#include <aggiornamento/container.h>

// pick one
#undef LOG_VERBOSE
//#define LOG_VERBOSE LOG
#define LOG_VERBOSE(...)


agm::Container::Container(
    const char *name
) noexcept :
    name_(name) {
}

agm::Container::~Container() noexcept {
}

std::string agm::Container::getName() const noexcept {
    return name_;
}
