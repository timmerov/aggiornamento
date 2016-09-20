/*
Copyright (C) 2012-2016 tim cotter. All rights reserved.
*/

/**
write once read many example.
implement the bob thread.
**/

#include <aggiornamento/aggiornamento.h>
#include <aggiornamento/log.h>
#include <aggiornamento/master.h>
#include <aggiornamento/thread.h>
#include <container/worm.h>

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

        Worm *worm_ = nullptr;
        bool first_pass_ = true;

        virtual void begin() throw() {
            LOG_VERBOSE("Bob");
        }

        virtual void runOnce() throw() {
            if (first_pass_) {
                first_pass_ = false;
                LOG("Bob is waiting.");
            }

            auto ptr = worm_->getFullWait();

            // check stop condition.
            if (isRunning() == false) {
                return;
            }

            LOG("Bob finds " << ptr << " in the worm");
            worm_->putEmpty();
            agm::sleep::milliseconds(1000);
        }


        virtual void end() throw() {
            LOG_VERBOSE("Bob");
        }
    };
}

agm::Thread *createBob(
    Worm *worm
) throw() {
    auto th = new(std::nothrow) Bob;
    th->worm_ = worm;
    return th;
}
