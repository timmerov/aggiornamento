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
#include <container/fifo.h>

// pick one
#undef LOG_VERBOSE
//#define LOG_VERBOSE LOG
#define LOG_VERBOSE(...)


// use an anonymous namespace to avoid name collisions at link time.
namespace {
    const auto kSugarSpice = "sugar and spice";

    class Alice : public agm::Thread {
    public:
        Alice() throw() : Thread("Alice") {
        }

        virtual ~Alice() = default;

        DoubleBuffer *db_ = nullptr;
        int size_ = 0;

        virtual void begin() throw() {
            LOG_VERBOSE("Alice");

            size_ = db_->getSize();
        }

        virtual void run() throw() {
            LOG("Alice acquires a buffer.");
            auto ptr = db_->acquire(0);
            agm::string::copy(ptr, size_, kSugarSpice);
            LOG("Alice puts " << kSugarSpice << " into the buffer.");
            LOG("Alice wants to swap buffers.");
            ptr = db_->swap(ptr);
            LOG("Alice swapped buffers.");
            LOG("Alice finds " << ptr << " in the buffer.");

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
    DoubleBuffer *db
) throw() {
    auto th = new(std::nothrow) Alice;
    th->db_ = db;
    return th;
}
