/*
Copyright (C) 2012-2024 tim cotter. All rights reserved.
*/

/**
it's very strange that c++ does not have a way to get input from std::cin without blocking.
it's even stranger that there's no nice and/or easy way to get the result.
this is the best i could come up with.

use a thread with two semaphores.
between empty and full the input thread owns the data.
between full and empty the consuming thread owns the data.
we assume there is only one consuming thread.
or multiple consuming threads cooperate.
**/

#include <aggiornamento/aggiornamento.h>
#include <aggiornamento/semaphore.h>

#include <thread>


namespace agm {
    class NonBlockingInput {
    public:
        NonBlockingInput() noexcept = default;
        NonBlockingInput(const NonBlockingInput &) = delete;
        ~NonBlockingInput() noexcept = default;

        /**
        public interface.
        get input from standard input.
        or an empty string if none is available.
        **/
        std::string get() noexcept;

        /** private interface. **/
    private:
        static void readInputThread(NonBlockingInput *nbi) noexcept;

        std::string input_;
        std::thread *thread_ = nullptr;
        agm::Semaphore empty_sem_;
        agm::Semaphore full_sem_;
    };
}
