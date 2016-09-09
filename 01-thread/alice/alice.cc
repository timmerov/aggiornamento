/*
Copyright (C) 2012-2016 tim cotter. All rights reserved.
*/

/**
simple thread example.
implement the alice thread.
**/

#include <aggiornamento/aggiornamento.h>
#include <aggiornamento/log.h>
#include <aggiornamento/master.h>
#include <aggiornamento/thread.h>

// pick one
#undef LOG_VERBOSE
//#define LOG_VERBOSE LOG
#define LOG_VERBOSE(...)


// use an anonymous namespace to avoid name collisions at link time.
namespace {
    class Alice : public agm::Thread {
    public:
        Alice() throw() : Thread("Alice") {
        }

        virtual ~Alice() throw() {
            LOG_VERBOSE("Alice");
        }

        int counter_ = 0;

        void begin() throw() {
            LOG_VERBOSE("Alice");
        }

        void runOnce() throw() {
            ++counter_;
            LOG("Alice " << counter_);
            if (counter_ >= 3) {
                master::setDone();
                stopProducingSelf();
            } else {
                std::this_thread::sleep_for(std::chrono::milliseconds(900));
            }
        }

        void drainOnce() throw() {
            LOG_VERBOSE("Alice");
            std::this_thread::sleep_for(std::chrono::milliseconds(900));
        }

        void unblock() throw() {
            LOG_VERBOSE("Alice");
        }

        void end() throw() {
            LOG_VERBOSE("Alice");
        }
    };
}

agm::Thread *createAlice() throw() {
    return new(std::nothrow) Alice;
}
