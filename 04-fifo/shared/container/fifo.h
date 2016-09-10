/*
Copyright (C) 2012-2016 tim cotter. All rights reserved.
*/

/*
fifo example.
interface

tbd
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
