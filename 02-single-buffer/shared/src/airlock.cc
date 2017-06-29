/*
Copyright (C) 2012-2016 tim cotter. All rights reserved.
*/

/**
single buffer example.
airlock container implementation.

assumes single producer, single consumer.

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

note: release must always be called when the buffer is acquired.
for production, you might want to make a fancy c++ class wrapper
that acquires on construction and releases on destruction.

the implementation uses two semaphores.
**/

#include <aggiornamento/aggiornamento.h>
#include <aggiornamento/semaphore.h>
#include <container/airlock.h>

#include <mutex>


// use an anonymous namespace to avoid name collisions at link time.
namespace {
    class AirlockImpl : public Airlock {
    public:
        AirlockImpl() = default;
        AirlockImpl(const AirlockImpl &) = delete;

        virtual ~AirlockImpl() noexcept {
            delete[] data_;
        }

        int size_ = 0;
        char *data_ = nullptr;
        agm::Semaphore sem_[2];
    };
}

Airlock::Airlock() noexcept :
    agm::Container("Airlock") {
}

Airlock::~Airlock() noexcept {
}

Airlock *Airlock::create(
    int size
) noexcept {
    auto impl = new(std::nothrow) AirlockImpl;

    impl->size_ = size;
    impl->data_ = new(std::nothrow) char[size];

    // release side 1 so side 0 can acquire it.
    impl->release(1);

    return impl;
}

/*
returns the size of the buffer.
*/
int Airlock::getSize() noexcept {
    auto impl = (AirlockImpl *) this;
    return impl->size_;
}

/*
get exclusive access to the buffer.
blocks if another thread has exclusive access.
*/
char *Airlock::acquire(
    int side
) noexcept {
    if (side != 0 && side != 1) {
        return nullptr;
    }

    // wait for THIS side.
    auto impl = (AirlockImpl *) this;
    impl->sem_[side].waitConsume();
    return impl->data_;
}

/*
release the buffer so it can be used by other threads
*/
void Airlock::release(
    int side
) noexcept {
    if (side != 0 && side != 1) {
        return;
    }

    // signal the OTHER side.
    auto impl = (AirlockImpl *) this;
    impl->sem_[1-side].signal();
}

/*
unblock threads.
*/
void Airlock::unblock() noexcept {
    release(0);
    release(1);
}
