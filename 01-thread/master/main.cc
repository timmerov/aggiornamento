/*
Copyright (C) 2012-2016 tim cotter. All rights reserved.
*/

/**
simple thread example.

alice is a thread that counts seconds.
run it for 3 seconds.
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

    // no containers
    std::vector<agm::Container *> containers;

    // create the thread.
    std::vector<agm::Thread *> threads;
    threads.push_back(createAlice());

    // run the thread for 3 seconds.
    agm::Thread::startAll(threads, containers);
    agm::sleep::seconds(3);
    agm::Thread::stopAll(threads, containers);

    return 0;
}
