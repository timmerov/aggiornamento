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
#include <container/airlock.h>

#include <mutex>


// use an anonymous namespace to avoid name collisions at link time.
namespace {
    class AirlockImpl : public Airlock {
    public:
        AirlockImpl() = default;
        AirlockImpl(const AirlockImpl &) = delete;
        virtual ~AirlockImpl() = default;

        agm::Semaphore sem_[2];
        char data_[kMaxBufferSize];
    };
}

Airlock::Airlock() {
}

Airlock::~Airlock() {
}

Airlock *Airlock::create() throw() {
    auto impl = new(std::nothrow) AirlockImpl;

    // release side 1 so side 0 can acquire it.
    impl->release(1);

    return impl;
}

/*
get exclusive access to the buffer.
blocks if another thread has exclusive access.
*/
char *Airlock::acquire(
    int side
) throw() {
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
) throw() {
    if (side != 0 && side != 1) {
        return;
    }

    // signal the OTHER side.
    auto impl = (AirlockImpl *) this;
    impl->sem_[1-side].signal();
}
