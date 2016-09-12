/*
Copyright (C) 2012-2016 tim cotter. All rights reserved.
*/

/**
message_queue example.
implement the bob thread.
**/

#include <aggiornamento/aggiornamento.h>
#include <aggiornamento/log.h>
#include <aggiornamento/master.h>
#include <aggiornamento/string.h>
#include <aggiornamento/thread.h>
#include <container/message-queue.h>

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

        MessageQueue *message_queue_= nullptr;
        int size_ = 0;

        virtual void begin() throw() {
            LOG_VERBOSE("Bob");

            size_ = message_queue_->getMessageSize();
        }

        virtual void run() throw() {
            LOG("Bob checks the message_queue.");
            auto ptr = message_queue_->getMessage();
            if (ptr == nullptr) {
                LOG("Bob found the message_queue empty.");
            } else {
                LOG("Oops! Bob found " << ptr << " in the message_queue.");
            }
            LOG("Bob waits for the message_queue.");
            ptr = message_queue_->getMessageWait();
            if (ptr) {
                LOG("Bob found " << ptr << " in the message_queue.");
            } else {
                LOG("Oops! Bob found the message_queue empty.");
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
    MessageQueue *message_queue
) throw() {
    auto th = new(std::nothrow) Bob;
    th->message_queue_ = message_queue;
    return th;
}
