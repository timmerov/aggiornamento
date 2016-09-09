/*
Copyright (C) 2012-2016 tim cotter. All rights reserved.
*/

/**
simple thread example.

alice is a thread that counts for 10 seconds.
**/

#include <aggiornamento/aggiornamento.h>
#include <aggiornamento/thread.h>


extern agm::Thread *createAlice();

int main(
    int argc, char *argv[]
) throw() {
    (void) argc;
    (void) argv;

    // create the thread.
    std::vector<agm::Thread *> threads;
    threads.push_back(createAlice());

    // run the thread for 10 seconds.
    agm::Thread::startAll(threads);
    std::this_thread::sleep_for(std::chrono::seconds(10));
    agm::Thread::stopAll(threads);

    return 0;
}
