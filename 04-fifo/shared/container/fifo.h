/*
Copyright (C) 2012-2016 tim cotter. All rights reserved.
*/

/*
fifo example.
interface

assumes multiple producers, multiple consumers.
assumes a maximum fixed size.

first in first out.
put will fail (return false) if the fifo is full.
put will fail if you pass null.
uses separate locks for producers and consumers
to minimize contention.
producers contend with other producers.
consumers contend with other consumers.
producers do not contend with consumers.
and vice versa.
*/

#pragma once


class Fifo {
protected:
    Fifo();
public:
    Fifo(const Fifo &) = delete;
    virtual ~Fifo();

    /*
    master thread creates the fifo.
    master thread deletes the fifo.

    Fifo uses a fixed size array of pointers to elements.
    max_count is the maximum number of elements the fifo can hold.
    */
    static Fifo *create(int max_count) throw();

    /*
    returns max_count passed to the constructor.
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

    /*
    unblock all waiting threads.
    */
    void unblock() throw();
};
