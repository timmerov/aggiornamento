/*
Copyright (C) 2012-2016 tim cotter. All rights reserved.
*/

/**
most recent out example.
implement the bob thread.
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
    const auto kDirtyShirt = "dirty shirt";
    const auto kFood = "food";

    class Bob : public agm::Thread {
    public:
        Bob() throw() : Thread("Bob") {
        }

        virtual ~Bob() = default;

        Mro *mro_ = nullptr;
        int mro_size_ = 0;
        char *mro_buffer_ = nullptr;

        virtual void begin() throw() {
            LOG("Bob arrives at the airlock.");

            mro_size_ = mro_->getSize();
            mro_buffer_ = new(std::nothrow) char[mro_size_];
        }

        virtual void run() throw() {
            agm::sleep::milliseconds(500);
            mro_->getString(mro_buffer_, mro_size_);
            LOG("Bob finds " << mro_buffer_ << " in the mro");
            agm::sleep::milliseconds(500);
            LOG("Bob puts " << kDirtyShirt << " into the mro");
            mro_->putString(kDirtyShirt);

            master::waitDone();
        }

        virtual void drainOnce() throw() {
            LOG_VERBOSE("Bob");
            agm::sleep::milliseconds(100);
        }

        virtual void unblock() throw() {
            LOG_VERBOSE("Bob");
        }

        virtual void end() throw() {
            LOG("Bob went home.");

            mro_size_ = 0;
            delete[] mro_buffer_;
            mro_buffer_ = nullptr;
        }
    };
}

agm::Thread *createBob(
    Mro *mro
) throw() {
    auto th = new(std::nothrow) Bob;
    th->mro_ = mro;
    return th;
}
