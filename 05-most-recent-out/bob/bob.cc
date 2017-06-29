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
        Bob() noexcept : Thread("Bob") {
        }

        virtual ~Bob() = default;

        Mro *mro_ = nullptr;
        bool first_pass_ = true;

        virtual void begin() noexcept {
            LOG_VERBOSE("Bob");
        }

        virtual void runOnce() noexcept {
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

        virtual void end() noexcept {
            LOG_VERBOSE("Bob");
        }
    };
}

agm::Thread *createBob(
    Mro *mro
) noexcept {
    auto th = new(std::nothrow) Bob;
    th->mro_ = mro;
    return th;
}
