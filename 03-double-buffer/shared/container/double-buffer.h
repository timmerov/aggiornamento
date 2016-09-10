/*
Copyright (C) 2012-2016 tim cotter. All rights reserved.
*/

/*
single buffer example.
airlock interface.

the contents of the buffer is protected by two semaphores.
two threads (0 and 1) alternate ownership of the buffer.
both threads attempt to acquire the buffer at start.
thread 0 will succeed.
thread 1 will be blocked.
thread 0 fills the buffer and releases it.
this unblocks thread 1.
repeat.

thread 0        thread 1
--------        --------
acquire 0       acquire 1
copy to buffer
delay
release 0
                copy to buffer
                delay
                release 1
acquire 0
*/

#pragma once


class DoubleBuffer {
protected:
    DoubleBuffer();
public:
    DoubleBuffer(const DoubleBuffer &) = delete;
    virtual ~DoubleBuffer();

    /*
    master thread creates the airlock.
    master thread deletes the airlock.
    */
    static DoubleBuffer *create() throw();

    /*
    the string in the airlock is at most this big.
    */
    static const int kMaxBufferSize = 100;

    /*
    get exclusive access to the buffer.
    blocks if another thread has exclusive access.
    not re-entrant.
    */
    char *acquire(int side) throw();

    /*
    release the buffer so it can be used by other threads
    */
    void release(int side) throw();
};
