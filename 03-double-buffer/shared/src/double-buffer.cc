/*
Copyright (C) 2012-2016 tim cotter. All rights reserved.
*/

/*
single buffer example.
airlock container implementation.

anyone can open the airlock...
put stuff in or take stuff out...
then close the airlock.
which makes it available for the next person.

the implementation uses two semaphores.
*/

#include <aggiornamento/aggiornamento.h>
#include <aggiornamento/string.h>
#include <aggiornamento/thread.h>
#include <container/double-buffer.h>

#include <mutex>


// use an anonymous namespace to avoid name collisions at link time.
namespace {
    class DoubleBufferImpl : public DoubleBuffer {
    public:
        DoubleBufferImpl() = default;
        DoubleBufferImpl(const DoubleBufferImpl &) = delete;
        virtual ~DoubleBufferImpl() = default;

        agm::Semaphore sem_[2];
        char data_[kMaxBufferSize];
    };
}

DoubleBuffer::DoubleBuffer() {
}

DoubleBuffer::~DoubleBuffer() {
}

DoubleBuffer *DoubleBuffer::create() throw() {
    auto impl = new(std::nothrow) DoubleBufferImpl;

    // release side 1 so side 0 can acquire it.
    impl->release(1);

    return impl;
}

/*
get exclusive access to the buffer.
blocks if another thread has exclusive access.
*/
char *DoubleBuffer::acquire(
    int side
) throw() {
    if (side != 0 && side != 1) {
        return nullptr;
    }

    // wait for THIS side.
    auto impl = (DoubleBufferImpl *) this;
    impl->sem_[side].waitConsume();
    return impl->data_;
}

/*
release the buffer so it can be used by other threads
*/
void DoubleBuffer::release(
    int side
) throw() {
    if (side != 0 && side != 1) {
        return;
    }

    // signal the OTHER side.
    auto impl = (DoubleBufferImpl *) this;
    impl->sem_[1-side].signal();
}
