/*
Copyright (C) 2012-2016 tim cotter. All rights reserved.
*/

/**
most recent out example.
implement the alice thread.
**/

#include <aggiornamento/aggiornamento.h>
#include <aggiornamento/log.h>
#include <aggiornamento/master.h>
#include <aggiornamento/thread.h>
#include <container/mro.h>

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

        Mro *mro_ = nullptr;
        int mro_size_ = 0;
        char *mro_buffer_ = nullptr;

        virtual void begin() throw() {
            LOG("Alice is in the spaceship at the airlock.");

            mro_size_ = mro_->getSize();
            mro_buffer_ = new(std::nothrow) char[mro_size_];
        }

        virtual void run() throw() {
            LOG("Alice puts " << kCleanSocks << " into the mro");
            mro_->putString(kCleanSocks);
            agm::sleep::milliseconds(1500);
            mro_->getString(mro_buffer_, mro_size_);
            LOG("Alice finds " << mro_buffer_ << " in the mro");

            master::setDone();
        }

        virtual void drainOnce() throw() {
            LOG_VERBOSE("Alice");
            agm::sleep::milliseconds(100);
        }

        virtual void unblock() throw() {
            LOG_VERBOSE("Alice");
        }

        virtual void end() throw() {
            LOG("Alice went home.");

            mro_size_ = 0;
            delete[] mro_buffer_;
            mro_buffer_ = nullptr;
        }
    };
}

agm::Thread *createAlice(
    Mro *mro
) throw() {
    auto th = new(std::nothrow) Alice;
    th->mro_ = mro;
    return th;
}
