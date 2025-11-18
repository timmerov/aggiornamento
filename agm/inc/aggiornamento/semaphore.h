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

        /** true if signaled. **/
        bool test() noexcept;

        /** waits forever for signal. consumes it. **/
        void waitConsume() noexcept;

        /** waits forever for signal. preserves it for other threads. **/
        void waitPreserve() noexcept;

        /** true if times out. waits for signal. consumes it. **/
        bool waitForConsume(int timeout_ms) noexcept;

        /** true if times out. waits for signal. preserves it. **/
        bool waitForPreserve(int timeout_ms) noexcept;

        /** sets the signal. **/
        void signal() noexcept;

    private:
        bool value_ = false;
        std::mutex mutex_;
        std::condition_variable cond_;
    };
}
