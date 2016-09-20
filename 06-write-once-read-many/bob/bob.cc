/*
Copyright (C) 2012-2016 tim cotter. All rights reserved.
*/

/**
write once-in-a-while read many example.
implement the bob thread.
**/

#include <aggiornamento/aggiornamento.h>
#include <aggiornamento/log.h>
#include <aggiornamento/master.h>
#include <aggiornamento/thread.h>
#include <container/worm.h>

#include <cstring>

// pick one
#undef LOG_VERBOSE
//#define LOG_VERBOSE LOG
#define LOG_VERBOSE(...)


// use an anonymous namespace to avoid name collisions at link time.
namespace {
    class Bob : public agm::Thread {
    public:
        Bob() throw() : Thread("Bob") {
        }

        virtual ~Bob() = default;

        Worm *worm_ = nullptr;
        std::string old_str_;

        virtual void begin() throw() {
            LOG_VERBOSE("Bob");
        }

        virtual void runOnce() throw() {
            int start_state;
            int end_state;
            std::string new_str;
            for(;;) {
                start_state = worm_->getState();
                auto ptr = worm_->getReadBuffer(start_state);
                new_str = ptr;
                end_state = worm_->getState();
                if (end_state == start_state) {
                    break;
                }
                LOG("Bob detected a worm collision. Retrying...");
            }
            if (strcmp(old_str_.c_str(), new_str.c_str()) != 0) {
                LOG("Bob finds " << new_str << " in the worm");
                old_str_ = std::move(new_str);
            }

            // normally we'd sleep or block.
            // but for this test, we busy loop.
        }

        virtual void end() throw() {
            LOG_VERBOSE("Bob");
        }
    };
}

agm::Thread *createBob(
    Worm *worm
) throw() {
    auto th = new(std::nothrow) Bob;
    th->worm_ = worm;
    return th;
}
