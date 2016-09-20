/*
Copyright (C) 2012-2016 tim cotter. All rights reserved.
*/

/**
write once-in-a-while read many example.
implement the alice thread.
**/

#include <aggiornamento/aggiornamento.h>
#include <aggiornamento/log.h>
#include <aggiornamento/master.h>
#include <aggiornamento/string.h>
#include <aggiornamento/thread.h>
#include <container/worm.h>

// pick one
#undef LOG_VERBOSE
//#define LOG_VERBOSE LOG
#define LOG_VERBOSE(...)


// use an anonymous namespace to avoid name collisions at link time.
namespace {
    class Alice : public agm::Thread {
    public:
        Alice() throw() : Thread("Alice") {
        }

        virtual ~Alice() = default;

        Worm *worm_ = nullptr;
        int size_ = 0;
        int counter_ = 0;

        virtual void begin() throw() {
            size_ = worm_->getSize();
        }

        virtual void runOnce() throw() {
            auto ptr = worm_->getWriteBuffer();
            LOG("Alice puts " << counter_ << " into the worm.");
            std::string s = std::move(std::to_string(counter_));
            ++counter_;
            agm::string::copy(ptr, size_, s.c_str());
            worm_->swap();
            agm::sleep::milliseconds(900);
        }

        virtual void end() throw() {
            LOG_VERBOSE("Alice");
        }
    };
}

agm::Thread *createAlice(
    Worm *worm
) throw() {
    auto th = new(std::nothrow) Alice;
    th->worm_ = worm;
    return th;
}
