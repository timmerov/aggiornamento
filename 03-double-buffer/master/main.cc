/*
Copyright (C) 2012-2016 tim cotter. All rights reserved.
*/

/**
double buffer example.

two threads alice and bob.
they each acquire one of the double buffer buffers.
they each write data to their own buffer.
then they swap buffers.
and print what the other put into the buffer.

also demonstrates how to unblock a thread.
**/

#include <aggiornamento/aggiornamento.h>
#include <aggiornamento/log.h>
#include <aggiornamento/thread.h>
#include <container/double-buffer.h>


extern agm::Thread *createAlice(DoubleBuffer *db);
extern agm::Thread *createBob(DoubleBuffer *db);

// use anonymous namespace to avoid collisions at link time.
namespace {
    static const int kBufferSize = 100;
}

int main(
    int argc, char *argv[]
) noexcept {
    (void) argc;
    (void) argv;

    agm::log::init(AGM_TARGET_NAME ".log");

    // create the containers.
    auto db = DoubleBuffer::create(kBufferSize);

    // store the containers in a vector.
    std::vector<agm::Container *> containers;
    containers.push_back(db);

    // create the threads.
    std::vector<agm::Thread *> threads;
    threads.push_back(createAlice(db));
    threads.push_back(createBob(db));

    // run the threads.
    agm::Thread::runAll(threads, containers);

    return 0;
}
