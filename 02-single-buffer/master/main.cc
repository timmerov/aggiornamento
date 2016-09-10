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
#include <aggiornamento/thread.h>
#include <container/airlock.h>
#include <container/trunk.h>


extern agm::Thread *createAlice(Airlock *airlock, Trunk *trunk);
extern agm::Thread *createBob(Airlock *airlock, Trunk *trunk);

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

    // use unique_ptr so they're deleted at end of scope.
    std::unique_ptr<Airlock> auto_0(airlock);
    std::unique_ptr<Trunk> auto_1(trunk);

    // create the threads.
    std::vector<agm::Thread *> threads;
    threads.push_back(createAlice(airlock, trunk));
    threads.push_back(createBob(airlock, trunk));

    // run the threads.
    agm::Thread::runAll(threads);

    return 0;
}
