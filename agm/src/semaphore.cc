/*
Copyright (C) 2012-2016 tim cotter. All rights reserved.
*/

/**
semaphore class implementation.

binary semaphore.
**/

#include <aggiornamento/aggiornamento.h>
#include <aggiornamento/log.h>
#include <aggiornamento/semaphore.h>

// pick one
#undef LOG_VERBOSE
#define LOG_VERBOSE LOG
//#define LOG_VERBOSE(...)


bool agm::Semaphore::test() throw() {
    return value_;
}

void agm::Semaphore::waitConsume() throw() {
    std::unique_lock<std::mutex> ul(mutex_);
    while (value_ == false) {
        cond_.wait(ul);
    }
    // consume the signal
    value_ = false;
}

void agm::Semaphore::waitPreserve() throw() {
    std::unique_lock<std::mutex> ul(mutex_);
    while (value_ == false) {
        cond_.wait(ul);
    }
    // preserve the signal
    //value_ = false;
}

void agm::Semaphore::signal() throw() {
    /*
    it sure looks weird to grab a lock simply
    to set a boolean.
    but...
    other threads depend on this value not changing
    while they are looking at it.
    so we can't change it while someone else is
    holding the lock.
    */
    {
        std::unique_lock<std::mutex> ul(mutex_);
        value_ = true;
    }
    cond_.notify_all();
}
