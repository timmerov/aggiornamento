/*
Copyright (C) 2012-2016 tim cotter. All rights reserved.
*/

/**
single buffer example.
implement the bob thread.
**/

#include <aggiornamento/aggiornamento.h>
#include <aggiornamento/log.h>
#include <aggiornamento/master.h>
#include <aggiornamento/string.h>
#include <aggiornamento/thread.h>
#include <container/airlock.h>
#include <container/trunk.h>

// pick one
#undef LOG_VERBOSE
//#define LOG_VERBOSE LOG
#define LOG_VERBOSE(...)


// use an anonymous namespace to avoid name collisions at link time.
namespace {
    const auto kDirtyShirt = "dirty shirt";
    const auto kFood = "food";

    class Bob : public agm::Thread {
    public:
        Bob() throw() : Thread("Bob") {
        }

        virtual ~Bob() = default;

        Trunk *trunk_ = nullptr;
        int trunk_size_ = 0;
        char *trunk_buffer_ = nullptr;

        Airlock *airlock_ = nullptr;
        int airlock_size_ = 0;

        virtual void begin() throw() {
            LOG_VERBOSE("Bob");

            trunk_size_ = trunk_->getSize();
            trunk_buffer_ = new(std::nothrow) char[trunk_size_];

            airlock_size_ = airlock_->getSize();
        }

        virtual void run() throw() {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            trunk_->getString(trunk_buffer_, trunk_size_);
            LOG("Bob finds " << trunk_buffer_ << " in the trunk");
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            LOG("Bob puts " << kDirtyShirt << " into the trunk");
            trunk_->putString(kDirtyShirt);

            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            LOG("Bob attempts to open the airlock.");
            auto ptr = airlock_->acquire(0);
            LOG("Bob opens the airlock.");
            agm::string::copy(ptr, airlock_size_, kFood);
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            LOG("Bob puts " << kFood << " into the airlock.");
            LOG("Bob closes the airlock.");
            airlock_->release(0);
            LOG("Bob attempts to open the airlock.");
            ptr = airlock_->acquire(0);
            LOG("Bob opens the airlock.");
            LOG("Bob removes " << ptr << " from the airlock.");
            LOG("Bob closes the airlock.");
            airlock_->release(0);

            master::setDone();
        }

        virtual void drainOnce() throw() {
            LOG_VERBOSE("Bob");
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        virtual void unblock() throw() {
            LOG_VERBOSE("Bob");
        }

        virtual void end() throw() {
            LOG_VERBOSE("Bob");

            trunk_size_ = 0;
            delete[] trunk_buffer_;
            trunk_buffer_ = nullptr;
        }
    };
}

agm::Thread *createBob(
    Airlock *airlock,
    Trunk *trunk
) throw() {
    auto th = new(std::nothrow) Bob;
    th->airlock_ = airlock;
    th->trunk_ = trunk;
    return th;
}
