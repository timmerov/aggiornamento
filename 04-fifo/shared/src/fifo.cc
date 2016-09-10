/*
Copyright (C) 2012-2016 tim cotter. All rights reserved.
*/

/*
double buffer example.
implementation

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

the implementation uses two semaphores.
*/

#include <aggiornamento/aggiornamento.h>
#include <aggiornamento/string.h>
#include <aggiornamento/thread.h>
#include <container/fifo.h>

#include <mutex>


// use an anonymous namespace to avoid name collisions at link time.
namespace {
    class DoubleBufferImpl : public DoubleBuffer {
    public:
        DoubleBufferImpl() = default;
        DoubleBufferImpl(const DoubleBufferImpl &) = delete;

        virtual ~DoubleBufferImpl() throw() {
            delete[] data0_;
            delete[] data1_;
        }

        int size_ = 0;
        char *data0_ = nullptr;
        char *data1_ = nullptr;
        agm::Semaphore sem0_;
        agm::Semaphore sem1_;
    };
}

DoubleBuffer::DoubleBuffer() {
}

DoubleBuffer::~DoubleBuffer() {
}

DoubleBuffer *DoubleBuffer::create(
    int size
) throw() {
    auto impl = new(std::nothrow) DoubleBufferImpl;
    impl->size_ = size;
    impl->data0_ = new(std::nothrow) char[size];
    impl->data1_ = new(std::nothrow) char[size];
    return impl;
}

/*
return size of buffer.
*/
int DoubleBuffer::getSize() throw() {
    auto impl = (DoubleBufferImpl *) this;
    return impl->size_;
}

/*
get exclusive access to one of the buffers.
*/
char *DoubleBuffer::acquire(
    int side
) throw() {
    auto impl = (DoubleBufferImpl *) this;
    if (side == 0) {
        return impl->data0_;
    }
    if (side == 1) {
        return impl->data1_;
    }
    return nullptr;
}

/*
swap buffers with the other thread.
*/
char *DoubleBuffer::swap(
    const char *buffer
) throw() {
    /*
    signal this buffer's semaphore.
    wait for the other buffer's semaphore.
    */
    auto impl = (DoubleBufferImpl *) this;
    if (buffer == impl->data0_) {
        impl->sem0_.signal();
        impl->sem1_.waitConsume();
        return impl->data1_;
    }
    if (buffer == impl->data1_) {
        impl->sem1_.signal();
        impl->sem0_.waitConsume();
        return impl->data0_;
    }
    return nullptr;
}
