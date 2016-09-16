/*
Copyright (C) 2012-2016 tim cotter. All rights reserved.
*/

/**
write once read many example.
implement the alice thread.
**/

#include <aggiornamento/aggiornamento.h>
#include <aggiornamento/log.h>
#include <aggiornamento/master.h>
#include <aggiornamento/string.h>
#include <aggiornamento/thread.h>
#include <container/worm.h>

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

        virtual ~Alice() = default;

        Worm *worm_ = nullptr;
        int size_ = 0;
        int counter_ = 0;

        virtual void begin() throw() {
            size_ = worm_->getSize();
        }

        virtual void runOnce() throw() {
            if (counter_ == 0) {
                LOG("Alice is snoozing.");
                agm::sleep::milliseconds(1000);
            }

            auto ptr = worm_->getEmpty();
            LOG("Alice puts " << counter_ << " into the worm.");
            std::string s = std::move(std::to_string(counter_));
            ++counter_;
            agm::string::copy(ptr, size_, s.c_str());
            worm_->putFull();
            agm::sleep::milliseconds(1000*24/60);
        }

        virtual void drainOnce() throw() {
            LOG_VERBOSE("Alice");
            agm::sleep::milliseconds(100);
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
    Worm *worm
) throw() {
    auto th = new(std::nothrow) Alice;
    th->worm_ = worm;
    return th;
}
