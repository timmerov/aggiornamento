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

        Airlock *airlock_ = nullptr;
        Trunk *trunk_ = nullptr;

        virtual void begin() throw() {
            LOG_VERBOSE("Alice");
        }

        virtual void run() throw() {
            LOG("Alice puts " << kCleanSocks << " into the trunk");
            trunk_->putString(kCleanSocks);
            std::this_thread::sleep_for(std::chrono::milliseconds(1500));
            char buffer[Trunk::kMaxStringSize];
            trunk_->getString(buffer);
            LOG("Alice finds " << buffer << " in the trunk");

            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            LOG("Alice attempts to open the airlock.");
            auto ptr = airlock_->acquire(1);
            LOG("Alice opens the airlock.");
            LOG("Alice removed " << ptr << " from the airlock.");
            agm::string::copy(ptr, Airlock::kMaxBufferSize, kGarbage);
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            LOG("Alice puts " << kGarbage << " in the airlock.");
            LOG("Alice closes the airlock.");
            airlock_->release(1);

            master::waitDone();
        }

        virtual void drainOnce() throw() {
            LOG_VERBOSE("Alice");
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        virtual void unblock() throw() {
            LOG_VERBOSE("Alice");
        }

        virtual void end() throw() {
            LOG_VERBOSE("Alice");
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
