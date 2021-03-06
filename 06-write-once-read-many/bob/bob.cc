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
        Bob() noexcept : Thread("Bob") {
        }

        virtual ~Bob() = default;

        Worm *worm_ = nullptr;
        std::string old_str_;

        virtual void begin() noexcept {
            LOG_VERBOSE("Bob");
        }

        virtual void runOnce() noexcept {
            std::string new_str;
            for(;;) {
                auto state = worm_->getState();
                auto ptr = worm_->getReadBuffer(state);
                new_str = ptr;
                if (worm_->checkState(state)) {
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

        virtual void end() noexcept {
            LOG_VERBOSE("Bob");
        }
    };
}

agm::Thread *createBob(
    Worm *worm
) noexcept {
    auto th = new(std::nothrow) Bob;
    th->worm_ = worm;
    return th;
}
