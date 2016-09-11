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
    class FifoImpl : public Fifo {
    public:
        FifoImpl() = default;
        FifoImpl(const FifoImpl &) = delete;

        virtual ~FifoImpl() throw() {
            delete[] data_;
        }

        int data_size_ = 0;
        int head_ = 0;
        int tail_ = 0;
        char **data_ = nullptr;
        std::mutex head_mutex_;
        std::mutex tail_mutex_;
        std::condition_variable cv_;
    };
}

Fifo::Fifo() {
}

Fifo::~Fifo() {
}

/*
master thread creates the double buffer.
master thread deletes the double buffer.

Fifo uses a fixed size array of pointers to elements.
maxCount is the maximum number of elements the fifo can hold.
*/
Fifo *Fifo::create(
    int maxCount
) throw() {
    auto impl = new(std::nothrow) FifoImpl;
    auto size = maxCount + 1;
    impl->data_size_ = size;
    impl->head_ = 0;
    impl->head_ = 0;
    impl->data_ = new(std::nothrow) char*[size];
    return impl;
}

/*
returns returns maxCount passed to the constructor.
*/
int Fifo::getMaxCount() throw() {
    auto impl = (FifoImpl *) this;
    auto count = impl->data_size_ - 1;
    return count;
}

/*
add an element to the fifo.
returns false if the fifo is full.
*/
bool Fifo::put(
    char *ptr
) throw() {
    auto impl = (FifoImpl *) this;
    bool result = false;
    {
        std::unique_lock<std::mutex> lock(impl->tail_mutex_);
        auto new_tail = (impl->tail_ + 1) % impl->data_size_;
        if (new_tail != impl->head_) {
            impl->data_[impl->tail_] = ptr;
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
char *Fifo::get() throw() {
    auto impl = (FifoImpl *) this;
    char *ptr = nullptr;
    {
        std::unique_lock<std::mutex> lock(impl->head_mutex_);
        if (impl->head_ != impl->tail_) {
            ptr = impl->data_[impl->head_];
            if (ptr) {
                impl->data_[impl->head_] = nullptr;
                impl->head_ = (impl->head_ + 1) % impl->data_size_;
            }
        }
    }
    return ptr;
}

/*
get an element from the fifo.
waits for an element to be pushed if the fifo is empty.
*/
char *Fifo::getWait() throw() {
    auto impl = (FifoImpl *) this;
    char *ptr = nullptr;
    {
        std::unique_lock<std::mutex> lock(impl->head_mutex_);
        for(;;) {
            if (impl->head_ != impl->tail_) {
                ptr = impl->data_[impl->head_];
                if (ptr) {
                    break;
                }
            }
            impl->cv_.wait(lock);
        }
        impl->data_[impl->head_] = nullptr;
        impl->head_ = (impl->head_ + 1) % impl->data_size_;
    }
    return ptr;
}
