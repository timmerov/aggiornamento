/*
Copyright (C) 2012-2016 tim cotter. All rights reserved.
*/

/**
fifo example.
implement the bob thread.
**/

#include <aggiornamento/aggiornamento.h>
#include <aggiornamento/log.h>
#include <aggiornamento/master.h>
#include <aggiornamento/string.h>
#include <aggiornamento/thread.h>
#include <container/fifo.h>

// pick one
#undef LOG_VERBOSE
//#define LOG_VERBOSE LOG
#define LOG_VERBOSE(...)


// use an anonymous namespace to avoid name collisions at link time.
namespace {
    const auto kSnipsSnails = "snips and snails";

    class Bob : public agm::Thread {
    public:
        Bob() throw() : Thread("Bob") {
        }

        virtual ~Bob() = default;

        Fifo *fifo_= nullptr;
        int size_ = 0;

        virtual void begin() throw() {
            LOG_VERBOSE("Bob");
        }

        virtual void run() throw() {
            LOG("Bob checks the fifo.");
            auto ptr = fifo_->get();
            if (ptr == nullptr) {
                LOG("Bob found the fifo empty.");
            } else {
                LOG("Oops! Bob found " << ptr << " in the fifo.");
            }
            LOG("Bob waits for the fifo.");
            ptr = fifo_->getWait();
            if (ptr) {
                LOG("Bob found " << ptr << " in the fifo.");
            } else {
                LOG("Oops! Bob found the fifo empty.");
            }

            master::waitDone();
        }

        virtual void drainOnce() throw() {
            LOG_VERBOSE("Bob");
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        virtual void unblock() throw() {
            LOG_VERBOSE("Bob");
        }

        virtual void end() throw() {
            LOG_VERBOSE("Bob");
        }
    };
}

agm::Thread *createBob(
    Fifo *fifo
) throw() {
    auto th = new(std::nothrow) Bob;
    th->fifo_ = fifo;
    return th;
}
