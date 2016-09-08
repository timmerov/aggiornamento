/*
Copyright (C) 2012-2016 tim cotter. All rights reserved.
*/

/*
single buffer example.

in this example, alice and bob are threads that can put things
into and take things out of a trunk.
*/

#include <aggiornamento/aggiornamento.h>


int main(
    int argc, char *argv[]
) throw() {
    (void) argc;
    (void) argv;

    LOG("Hello, World!");
    LOG("Goodbye, World!");

    return 0;
}
