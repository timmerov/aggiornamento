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
#include <aggiornamento/thread.h>
#include <container/trunk.h>

// pick one
#undef LOG_VERBOSE
//#define LOG_VERBOSE LOG
#define LOG_VERBOSE(...)


// use an anonymous namespace to avoid name collisions at link time.
namespace {
    const auto kDirtyShirt = "dirty shirt";

    class Bob : public agm::Thread {
    public:
        Bob() throw() : Thread("Bob") {
        }

        virtual ~Bob() = default;

        Trunk *trunk_ = nullptr;

        virtual void begin() throw() {
            LOG_VERBOSE("Bob");
        }

        virtual void run() throw() {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            char buffer[Trunk::kMaxStringSize];
            trunk_->getString(buffer);
            LOG("Bob finds \"" << buffer << "\" in the trunk");
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            LOG("Bob puts \"" << kDirtyShirt << "\" into the trunk");
            trunk_->putString(kDirtyShirt);
            master::waitDone();
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
        }
    };
}

agm::Thread *createBob(
    Trunk *trunk
) throw() {
    auto th = new(std::nothrow) Bob;
    th->trunk_ = trunk;
    return th;
}
