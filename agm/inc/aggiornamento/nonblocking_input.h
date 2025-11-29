/*
Copyright (C) 2012-2025 tim cotter. All rights reserved.
*/

/**
NOTE: non-blecking input is a bit of a misnomer.
it creates a thread that is immortal.

only one of these objects should ever be created.

it's very strange that c++ does not have a way to get input from std::cin without blocking.
it's even stranger that there's no nice and/or easy way to get the result.
this is the best i could come up with.
**/

#include <aggiornamento/aggiornamento.h>
#include <aggiornamento/semaphore.h>

#include <thread>


namespace agm {
    class NonBlockingInput {
    public:
        NonBlockingInput() noexcept;
        NonBlockingInput(const NonBlockingInput &) = delete;
        ~NonBlockingInput() noexcept;

        /**
        public interface.
        get input from standard input.
        or an empty string if none is available.
        there is no delay.
        advise against spin-looping here.
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
