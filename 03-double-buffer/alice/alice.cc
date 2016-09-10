/*
Copyright (C) 2012-2016 tim cotter. All rights reserved.
*/

/**
double buffer example.
implement the alice thread.
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
    const auto kCleanSocks = "clean socks";
    const auto kGarbage = "garbage";

    class Alice : public agm::Thread {
    public:
        Alice() throw() : Thread("Alice") {
        }

        virtual ~Alice() = default;

        DoubleBuffer *db_ = nullptr;

        virtual void begin() throw() {
            LOG_VERBOSE("Alice");
        }

        virtual void run() throw() {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            LOG("Alice attempts to open the airlock.");
            auto ptr = db_->acquire(1);
            LOG("Alice opens the airlock.");
            LOG("Alice removed " << ptr << " from the airlock.");
            agm::string::copy(ptr, DoubleBuffer::kMaxBufferSize, kGarbage);
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            LOG("Alice puts " << kGarbage << " in the airlock.");
            LOG("Alice closes the airlock.");
            db_->release(1);

            master::waitDone();
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
    DoubleBuffer *db
) throw() {
    auto th = new(std::nothrow) Alice;
    th->db_ = db;
    return th;
}
