/*
Copyright (C) 2012-2016 tim cotter. All rights reserved.
*/

/**
single buffer example.

in this example, alice and bob are threads that can put things
into and take things out of a trunk.
**/

#include <aggiornamento/aggiornamento.h>
#include <aggiornamento/log.h>
#include <aggiornamento/thread.h>
#include <container/trunk.h>


extern agm::Thread *createAlice(Trunk *trunk);
extern agm::Thread *createBob(Trunk *trunk);

int main(
    int argc, char *argv[]
) throw() {
    (void) argc;
    (void) argv;

    agm::log::init(AGM_TARGET_NAME ".log");

    // create the containers.
    // use unique_ptr so they're deleted at end of scope.
    auto trunk = Trunk::create();
    std::unique_ptr<Trunk> auto_trunk(trunk);

    // create the threads.
    std::vector<agm::Thread *> threads;
    threads.push_back(createAlice(trunk));
    threads.push_back(createBob(trunk));

    // run the threads.
    agm::Thread::runAll(threads);

    return 0;
}
