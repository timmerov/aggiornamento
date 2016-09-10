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
#include <container/double-buffer.h>


extern agm::Thread *createAlice(DoubleBuffer *db);
extern agm::Thread *createBob(DoubleBuffer *db);

int main(
    int argc, char *argv[]
) throw() {
    (void) argc;
    (void) argv;

    agm::log::init(AGM_TARGET_NAME ".log");

    // create the containers.
    auto db = DoubleBuffer::create();

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
