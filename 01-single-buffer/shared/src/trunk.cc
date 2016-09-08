/*
Copyright (C) 2012-2016 tim cotter. All rights reserved.
*/

/*
single buffer example.
trunk container implementation.
*/

#include <container/trunk.h>


void agm::master::init() throw() {
}

void agm::master::exit() throw() {
}

void agm::trunk::putString(
    const char *buffer
) throw() {
    (void) buffer;
}

void agm::trunk::getString(
    char *buffer,
    int size
) throw() {
    (void) buffer;
    (void) size;
}
