/*
Copyright (C) 2012-2016 tim cotter. All rights reserved.
*/

/**
most recent out example.
implement the alice thread.
**/

#include <aggiornamento/aggiornamento.h>
#include <aggiornamento/log.h>
#include <aggiornamento/master.h>
#include <aggiornamento/string.h>
#include <aggiornamento/thread.h>
#include <container/mro.h>

// pick one
#undef LOG_VERBOSE
//#define LOG_VERBOSE LOG
#define LOG_VERBOSE(...)


// use an anonymous namespace to avoid name collisions at link time.
namespace {
    class Alice : public agm::Thread {
    public:
        Alice() noexcept : Thread("Alice") {
        }

        virtual ~Alice() = default;

        Mro *mro_ = nullptr;
        int size_ = 0;
        int counter_ = 0;

        virtual void begin() noexcept {
            size_ = mro_->getSize();
        }

        virtual void runOnce() noexcept {
            if (counter_ == 0) {
                LOG("Alice is snoozing.");
                agm::sleep::milliseconds(1000);
            }

            auto ptr = mro_->getEmpty();
            LOG("Alice puts " << counter_ << " into the mro.");
            std::string s = std::move(std::to_string(counter_));
            ++counter_;
            agm::string::copy(ptr, size_, s.c_str());
            mro_->putFull();
            agm::sleep::milliseconds(1000*24/60);
        }

        virtual void end() noexcept {
            LOG_VERBOSE("Alice");
        }
    };
}

agm::Thread *createAlice(
    Mro *mro
) noexcept {
    auto th = new(std::nothrow) Alice;
    th->mro_ = mro;
    return th;
}
