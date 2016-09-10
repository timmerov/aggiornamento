/*
Copyright (C) 2012-2016 tim cotter. All rights reserved.
*/

/*
double buffer example.
interface

assumes single producer, single consumer.

the contents of the buffers are protected by two semaphores.
two threads (0 and 1) each acquire a different buffer.
they fill their respective buffer.
then swap buffers.
the first to swap blocks until the second swaps.

thread 0        thread 1
--------        --------
acquire 0       acquire 1
copy to buffer  copy to buffer
delay           delay
swap
                swap
copy to buffer  copy to buffer
delay           delay
                swap
swap
etc.
*/

#pragma once


class DoubleBuffer {
protected:
    DoubleBuffer();
public:
    DoubleBuffer(const DoubleBuffer &) = delete;
    virtual ~DoubleBuffer();

    /*
    master thread creates the double buffer.
    master thread deletes the double buffer.
    */
    static DoubleBuffer *create(int size) throw();

    /*
    returns the size of a buffer.
    */
    int getSize() throw();

    /*
    get exclusive access to one of the buffers.
    */
    char *acquire(int side) throw();

    /*
    swap buffers with the other thread.
    */
    char *swap(const char *buffer) throw();
};
