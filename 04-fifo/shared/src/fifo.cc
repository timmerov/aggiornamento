/*
Copyright (C) 2012-2016 tim cotter. All rights reserved.
*/

/*
fifo example.
implementation

tbd.
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
