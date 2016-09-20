/*
Copyright (C) 2012-2016 tim cotter. All rights reserved.
*/

/*
write once read many example.
worm interface.

tbd
*/

#pragma once

#include <aggiornamento/container.h>


class Worm : public agm::Container {
protected:
    Worm() throw();
public:
    Worm(const Worm &) = delete;
    virtual ~Worm() throw();

    /*
    master thread creates the mro.
    master thread deletes the mro.
    */
    static Worm *create(int size) throw();

    /*
    returns the size of the buffer.
    */
    int getSize() throw();

    /*
    returns an empty buffer.
    marks it "filling".
    */
    char *getEmpty() throw();

    /*
    full buffers are marked empty.
    changes filling buffers to full.
    signals the consumer thread.
    */
    void putFull() throw();

    /*
    gets a full buffer.
    marks it "emptying".
    returns null if no buffer is full.
    */
    char *getFull() throw();

    /*
    gets a full buffer.
    marks it "emptying".
    blocks if no buffer is full until
    a full buffer is put.
    */
    char *getFullWait() throw();

    /*
    changes emptying buffers to empty.
    */
    void putEmpty() throw();
};
