/*
Copyright (C) 2012-2016 tim cotter. All rights reserved.
*/

/**
simple thread example.

alice is a thread that counts for 10 seconds.
**/

#include <aggiornamento/aggiornamento.h>
#include <aggiornamento/log.h>
#include <aggiornamento/thread2.h>


extern agm::Thread2 *createAlice();

int main(
    int argc, char *argv[]
) throw() {
    (void) argc;
    (void) argv;

    agm::log::init(AGM_TARGET_NAME ".log");

    // no containers
    std::vector<agm::Container *> containers;

    // create the thread.
    std::vector<agm::Thread2 *> threads;
    threads.push_back(createAlice());

    // run the thread for 3 seconds.
    agm::Thread2::startAll(threads, containers);
    agm::sleep::seconds(3);
    agm::Thread2::stopAll(threads, containers);

    return 0;
}
