/*
Copyright (C) 2012-2016 tim cotter. All rights reserved.
*/

/**
single buffer example.
implement the bob thread.
**/

#include <aggiornamento/aggiornamento.h>
#include <aggiornamento/log.h>
#include <aggiornamento/master.h>
#include <aggiornamento/string.h>
#include <aggiornamento/thread.h>
#include <container/double-buffer.h>

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

        DoubleBuffer *db_= nullptr;

        virtual void begin() throw() {
            LOG_VERBOSE("Bob");
        }

        virtual void run() throw() {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            LOG("Bob attempts to open the airlock.");
            auto ptr = db_->acquire(0);
            LOG("Bob opens the airlock.");
            agm::string::copy(ptr, DoubleBuffer::kMaxBufferSize, kFood);
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            LOG("Bob puts " << kFood << " into the airlock.");
            LOG("Bob closes the airlock.");
            db_->release(0);
            LOG("Bob attempts to open the airlock.");
            ptr = db_->acquire(0);
            LOG("Bob opens the airlock.");
            LOG("Bob removes " << ptr << " from the airlock.");
            LOG("Bob closes the airlock.");
            db_->release(0);

            master::setDone();
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
    DoubleBuffer *db
) throw() {
    auto th = new(std::nothrow) Bob;
    th->db_ = db;
    return th;
}
