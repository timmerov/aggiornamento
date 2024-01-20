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

#include <iostream>

#include <aggiornamento/aggiornamento.h>
#include <aggiornamento/nonblocking_input.h>


std::string agm::NonBlockingInput::get() noexcept {
    /** start the thread if necessary. **/
    if (thread_ == nullptr) {
        thread_ = new std::thread(readInputThread, this);
        empty_sem_.signal();
    }

    /** check if the thread has input. **/
    std::string result;
    if (full_sem_.test()) {
        result = std::move(input_);
        full_sem_.waitConsume();
        empty_sem_.signal();
    }
    return result;
}

void agm::NonBlockingInput::readInputThread(
    NonBlockingInput *nbi
) noexcept {
    for(;;) {
        /** wait for the caller to consume the old input. **/
        nbi->empty_sem_.waitConsume();

        /** get new input. **/
        std::cin >> nbi->input_;

        /** tell the caller there's new input. **/
        nbi->full_sem_.signal();
    }
}
