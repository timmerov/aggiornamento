/*
Copyright (C) 2012-2016 tim cotter. All rights reserved.
*/

/*
single buffer example.
trunk container implementation.

the implementation uses a single mutex to protect the data.
there is no other synchronization.
there is no signalling.
*/

#include <aggiornamento/aggiornamento.h>
#include <aggiornamento/string.h>
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

/*
overwrites whatever was in the trunk.
*/
void Trunk::putString(
    const char *buffer
) throw() {
    auto impl = (TrunkImpl *) this;
    std::lock_guard<std::mutex> lock(impl->mutex_);
    agm::string::copy(impl->data_, buffer);
}

/*
copies the string from the trunk.
the string in the trunk is preserved.
*/
void Trunk::getString(
    char *buffer,
    int size
) const throw() {
    auto impl = (TrunkImpl *) this;
    std::lock_guard<std::mutex> lock(impl->mutex_);
    agm::string::copy(buffer, size, impl->data_);
}
