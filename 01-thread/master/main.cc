/*
Copyright (C) 2012-2016 tim cotter. All rights reserved.
*/

/**
simple thread example.

alice is a thread that counts for 10 seconds.
**/

#include <aggiornamento/aggiornamento.h>
#include <aggiornamento/log.h>
#include <aggiornamento/thread.h>


extern agm::Thread *createAlice();

int main(
    int argc, char *argv[]
) throw() {
    (void) argc;
    (void) argv;

    agm::log::init(AGM_TARGET_NAME ".log");

    // create the thread.
    std::vector<agm::Thread *> threads;
    threads.push_back(createAlice());

    // run the thread until it's done.
    agm::Thread::runAll(threads);

    return 0;
}
