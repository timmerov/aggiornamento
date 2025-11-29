/*
Copyright (C) 2012-2025 tim cotter. All rights reserved.
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

we use the gnu readline library.
which is really nice.
it handles special characters.
and adding a command history is pretty easy.

BUT....

there's still no way to unblock the readline from another thread.
sigh.
so we still have to use the quick_exit(0) hack.

BUT...

readline takes over the terminal and changes tty settings.
which persist after the program exits.
argh!!!

so we capture the tty attributes on construction.
and we restore them on destruction.
**/

#include <iostream>
#include <readline/readline.h>
#include <readline/history.h>
#include <termios.h>
#include <unistd.h>

#include <aggiornamento/aggiornamento.h>
#include <aggiornamento/nonblocking_input.h>


namespace {
static struct termios original_termios;
}

agm::NonBlockingInput::NonBlockingInput() noexcept {
    tcgetattr(STDIN_FILENO, &original_termios);
}

agm::NonBlockingInput::~NonBlockingInput() noexcept {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &original_termios);
}

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

        /** get new input using gnu readline library. **/
        char *line = readline(nullptr);
        if (line && line[0]) {
            /** append to the input with a closing end line. **/
            nbi->input_ += line;
            nbi->input_ += '\n';
            /** add it to the history. **/
            add_history(line);
        }
        if (line) {
            free(line);
        }

        /** tell the caller there's new input. **/
        nbi->full_sem_.signal();
    }
    //el_deprep();
}
