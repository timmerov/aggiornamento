/*
Copyright (C) 2012-2016 tim cotter. All rights reserved.
*/

/**
most recent out example.

tk
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

    // run the threads.
    agm::Thread::runAll(threads);

    return 0;
}
