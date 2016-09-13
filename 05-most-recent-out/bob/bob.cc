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
    class Bob : public agm::Thread {
    public:
        Bob() throw() : Thread("Bob") {
        }

        virtual ~Bob() = default;

        Mro *mro_ = nullptr;
        bool first_pass_ = true;

        virtual void begin() throw() {
            LOG_VERBOSE("Bob");
        }

        virtual void runOnce() throw() {
            if (first_pass_) {
                first_pass_ = false;
                LOG("Bob is waiting.");
            }

            auto ptr = mro_->getFullWait();

            // check stop condition.
            if (isProducing() == false) {
                return;
            }

            LOG("Bob finds " << ptr << " in the mro");
            mro_->putEmpty();
            agm::sleep::milliseconds(1000);
        }

        virtual void drainOnce() throw() {
            LOG_VERBOSE("Bob");
            agm::sleep::milliseconds(100);
        }

        virtual void unblock() throw() {
            LOG_VERBOSE("Bob");

            if (isDraining()) {
                mro_->putFull();
            }
        }

        virtual void end() throw() {
            LOG_VERBOSE("Bob");
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
