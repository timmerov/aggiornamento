/*
Copyright (C) 2012-2016 tim cotter. All rights reserved.
*/

/*
single buffer example.
trunk container implementation.
*/

#include <aggiornamento/aggiornamento.h>
#include <container/trunk.h>

#include <mutex>


// use an anonymous namespace to avoid name collisions at link time.
namespace {
    class TrunkImpl : public Trunk {
    public:
        TrunkImpl() = default;
        TrunkImpl(const TrunkImpl &) = delete;
        virtual ~TrunkImpl() = default;

        std::mutex mutex_;
        char data_[kMaxStringSize];
    };
}

Trunk::Trunk() {
}

Trunk::~Trunk() {
}

Trunk *Trunk::create() throw() {
    return new(std::nothrow) TrunkImpl;
}

void Trunk::putString(
    const char *buffer
) throw() {
    auto impl = (TrunkImpl *) this;
    std::lock_guard<std::mutex> lock(impl->mutex_);
    agm::string::copy(impl->data_, buffer);
}

void Trunk::getString(
    char *buffer,
    int size
) const throw() {
    auto impl = (TrunkImpl *) this;
    std::lock_guard<std::mutex> lock(impl->mutex_);
    agm::string::copy(buffer, size, impl->data_);
}
