/*
Copyright (C) 2012-2016 tim cotter. All rights reserved.
*/

/**
double buffer example.
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
    const auto kSnipsSnails = "snips and snails";

    class Bob : public agm::Thread {
    public:
        Bob() throw() : Thread("Bob") {
        }

        virtual ~Bob() = default;

        DoubleBuffer *db_= nullptr;
        int size_ = 0;

        virtual void begin() throw() {
            LOG_VERBOSE("Bob");

            size_ = db_->getSize();
        }

        virtual void run() throw() {
            LOG("Bob acquires a buffer.");
            auto ptr = db_->acquire(1);
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            agm::string::copy(ptr, size_, kSnipsSnails);
            LOG("Bob puts " << kSnipsSnails << " into the buffer.");
            LOG("Bob wants to swap buffers.");
            ptr = db_->swap(ptr);
            LOG("Bob swapped buffers.");
            LOG("Bob finds " << ptr << " in the buffer.");

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
    DoubleBuffer *db
) throw() {
    auto th = new(std::nothrow) Bob;
    th->db_ = db;
    return th;
}
