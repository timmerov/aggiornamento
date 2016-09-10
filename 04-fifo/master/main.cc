/*
Copyright (C) 2012-2016 tim cotter. All rights reserved.
*/

/**
fifo example.

tbd
**/

#include <aggiornamento/aggiornamento.h>
#include <aggiornamento/log.h>
#include <aggiornamento/thread.h>
#include <container/fifo.h>


extern agm::Thread *createAlice(DoubleBuffer *db);
extern agm::Thread *createBob(DoubleBuffer *db);

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
    auto db = DoubleBuffer::create(kBufferSize);

    // use unique_ptr so they're deleted at end of scope.
    std::unique_ptr<DoubleBuffer> auto_0(db);

    // create the threads.
    std::vector<agm::Thread *> threads;
    threads.push_back(createAlice(db));
    threads.push_back(createBob(db));

    // run the threads.
    agm::Thread::runAll(threads);

    return 0;
}
