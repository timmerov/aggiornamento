/*
Copyright (C) 2012-2016 tim cotter. All rights reserved.
*/

/**
simple thread example.
implement the alice thread.

count to 3 every 0.9 seconds.
then quit.

technically we don't need to override all of the
interface funcions.
**/

#include <aggiornamento/aggiornamento.h>
#include <aggiornamento/log.h>
#include <aggiornamento/master.h>
#include <aggiornamento/thread2.h>

// pick one
#undef LOG_VERBOSE
#define LOG_VERBOSE LOG
//#define LOG_VERBOSE(...)


// use an anonymous namespace to avoid name collisions at link time.
namespace {
    class Alice : public agm::Thread2 {
    public:
        Alice() throw() : Thread2("Alice") {
        }

        virtual ~Alice() throw() {
            LOG_VERBOSE("Alice");
        }

        int counter_ = 0;

        virtual void begin() throw() {
            LOG_VERBOSE("Alice");
        }

        virtual void runOnce() throw() {
            ++counter_;
            LOG("Alice produces " << counter_);
            agm::sleep::milliseconds(900);
        }

        virtual void end() throw() {
            LOG_VERBOSE("Alice");
        }
    };
}

agm::Thread2 *createAlice() throw() {
    return new(std::nothrow) Alice;
}
