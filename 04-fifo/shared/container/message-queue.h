/*
Copyright (C) 2012-2016 tim cotter. All rights reserved.
*/

/*
fifo example.
message queue interface

build a message queue from 2 fifos.
*/

#pragma once


class MessageQueue {
protected:
    MessageQueue();
public:
    MessageQueue(const MessageQueue &) = delete;
    virtual ~MessageQueue();

    /*
    master thread creates the message queue.
    master thread deletes the double buffer.
    */
    static MessageQueue *create(
        int message_size,  // size of one message
        int max_count      // maximum number of messages
    ) throw();

    /*
    returns message_size passed to the constructor.
    */
    int getMessageSize() throw();

    /*
    returns max_count passed to the constructor.
    */
    int getMaxCount() throw();

    /*
    add an element to the empty fifo.
    returns false if the fifo is full.
    */
    bool putEmpty(char *ptr) throw();

    /*
    get an element from the empty fifo.
    returns nullptr immediately if the fifo is empty.
    */
    char *getEmpty() throw();

    /*
    get an element from the empty fifo.
    waits for an element to be pushed if the fifo is empty.
    */
    char *getEmptyWait() throw();

    /*
    add an element to the message fifo.
    returns false if the fifo is full.
    */
    bool putMessage(char *ptr) throw();

    /*
    get an element from the message fifo.
    returns nullptr immediately if the fifo is empty.
    */
    char *getMessage() throw();

    /*
    get an element from the message fifo.
    waits for an element to be pushed if the fifo is empty.
    */
    char *getMessageWait() throw();
};
