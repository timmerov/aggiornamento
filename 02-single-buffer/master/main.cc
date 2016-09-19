/*
Copyright (C) 2012-2016 tim cotter. All rights reserved.
*/

/**
single buffer example.

this example has two parts.
alica and bob are thread that use different types of single buffers.
in the first part, they put things into and take things out of a trunk.
in the second part, the put things into and take things out of an airlock.
**/

#include <aggiornamento/aggiornamento.h>
#include <aggiornamento/log.h>
#include <aggiornamento/master.h>
#include <aggiornamento/thread2.h>
#include <container/airlock.h>
#include <container/trunk.h>


extern agm::Thread2 *createAlice(Airlock *airlock, Trunk *trunk);
extern agm::Thread2 *createBob(Airlock *airlock, Trunk *trunk);

// use anonymous namespace to avoid collisions at link time.
namespace {
    static const int kBufferSize = 100;
}

int main(
    int argc, char *argv[]
) throw() {
    (void) argc;
    (void) argv;

    agm::log::init(AGM_TARGET_NAME ".log");

    // create the containers.
    auto airlock = Airlock::create(kBufferSize);
    auto trunk = Trunk::create(kBufferSize);

    // store them in a vector.
    std::vector<agm::Container *> containers;
    containers.push_back(airlock);
    containers.push_back(trunk);

    // create the threads.
    std::vector<agm::Thread2 *> threads;
    threads.push_back(createAlice(airlock, trunk));
    threads.push_back(createBob(airlock, trunk));

    // run the threads.
    agm::Thread2::startAll(threads, containers);
    agm::master::waitDone();
    agm::Thread2::stopAll(threads, containers);

    return 0;
}
