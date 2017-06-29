/*
Copyright (C) 2012-2016 tim cotter. All rights reserved.
*/

/**
write once-in-a-while read many example.
worm interface.

assumes single writer, multiple simultaneous readers.
assumes writer writes rarely.

protecting the data in this case causes massive contention.
so we don't.
the data is constant the vast majority of the time.
the strategy is to...
- simply read the data.
- detect the possibility of corrupt data.
- retry if necessary.

writer usage:
char *ptr = getWriteBuffer();
memcpy(ptr, data, sizeof(data));
swap();

reader usage:
int state;
for(;;) {
    state = getStartState();
    const char *ptr = getReadBuffer(state);
    memcpy(data, ptr, sizeof(data));
} while (checkState(state));
**/

#pragma once

#include <aggiornamento/container.h>


class Worm : public agm::Container {
protected:
    Worm() noexcept;
public:
    Worm(const Worm &) = delete;
    virtual ~Worm() noexcept;

    /*
    master thread creates the worm.
    master thread deletes the worm.
    */
    static Worm *create(int size) noexcept;

    /*
    returns the size of the buffer.
    */
    int getSize() noexcept;

    /*
    returns the write buffer.
    */
    char *getWriteBuffer() noexcept;

    /*
    swap the read/write buffers and update the state.
    */
    void swap() noexcept;

    /*
    returns the state of the buffers.
    required to get the read buffer
    and to detect read failures.
    */
    int getState() noexcept;

    /*
    returns true if the current state is the same.
    returns false if the current state changed.
    */
    bool checkState(int state) noexcept;

    /*
    returns the read buffer.
    */
    const char *getReadBuffer(int state) noexcept;
};
