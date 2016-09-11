/*
Copyright (C) 2012-2016 tim cotter. All rights reserved.
*/

/**
fifo example.
implement the alice thread.
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
    const char kMeetingRequest[] = "meeting request";

    class Alice : public agm::Thread {
    public:
        Alice() throw() : Thread("Alice") {
        }

        virtual ~Alice() = default;

        Fifo *fifo_ = nullptr;
        char buffer_[sizeof(kMeetingRequest)];

        virtual void begin() throw() {
            LOG_VERBOSE("Alice");
        }

        virtual void run() throw() {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            LOG("Alice puts " << kMeetingRequest << " into the fifo.");
            agm::string::copy(buffer_, kMeetingRequest);
            fifo_->put(buffer_);

            master::setDone();
        }

        virtual void drainOnce() throw() {
            LOG_VERBOSE("Alice");
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
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
    Fifo *fifo
) throw() {
    auto th = new(std::nothrow) Alice;
    th->fifo_ = fifo;
    return th;
}
