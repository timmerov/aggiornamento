/*
Copyright (C) 2012-2016 tim cotter. All rights reserved.
*/

/**
fifo example.
message queue interface

build a message queue from 2 fifos.

usage:
thread 0        thread 1
--------        --------
getEmpty        getMessageWait
fill it
putMessage
                read message
                putEmpty
**/

#pragma once

#include <aggiornamento/container.h>


class MessageQueue : public agm::Container {
protected:
    MessageQueue() noexcept;
public:
    MessageQueue(const MessageQueue &) = delete;
    virtual ~MessageQueue() noexcept;

    /*
    master thread creates the message queue.
    master thread deletes the double buffer.
    */
    static MessageQueue *create(
        int message_size,  // size of one message
        int max_count      // maximum number of messages
    ) noexcept;

    /*
    returns message_size passed to the constructor.
    */
    int getMessageSize() noexcept;

    /*
    returns max_count passed to the constructor.
    */
    int getMaxCount() noexcept;

    /*
    add an element to the empty fifo.
    returns false if the fifo is full.
    */
    bool putEmpty(char *ptr) noexcept;

    /*
    get an element from the empty fifo.
    returns nullptr immediately if the fifo is empty.
    */
    char *getEmpty() noexcept;

    /*
    get an element from the empty fifo.
    waits for an element to be pushed if the fifo is empty.
    */
    char *getEmptyWait() noexcept;

    /*
    add an element to the message fifo.
    returns false if the fifo is full.
    */
    bool putMessage(char *ptr) noexcept;

    /*
    get an element from the message fifo.
    returns nullptr immediately if the fifo is empty.
    */
    char *getMessage() noexcept;

    /*
    get an element from the message fifo.
    waits for an element to be pushed if the fifo is empty.
    */
    char *getMessageWait() noexcept;

    /*
    unblock all waiting threads.
    */
    virtual void unblock() noexcept;
};
