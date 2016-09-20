/*
Copyright (C) 2012-2016 tim cotter. All rights reserved.
*/

/**
semaphore class interface.

binary semaphore.
**/

#pragma once

#include <aggiornamento/aggiornamento.h>

#include <condition_variable>
#include <mutex>


namespace agm {
    class Semaphore {
    public:
        Semaphore() = default;
        Semaphore(const Semaphore &) = delete;
        ~Semaphore() = default;

        bool test() throw();
        void waitConsume() throw();
        void waitPreserve() throw();
        void signal() throw();

    private:
        bool value_ = false;
        std::mutex mutex_;
        std::condition_variable cond_;
    };
}
