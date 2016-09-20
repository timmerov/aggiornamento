/*
Copyright (C) 2012-2016 tim cotter. All rights reserved.
*/

/**
write once read many example.
worm implementation.

tbd
**/

#include <aggiornamento/aggiornamento.h>
#include <aggiornamento/string.h>
#include <container/worm.h>

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
        int state_[3];
        char *data_[3];
        std::mutex mutex_;
        std::condition_variable cv_;
    };
}

Worm::Worm() throw() :
    agm::Container("WriteOnceReadMany") {
}

Worm::~Worm() {
}

Worm *Worm::create(
    int size
) throw() {
    auto impl = new(std::nothrow) WormImpl;
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
int Worm::getSize() throw() {
    auto impl = (WormImpl *) this;
    return impl->size_;
}

/*
returns an empty buffer.
marks it "filling".
*/
char *Worm::getEmpty() throw() {
    auto impl = (WormImpl *) this;
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
void Worm::putFull() throw() {
    auto impl = (WormImpl *) this;
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
char *Worm::getFull() throw() {
    auto impl = (WormImpl *) this;
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
char *Worm::getFullWait() throw() {
    auto impl = (WormImpl *) this;
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
void Worm::putEmpty() throw() {
    auto impl = (WormImpl *) this;
    {
        std::unique_lock<std::mutex> lock(impl->mutex_);
        for (auto n = 0; n < 3; ++n) {
            if (impl->state_[n] == kEmptying) {
                impl->state_[n] = kEmpty;
            }
        }
    }
}
