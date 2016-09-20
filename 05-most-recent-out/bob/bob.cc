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
#include <aggiornamento/thread2.h>
#include <container/mro.h>

// pick one
#undef LOG_VERBOSE
//#define LOG_VERBOSE LOG
#define LOG_VERBOSE(...)


// use an anonymous namespace to avoid name collisions at link time.
namespace {
    class Bob : public agm::Thread2 {
    public:
        Bob() throw() : Thread2("Bob") {
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
            if (isRunning() == false) {
                return;
            }

            LOG("Bob finds " << ptr << " in the mro");
            mro_->putEmpty();
            agm::sleep::milliseconds(1000);
        }

        virtual void end() throw() {
            LOG_VERBOSE("Bob");
        }
    };
}

agm::Thread2 *createBob(
    Mro *mro
) throw() {
    auto th = new(std::nothrow) Bob;
    th->mro_ = mro;
    return th;
}
