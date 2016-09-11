/*
Copyright (C) 2012-2016 tim cotter. All rights reserved.
*/

/*
fifo example.
interface

tbd
*/

#pragma once


class Fifo {
protected:
    Fifo();
public:
    Fifo(const Fifo &) = delete;
    virtual ~Fifo();

    /*
    master thread creates the double buffer.
    master thread deletes the double buffer.

    Fifo uses a fixed size array of pointers to elements.
    maxCount is the maximum number of elements the fifo can hold.
    */
    static Fifo *create(int maxCount) throw();

    /*
    returns returns maxCount passed to the constructor.
    */
    int getMaxCount() throw();

    /*
    add an element to the fifo.
    returns false if the fifo is full.
    */
    bool put(char *ptr) throw();

    /*
    get an element from the fifo.
    returns nullptr immediately if the fifo is empty.
    */
    char *get() throw();

    /*
    get an element from the fifo.
    waits for an element to be pushed if the fifo is empty.
    */
    char *getWait() throw();
};
