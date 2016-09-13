/*
Copyright (C) 2012-2016 tim cotter. All rights reserved.
*/

/*
most recent out example.
implementation.

tk
*/

#include <aggiornamento/aggiornamento.h>
#include <aggiornamento/string.h>
#include <container/mro.h>

#include <mutex>


// use an anonymous namespace to avoid name collisions at link time.
namespace {
    class MroImpl : public Mro {
    public:
        MroImpl() = default;
        MroImpl(const MroImpl &) = delete;

        virtual ~MroImpl() throw() {
            delete data_;
        }

        int size_ = 0;
        char *data_ = nullptr;
        std::mutex mutex_;
    };
}

Mro::Mro() {
}

Mro::~Mro() {
}

Mro *Mro::create(
    int size
) throw() {
    auto impl = new(std::nothrow) MroImpl;
    impl->size_ = size;
    impl->data_ = new(std::nothrow) char[size];
    return impl;
}

/*
return the size of the buffer.
*/
int Mro::getSize() throw() {
    auto impl = (MroImpl *) this;
    return impl->size_;
}

/*
overwrites whatever was in the trunk.
*/
void Mro::putString(
    const char *buffer
) throw() {
    auto impl = (MroImpl *) this;
    std::lock_guard<std::mutex> lock(impl->mutex_);
    agm::string::copy(impl->data_, impl->size_, buffer);
}

/*
copies the string from the trunk.
the string in the trunk is preserved.
*/
void Mro::getString(
    char *buffer,
    int size
) const throw() {
    auto impl = (MroImpl *) this;
    std::lock_guard<std::mutex> lock(impl->mutex_);
    agm::string::copy(buffer, size, impl->data_);
}
