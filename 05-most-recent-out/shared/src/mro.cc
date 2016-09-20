/*
Copyright (C) 2012-2016 tim cotter. All rights reserved.
*/

/*
most recent out example.
mro implementation.

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
*/

#include <aggiornamento/aggiornamento.h>
#include <aggiornamento/string.h>
#include <container/mro.h>

#include <condition_variable>
#include <mutex>


// use an anonymous namespace to avoid name collisions at link time.
namespace {
    enum {
        kEmpty,
        kFilling,
        kFull,
        kEmptying
    };

    class MroImpl : public Mro {
    public:
        MroImpl() throw() {
            data_[0] = nullptr;
        }

        MroImpl(const MroImpl &) = delete;

        virtual ~MroImpl() throw() {
            delete[] data_[0];
        }

        int size_ = 0;
        int state_[3];
        char *data_[3];
        std::mutex mutex_;
        std::condition_variable cv_;
    };
}

Mro::Mro() throw() :
    agm::Container("MostRecentOut") {
}

Mro::~Mro() throw() {
}

Mro *Mro::create(
    int size
) throw() {
    auto impl = new(std::nothrow) MroImpl;
    auto stride = (size + 15) / 16 * 16;
    auto size3 = 3*stride;
    impl->size_ = size;
    impl->state_[0] = kEmpty;
    impl->state_[1] = kEmpty;
    impl->state_[2] = kEmpty;
    impl->data_[0] = new(std::nothrow) char[size3];
    impl->data_[1] = impl->data_[0] + stride;
    impl->data_[2] = impl->data_[1] + stride;
    return impl;
}

/*
returns the size of the buffer.
*/
int Mro::getSize() throw() {
    auto impl = (MroImpl *) this;
    return impl->size_;
}

/*
returns an empty buffer.
marks it "filling".
*/
char *Mro::getEmpty() throw() {
    auto impl = (MroImpl *) this;
    char *ptr = nullptr;
    {
        std::unique_lock<std::mutex> lock(impl->mutex_);
        for (auto n = 0; n < 3; ++n) {
            if (impl->state_[n] == kEmpty) {
                impl->state_[n] = kFilling;
                ptr = impl->data_[n];
                break;
            }
        }
    }
    return ptr;
}

/*
full buffers are marked empty.
changes filling buffers to full.
signals the consumer thread.
*/
void Mro::putFull() throw() {
    auto impl = (MroImpl *) this;
    {
        std::unique_lock<std::mutex> lock(impl->mutex_);
        for (auto n = 0; n < 3; ++n) {
            auto state = impl->state_[n];
            if (state == kFull) {
                impl->state_[n] = kEmpty;
            }
            if (state == kFilling) {
                impl->state_[n] = kFull;
            }
        }
    }
    impl->cv_.notify_one();
}

/*
gets a full buffer.
marks it "emptying".
returns null if no buffer is full.
*/
char *Mro::getFull() throw() {
    auto impl = (MroImpl *) this;
    char *ptr = nullptr;
    {
        std::unique_lock<std::mutex> lock(impl->mutex_);
        for (auto n = 0; n < 3; ++n) {
            if (impl->state_[n] == kFull) {
                impl->state_[n] = kEmptying;
                ptr = impl->data_[n];
                break;
            }
        }
    }
    return ptr;
}

/*
gets a full buffer.
marks it "emptying".
blocks if no buffer is full until
a full buffer is put.
*/
char *Mro::getFullWait() throw() {
    auto impl = (MroImpl *) this;
    char *ptr = nullptr;
    {
        std::unique_lock<std::mutex> lock(impl->mutex_);
        for(;;) {
            for (auto n = 0; n < 3; ++n) {
                if (impl->state_[n] == kFull) {
                    impl->state_[n] = kEmptying;
                    ptr = impl->data_[n];
                    break;
                }
            }
            if (ptr) {
                break;
            }
            impl->cv_.wait(lock);
        }
    }
    return ptr;
}

/*
changes emptying buffers to empty.
*/
void Mro::putEmpty() throw() {
    auto impl = (MroImpl *) this;
    {
        std::unique_lock<std::mutex> lock(impl->mutex_);
        for (auto n = 0; n < 3; ++n) {
            if (impl->state_[n] == kEmptying) {
                impl->state_[n] = kEmpty;
            }
        }
    }
}
