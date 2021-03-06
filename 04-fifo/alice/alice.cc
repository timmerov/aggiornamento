/*
Copyright (C) 2012-2016 tim cotter. All rights reserved.
*/

/**
message queue example.
implement the alice thread.
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
    const char kMeetingRequest[] = "meeting request";

    class Alice : public agm::Thread {
    public:
        Alice() noexcept : Thread("Alice") {
        }

        virtual ~Alice() = default;

        MessageQueue *message_queue_ = nullptr;
        int size_ = 0;

        virtual void begin() noexcept {
            LOG_VERBOSE("Alice");

            size_ = message_queue_->getMessageSize();
        }

        virtual void run() noexcept {
            LOG("Alice gets an empty meessage from the queue.");
            auto ptr = message_queue_->getEmpty();
            agm::sleep::milliseconds(1000);

            LOG("Alice puts " << kMeetingRequest << " into the message queue.");
            agm::string::copy(ptr, size_, kMeetingRequest);
            message_queue_->putMessage(ptr);

            agm::master::setDone();
        }

        virtual void end() noexcept {
            LOG_VERBOSE("Alice");
        }
    };
}

agm::Thread *createAlice(
    MessageQueue *message_queue
) noexcept {
    auto th = new(std::nothrow) Alice;
    th->message_queue_ = message_queue;
    return th;
}
