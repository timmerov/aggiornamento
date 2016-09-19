/*
Copyright (C) 2012-2016 tim cotter. All rights reserved.
*/

/**
implementation interface to the master thread.
tell the master thread to stop all threads.
**/

#include <aggiornamento/aggiornamento.h>
#include <aggiornamento/master.h>

#include <condition_variable>
#include <mutex>


namespace {
    class Master {
    public:
        Master() = default;
        Master(const Master &) = delete;
        ~Master() = default;

        bool done_ = false;
        std::mutex mutex_;
        std::condition_variable done_cv_;

        static Master *getSingleton() throw() {
            static Master g_singleton;
            return &g_singleton;
        };
    };
}

bool agm::master::isDone() throw() {
    auto master = Master::getSingleton();
    return master->done_;
}

void agm::master::setDone() throw() {
    auto master = Master::getSingleton();
    {
        std::unique_lock<std::mutex> lock(master->mutex_);
        master->done_ = true;
    }
    master->done_cv_.notify_all();
}

void agm::master::waitDone() throw() {
    auto master = Master::getSingleton();
    std::unique_lock<std::mutex> lock(master->mutex_);
    while (master->done_ == false) {
        master->done_cv_.wait(lock);
    }
}
