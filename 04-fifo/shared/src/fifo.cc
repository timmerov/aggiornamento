/*
Copyright (C) 2012-2016 tim cotter. All rights reserved.
*/

/**
fifo example.
implementation

assumes multiple producers, multiple consumers.
assumes a maximum fixed size.

first in first out.
put will fail (return false) if the fifo is full.
put will fail if you pass null.
uses separate locks for producers and consumers
to minimize contention.
producers contend with other producers.
consumers contend with other consumers.
producers do not contend with consumers.
and vice versa.

the usage pattern assumes the caller will never
try to put more elements into the firo than the
fifo will hold.
put will never block.

separating producers and consumers has a pitfall
that will take a bit of explaining.
put fundamentally does the following operations:
    *T = ptr
    ++T
these operations are done outside of the consumer lock.
there's no guarantee that these two operations will be
performed in this order.
so get must handle the case where ++T happens before
ptr is written to T.
get does not update H unless it gets a valid non-null
pointer value from *H.
getWait spin-locks waiting for a valid pointer value
to appear in *H.

assumption:
cpus may execute instructions out of order.
we assume the cpu ingests instructions in order.
and that all ingested instructions will be completed
before a thread is suspended for a context switch.
specifically we have the following special case:
    *T = ptr
    context switch
    ++T
we assume ptr is written to *T before the context is
actually switched out.
get will find H == T and return.
it will not matter that *H contains a valid pointer.

assumption:
the compiler won't re-order volatile memory accesses.
tail, and the pointers in the array are volatile.
so the compiler should emit the instructions for put
in the desired order.

assumes the library lock and unlock functions contain
fences that flush the memory write operations.
**/

#include <aggiornamento/aggiornamento.h>
#include <aggiornamento/thread.h>
#include <container/fifo.h>

#include <mutex>


// use an anonymous namespace to avoid name collisions at link time.
namespace {
    class FifoImpl : public Fifo {
    public:
        FifoImpl() = default;
        FifoImpl(const FifoImpl &) = delete;

        virtual ~FifoImpl() noexcept {
            delete[] data_;
        }

        int size_ = 0;
        int head_ = 0;
        int volatile tail_ = 0;            // must be volatile, see put
        char * volatile *data_ = nullptr;  // must be volatile, see put
        std::mutex head_mutex_;
        std::mutex tail_mutex_;
        std::condition_variable cv_;
        bool volatile cancel_ = false;
    };
}

Fifo::Fifo() {
}

Fifo::~Fifo() {
}

/*
master thread creates the fifo.
master thread deletes the fifo.

Fifo uses a fixed size array of pointers to elements.
maxCount is the maximum number of elements the fifo can hold.
*/
Fifo *Fifo::create(
    int maxCount
) noexcept {
    auto impl = new(std::nothrow) FifoImpl;
    auto size = maxCount + 1;
    impl->size_ = size;
    impl->head_ = 0;
    impl->head_ = 0;
    impl->data_ = new(std::nothrow) char*[size];
    return impl;
}

/*
returns maxCount passed to the constructor.
*/
int Fifo::getMaxCount() noexcept {
    auto impl = (FifoImpl *) this;
    auto count = impl->size_ - 1;
    return count;
}

/*
add an element to the fifo.
returns false if the fifo is full.
*/
bool Fifo::put(
    char *ptr
) noexcept {
    if (ptr == nullptr) {
        return false;
    }
    auto impl = (FifoImpl *) this;
    bool result = false;
    {
        std::unique_lock<std::mutex> lock(impl->tail_mutex_);
        auto tail = impl->tail_;
        auto new_tail = (tail + 1) % impl->size_;
        if (new_tail != impl->head_) {
            /*
            data_[n] and tail_ are volatile.
            the compiler should not change the order of these
            write operations.
            which means the cpu should ingest them together
            or in order.
            so on a context switch, either...
            both of them will be retired before the switch.
            or *T=ptr will be retired and ++T will not.
            that case is handled in get.

            if the above is wrong...
            and ++T is retired before a context switch.
            but *T++ is not...
            then getWait could be spin-locking for a long time.
            */
            impl->data_[tail] = ptr;
            impl->tail_ = new_tail;
            result = true;
        }
    }
    if (result) {
        impl->cv_.notify_all();
    }
    return result;
}

/*
get an element from the fifo.
returns nullptr immediately if the fifo is empty.
*/
char *Fifo::get() noexcept {
    auto impl = (FifoImpl *) this;
    char *ptr = nullptr;
    {
        std::unique_lock<std::mutex> lock(impl->head_mutex_);
        if (impl->head_ != impl->tail_) {
            ptr = impl->data_[impl->head_];
            if (ptr) {
                impl->data_[impl->head_] = nullptr;
                impl->head_ = (impl->head_ + 1) % impl->size_;
            }
        }
    }
    return ptr;
}

/*
get an element from the fifo.
waits for an element to be pushed if the fifo is empty.
*/
char *Fifo::getWait() noexcept {
    auto impl = (FifoImpl *) this;
    char *ptr = nullptr;
    {
        std::unique_lock<std::mutex> lock(impl->head_mutex_);
        while (impl->head_ != impl->tail_) {
            if (impl->cancel_) {
                return nullptr;
            }
            impl->cv_.wait(lock);
        }
        /*
        spin-lock here in case the ++T memory operation in put
        is completed before the write of ptr to *T.
        */
        auto head = impl->head_;
        do {
            if (impl->cancel_) {
                return nullptr;
            }
            ptr = impl->data_[head];
        } while (ptr == nullptr);
        impl->data_[head] = nullptr;
        impl->head_ = (head + 1) % impl->size_;
    }
    return ptr;
}

/*
unblock waiting threads.
*/
void Fifo::unblock() noexcept {
    auto impl = (FifoImpl *) this;
    impl->cancel_ = true;
    impl->cv_.notify_all();
}
