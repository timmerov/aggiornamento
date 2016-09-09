/*
Copyright (C) 2012-2016 tim cotter. All rights reserved.
*/

/*
single buffer example.

in this example, alice and bob are threads that can put things
into and take things out of a trunk.
*/

#include <aggiornamento/aggiornamento.h>
#include <aggiornamento/thread.h>
#include <container/trunk.h>


extern agm::Thread *createAlice(Trunk *trunk);
extern agm::Thread *createBob(Trunk *trunk);

int main(
    int argc, char *argv[]
) throw() {
    (void) argc;
    (void) argv;

    // create the containers.
    // use unique_ptr so they're deleted at end of scope.
    std::unique_ptr<Trunk> trunk(Trunk::create());

    // create the threads.
    std::vector<agm::Thread *> threads;
    threads.push_back(createAlice(trunk.get()));
    threads.push_back(createBob(trunk.get()));

    agm::Thread::startAll(threads);
    std::this_thread::sleep_for(std::chrono::seconds(10));
    agm::Thread::stopAll(threads);

    return 0;
}
