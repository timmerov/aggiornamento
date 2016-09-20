/*
Copyright (C) 2012-2016 tim cotter. All rights reserved.
*/

/**
write once read many example.

tk
**/

#include <aggiornamento/aggiornamento.h>
#include <aggiornamento/log.h>
#include <aggiornamento/thread.h>
#include <container/worm.h>


extern agm::Thread *createAlice(Worm *worm);
extern agm::Thread *createBob(Worm *worm);

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
    auto worm = Worm::create(kBufferSize);

    // store containers in a vector.
    std::vector<agm::Container *> containers;
    containers.push_back(worm);

    // create the threads.
    std::vector<agm::Thread *> threads;
    threads.push_back(createAlice(worm));
    threads.push_back(createBob(worm));

    // run the threads for 10 seconds.
    agm::Thread::startAll(threads, containers);
    agm::sleep::seconds(10);
    agm::Thread::stopAll(threads, containers);

    return 0;
}
