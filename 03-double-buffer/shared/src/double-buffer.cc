/*
Copyright (C) 2012-2016 tim cotter. All rights reserved.
*/

/**
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
**/

#include <aggiornamento/aggiornamento.h>
#include <aggiornamento/thread.h>
#include <container/double-buffer.h>

#include <mutex>


// use an anonymous namespace to avoid name collisions at link time.
namespace {
    class DoubleBufferImpl : public DoubleBuffer {
    public:
        DoubleBufferImpl() = default;
        DoubleBufferImpl(const DoubleBufferImpl &) = delete;

        virtual ~DoubleBufferImpl() noexcept {
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

DoubleBuffer::DoubleBuffer() noexcept :
    agm::Container("DoubleBuffer") {
}

DoubleBuffer::~DoubleBuffer() noexcept {
}

DoubleBuffer *DoubleBuffer::create(
    int size
) noexcept {
    auto impl = new(std::nothrow) DoubleBufferImpl;
    impl->size_ = size;
    impl->data0_ = new(std::nothrow) char[size];
    impl->data1_ = new(std::nothrow) char[size];
    return impl;
}

/*
return size of buffer.
*/
int DoubleBuffer::getSize() noexcept {
    auto impl = (DoubleBufferImpl *) this;
    return impl->size_;
}

/*
get exclusive access to one of the buffers.
*/
char *DoubleBuffer::acquire(
    int side
) noexcept {
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
) noexcept {
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

/*
unblock both threads as if the other thread
called swap.
there's no way for swap to know if it was
called normally or if it returned because
it was unblocked.
the caller will need to make that determination
some other way.
*/
void DoubleBuffer::unblock() noexcept {
    /*
    signal both semaphores to unblock both threads.
    */
    auto impl = (DoubleBufferImpl *) this;
    impl->sem0_.signal();
    impl->sem1_.signal();
}
