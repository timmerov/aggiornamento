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
#include <aggiornamento/thread.h>
#include <container/trunk.h>

// pick one
#undef LOG_VERBOSE
//#define LOG_VERBOSE LOG
#define LOG_VERBOSE(...)


// use an anonymous namespace to avoid name collisions at link time.
namespace {
    const auto kCleanSocks = "clean socks";

    class Alice : public agm::Thread {
    public:
        Alice() throw() : Thread("Alice") {
        }

        virtual ~Alice() = default;

        Trunk *trunk_ = nullptr;

        virtual void begin() throw() {
            LOG_VERBOSE("Alice");
        }

        virtual void run() throw() {
            LOG("Alice puts \"" << kCleanSocks << "\" into the trunk");
            trunk_->putString(kCleanSocks);
            std::this_thread::sleep_for(std::chrono::milliseconds(1500));
            char buffer[Trunk::kMaxStringSize];
            trunk_->getString(buffer);
            LOG("Alice finds \"" << buffer << "\" in the trunk");
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
            LOG_VERBOSE("Alice");
        }
    };
}

agm::Thread *createAlice(
    Trunk *trunk
) throw() {
    auto th = new(std::nothrow) Alice;
    th->trunk_ = trunk;
    return th;
}
