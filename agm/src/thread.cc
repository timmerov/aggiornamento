/*
Copyright (C) 2012-2016 tim cotter. All rights reserved.
*/

/*
*/

#include <aggiornamento/aggiornamento.h>
#include <aggiornamento/thread.h>

// use an anonymous namespace to avoid name collisions at link time
namespace agm {
    void runThread(agm::Thread *thread) throw() {
        thread->begin();
        thread->begin_cv_.notify_one();
        {
            std::unique_lock<std::mutex> lock(thread->mutex_);
            thread->start_cv_.wait(lock);
        }
        thread->run();
        thread->run_cv_.notify_one();
        thread->drain();
        thread->end();
    }
}

agm::Thread::Thread() throw() :
    thread_(nullptr),
    mutex_(),
    begin_cv_(),
    start_cv_(),
    run_cv_(),
    is_producing_(true),
    is_draining_(true) {
}

agm::Thread::~Thread() throw() {
    /*
    at this point thread_ should be null.
    probably want to whine if it isn't.
    */
}

void agm::Thread::init() throw() {
    thread_ = new std::thread(runThread, this);
    {
        std::unique_lock<std::mutex> lock(mutex_);
        begin_cv_.wait(lock);
    }
}

void agm::Thread::startProducing() throw() {
    start_cv_.notify_one();
}

void agm::Thread::stopProducing() throw() {
    is_producing_ = false;
    unblock();
    {
        std::unique_lock<std::mutex> lock(mutex_);
        run_cv_.wait(lock);
    }
}

void agm::Thread::stopCompletely() throw() {
    is_draining_ = false;
    unblock();
    thread_->join();
    delete thread_;
    thread_ = nullptr;
}

void agm::Thread::startAll(
    std::vector<Thread *> threads
) throw() {
    for (auto th: threads) {
        th->init();
    }
    for (auto th: threads) {
        th->startProducing();
    }
}

void agm::Thread::stopAll(
    std::vector<Thread *> threads
) throw() {
    for (auto th = threads.rbegin(); th != threads.rend(); ++th) {
        (*th)->stopProducing();
    }
    for (auto th = threads.rbegin(); th != threads.rend(); ++th) {
        (*th)->stopCompletely();
    }
    for (auto th = threads.rbegin(); th != threads.rend(); ++th) {
        delete *th;
        *th = nullptr;
    }
}

bool agm::Thread::isProducing() throw() {
    return is_producing_;
}

bool agm::Thread::isDraining() throw() {
    return is_draining_;
}

void agm::Thread::run() throw() {
    while (isProducing()) {
        runOnce();
    }
}

void agm::Thread::drain() throw() {
    while (isDraining()) {
        drainOnce();
    }
}
