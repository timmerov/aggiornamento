/*
Copyright (C) 2012-2016 tim cotter. All rights reserved.
*/

/**
most recent out example.

the alice thread writes the count as a string to the mro buffer 2.5 times per second.
the bob thread pulls the count string from the mro buffer 1.0 times per second.
neither the alice thread nor the bob thread ever block waiting for the other.
**/

#include <aggiornamento/aggiornamento.h>
#include <aggiornamento/log.h>
#include <aggiornamento/thread.h>
#include <container/mro.h>


extern agm::Thread *createAlice(Mro *mro);
extern agm::Thread *createBob(Mro *mro);

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
    auto mro = Mro::create(kBufferSize);

    // use unique_ptr so they're deleted at end of scope.
    std::unique_ptr<Mro> auto_0(mro);

    // create the threads.
    std::vector<agm::Thread *> threads;
    threads.push_back(createAlice(mro));
    threads.push_back(createBob(mro));

    // run the threads for 10 seconds.
    agm::Thread::startAll(threads);
    agm::sleep::seconds(10);
    agm::Thread::stopAll(threads);

    return 0;
}
