/*
Copyright (C) 2012-2016 tim cotter. All rights reserved.
*/

/*
simple thread example.
implement the alice thread.
*/

#include <aggiornamento/aggiornamento.h>
#include <aggiornamento/thread.h>
#include <container/trunk.h>


// use an anonymous namespace to avoid name collisions at link time.
namespace {
    class Bob : public agm::Thread {
    public:
        Bob() = default;
        virtual ~Bob() = default;

        int counter_ = 0;

        void begin() throw() {
            LOG("Bob");
        }

        void runOnce() throw() {
            ++counter_;
            LOG("Bob " << counter_);
            std::this_thread::sleep_for(std::chrono::milliseconds(1100));
        }

        void drainOnce() throw() {
            LOG("Bob");
            std::this_thread::sleep_for(std::chrono::milliseconds(1100));
        }

        void unblock() throw() {
            LOG("Bob");
        }

        void end() throw() {
            LOG("Bob");
        }
    };
}

agm::Thread *createBob(
    Trunk *trunk
) throw() {
    (void) trunk;
    return new(std::nothrow) Bob;
}
