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

        void begin() throw() {
            LOG_VERBOSE("Bob");
        }

        void run() throw() {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            char buffer[Trunk::kMaxStringSize];
            trunk_->getString(buffer);
            LOG("Bob finds \"" << buffer << "\" in the trunk");
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            LOG("Bob puts \"" << kDirtyShirt << "\" into the trunk");
            trunk_->putString(kDirtyShirt);
            master::waitDone();
        }

        void drainOnce() throw() {
            LOG_VERBOSE("Bob");
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        void unblock() throw() {
            LOG_VERBOSE("Bob");
        }

        void end() throw() {
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
