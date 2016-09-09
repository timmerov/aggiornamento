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
    class Alice : public agm::Thread {
    public:
        Alice() = default;
        virtual ~Alice() = default;

        int counter_ = 0;

        void begin() throw() {
            LOG("Alice");
        }

        void runOnce() throw() {
            ++counter_;
            LOG("Alice " << counter_);
            std::this_thread::sleep_for(std::chrono::milliseconds(900));
        }

        void drainOnce() throw() {
            LOG("Alice");
            std::this_thread::sleep_for(std::chrono::milliseconds(900));
        }

        void unblock() throw() {
            LOG("Alice");
        }

        void end() throw() {
            LOG("Alice");
        }
    };
}

agm::Thread *createAlice(
    Trunk *trunk
) throw() {
    (void) trunk;
    return new(std::nothrow) Alice;
}
