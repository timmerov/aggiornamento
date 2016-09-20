/*
Copyright (C) 2012-2016 tim cotter. All rights reserved.
*/

/**
most recent out example.
mro interface.

assumes: single producer, single consumer.

long ago this was called a triple buffer.
however that term is now confused with a fifo with
three buffers.
cause microsoft.
sigh.
so now i use the term most-recent-out buffer.
which uses three buffers.

unlike a fifo, data is not guaranteed to be delivered.
unlike a fifo, put cannot fail and cannot block the producer.
like a fifo, get optionally blocks the consumer.

producer thread and consumer thread.
the producer produces data and cannot be blocked.
it must always have somewhere to put the latest data.
the consumer consumes data as soon as it is available.

if the consumer is consuming data faster than the producer
is producing it, the consumer may optionally block waiting
for the producer to produce data.

if the producer is producing data faster than the consumer
can consume it, the not-latest data is discarded.

the producer gets empty buffers.
and puts full buffers.
neither of these operations block the producer.

the consumer gets full buffers.
the consumer puts empty buffers.
get optionally blocks.
put never blocks.

real world use case:
suppose you're decoding video at 60 fps.
but playing it back at 24 fps.
you can't block the decoder or it will fall behind.
you must decode every frame.
therefore you must discard some of the decoded frames.
the easy way is to put all frames into an mro.
**/

#pragma once

#include <aggiornamento/container.h>


class Mro : public agm::Container {
protected:
    Mro() throw();
public:
    Mro(const Mro &) = delete;
    virtual ~Mro() throw();

    /*
    master thread creates the mro.
    master thread deletes the mro.
    */
    static Mro *create(int size) throw();

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
