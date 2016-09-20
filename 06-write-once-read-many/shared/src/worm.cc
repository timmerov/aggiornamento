/*
Copyright (C) 2012-2016 tim cotter. All rights reserved.
*/

/**
write once-in-a-while read many example.
worm implementation.

assumes single writer, multiple simultaneous readers.
assumes writer writes rarely.

protecting the data in this case causes massive contention.
so we don't.
the data is constant the vast majority of the time.
the strategy is to...
- simply read the data.
- detect the possibility of corrupt data.
- retry if necessary.

writer usage:
char *ptr = getWriteBuffer();
memcpy(ptr, data, sizeof(data));
swap();

reader usage:
int state;
for(;;) {
    state = getStartState();
    const char *ptr = getReadBuffer(state);
    memcpy(data, ptr, sizeof(data));
} while (checkState(state));

caution:
out of order execution can wreak havoc.
out of order memory flush can wreak havoc.

state is incremented every write.
the low order bit identifies which buffer is which.
the LOB is the index of the read buffer.
1-LOB is the index of the write buffer.
**/

#include <aggiornamento/aggiornamento.h>
#include <container/worm.h>

#include <atomic>


// use an anonymous namespace to avoid name collisions at link time.
namespace {
    class WormImpl : public Worm {
    public:
        WormImpl() throw() {
            data_[0] = nullptr;
        }

        WormImpl(const WormImpl &) = delete;

        virtual ~WormImpl() throw() {
            delete[] data_[0];
        }

        int size_ = 0;
        int volatile state_ = 0;
        char *data_[2];
    };
}

Worm::Worm() throw() :
    agm::Container("WriteOnceReadMany") {
}

Worm::~Worm() {
}

/*
master thread creates the worm.
master thread deletes the worm.
*/
Worm *Worm::create(
    int size
) throw() {
    auto impl = new(std::nothrow) WormImpl;
    auto stride = (size + 15) / 16 * 16;
    auto size2 = 2*stride;
    impl->size_ = size;
    impl->data_[0] = new(std::nothrow) char[size2];
    impl->data_[1] = impl->data_[0] + stride;
    return impl;
}

/*
returns the size of the buffer.
*/
int Worm::getSize() throw() {
    auto impl = (WormImpl *) this;
    return impl->size_;
}

/*
returns the write buffer.
*/
char *Worm::getWriteBuffer() throw() {
    auto impl = (WormImpl *) this;

    // return the write buffer.
    auto idx = 1 - (impl->state_ & 1);
    auto ptr = impl->data_[idx];
    return ptr;
}

/*
swap the read/write buffers and update the state.
*/
void Worm::swap() throw() {
    auto impl = (WormImpl *) this;

    /*
    a release fence forces completion of all pending
    reads and writes before the next write.

    in this case, the writer's new data is written
    before the state is changed.
    */
    atomic_thread_fence(std::memory_order_release);
    ++impl->state_;
}

/*
returns the state of the buffers.
required to get the read buffer
and to detect read failures.
*/
int Worm::getState() throw() {
    auto impl = (WormImpl *) this;

    /*
    an acquire fence forces completion of all pending
    reads before the next read or write.

    in this case, the state is read before the the
    reader reads the data.
    */
    auto state = impl->state_;
    atomic_thread_fence(std::memory_order_acquire);
    return state;
}

/*
returns true if the current state is the same.
returns false if the current state changed.
*/
bool Worm::checkState(
    int start_state
) throw() {
    auto impl = (WormImpl *) this;

    /*
    an acquire fence forces completion of all pending
    reads before the next read or write.

    in this case, the reader's reads are completed
    before the state is re-read and tested.
    */
    atomic_thread_fence(std::memory_order_acquire);
    auto end_state = impl->state_;
    return (start_state == end_state);
}

/*
returns the read buffer.
*/
const char *Worm::getReadBuffer(
    int state
) throw() {
    auto impl = (WormImpl *) this;

    // use the state passed to us to
    // determine the read buffer.
    auto idx = state & 1;
    auto ptr = impl->data_[idx];
    return ptr;
}
