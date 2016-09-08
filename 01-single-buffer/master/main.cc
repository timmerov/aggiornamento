/*
Copyright (C) 2012-2016 tim cotter. All rights reserved.
*/

/*
single buffer example.

in this example, alice and bob are threads that can put things
into and take things out of a trunk.
*/

#include <aggiornamento/aggiornamento.h>
#include <container/trunk.h>


extern agm::Thread *createAlice(Trunk *trunk);
extern agm::Thread *createBob(Trunk *trunk);

int main(
    int argc, char *argv[]
) throw() {
    (void) argc;
    (void) argv;

    auto trunk = Trunk::create();
    auto alice = createAlice(trunk);
    auto bob = createBob(trunk);

    alice->start();
    bob->start();
    alice->stopProducing();
    bob->stopProducing();
    alice->stopCompletely();
    bob->stopCompletely();

    return 0;
}
