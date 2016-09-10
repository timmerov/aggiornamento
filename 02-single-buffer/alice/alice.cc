/*
Copyright (C) 2012-2016 tim cotter. All rights reserved.
*/

/**
single buffer example.
implement the alice thread.
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
    const auto kCleanSocks = "clean socks";
    const auto kGarbage = "garbage";

    class Alice : public agm::Thread {
    public:
        Alice() throw() : Thread("Alice") {
        }

        virtual ~Alice() = default;

        Trunk *trunk_ = nullptr;
        int trunk_size_ = 0;
        char *trunk_buffer_ = nullptr;

        Airlock *airlock_ = nullptr;
        int airlock_size_ = 0;

        virtual void begin() throw() {
            LOG("Alice is in the spaceship at the airlock.");

            trunk_size_ = trunk_->getSize();
            trunk_buffer_ = new(std::nothrow) char[trunk_size_];

            airlock_size_ = airlock_->getSize();
        }

        virtual void run() throw() {
            LOG("Alice puts " << kCleanSocks << " into the trunk");
            trunk_->putString(kCleanSocks);
            std::this_thread::sleep_for(std::chrono::milliseconds(1500));
            trunk_->getString(trunk_buffer_, trunk_size_);
            LOG("Alice finds " << trunk_buffer_ << " in the trunk");

            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            LOG("Alice attempts to open the airlock.");
            auto ptr = airlock_->acquire(1);
            LOG("Alice opens the airlock.");
            LOG("Alice removed " << ptr << " from the airlock.");
            agm::string::copy(ptr, airlock_size_, kGarbage);
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            LOG("Alice puts " << kGarbage << " in the airlock.");
            LOG("Alice closes the airlock.");
            airlock_->release(1);

            LOG("Alice is no longer using the airlock.");
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));

            LOG("Alice ends the interaction.");
            master::setDone();
        }

        virtual void drainOnce() throw() {
            LOG_VERBOSE("Alice");
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        virtual void unblock() throw() {
            LOG_VERBOSE("Alice");
        }

        virtual void end() throw() {
            LOG("Alice went home.");

            trunk_size_ = 0;
            delete[] trunk_buffer_;
            trunk_buffer_ = nullptr;
        }
    };
}

agm::Thread *createAlice(
    Airlock *airlock,
    Trunk *trunk
) throw() {
    auto th = new(std::nothrow) Alice;
    th->airlock_ = airlock;
    th->trunk_ = trunk;
    return th;
}
