/*
Copyright (C) 2012-2016 tim cotter. All rights reserved.
*/

/**
simple thread example.
implement the alice thread.

increment and log the counter every 0.9 seconds.

technically we don't need to override all of the
interface funcions.
**/

#include <aggiornamento/aggiornamento.h>
#include <aggiornamento/log.h>
#include <aggiornamento/master.h>
#include <aggiornamento/thread.h>

// pick one
#undef LOG_VERBOSE
#define LOG_VERBOSE LOG
//#define LOG_VERBOSE(...)


// use an anonymous namespace to avoid name collisions at link time.
namespace {
    class Alice : public agm::Thread {
    public:
        Alice() noexcept : Thread("Alice") {
        }

        virtual ~Alice() noexcept {
            LOG_VERBOSE("Alice");
        }

        int counter_ = 0;

        virtual void begin() noexcept {
            LOG_VERBOSE("Alice");
        }

        virtual void runOnce() noexcept {
            ++counter_;
            LOG("Alice produces " << counter_);
            agm::sleep::milliseconds(900);
        }

        virtual void end() noexcept {
            LOG_VERBOSE("Alice");
        }
    };
}

agm::Thread *createAlice() noexcept {
    return new(std::nothrow) Alice;
}
