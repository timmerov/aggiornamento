/*
Copyright (C) 2012-2016 tim cotter. All rights reserved.
*/

/**
thread class implementation.

=== caution ===
the functions in class are called by different threads.
a clear violation of the best practices.
every rule has its exception.
the functions clearly document the intended calling thread.
=== caution ===

it's possible to split the master thread api from the
implementation api.
well, almost possible.
the unblock function is the exception.
this implementation is easier to understand.

the thread function has four distinct parts called in order.
    initialize
    produce and consume data
    consume and discard (do not produce) data
    clean up

the thread function operates something like this:
master                      thread
--------------------        --------------------
init()
    starts thread
    waits begin_sem
                            begin()
                            signal begin_sem
                            wait start_sem
startProducing()
    signal start_sem
                            run()
                                while isProducing
                                    runOnce()
...                         ...
stopProducing()
    isProducing = false
    unblock()
    wait run_sem
                            signal run_sem
                            drain()
                                while isDraining
                                    drainOnce()
stopCompletely
    isDraining = false
    unblock()
    join
                            end()
**/

#include <aggiornamento/aggiornamento.h>
#include <aggiornamento/log.h>
#include <aggiornamento/master.h>
#include <aggiornamento/thread.h>

// pick one
#undef LOG_VERBOSE
#define LOG_VERBOSE LOG
//#define LOG_VERBOSE(...)


// use an anonymous namespace to avoid name collisions at link time
namespace agm {
    void runThread(agm::Thread *thread) throw() {
        LOG_VERBOSE(thread->getName() << " begin...");
        thread->begin();
        LOG_VERBOSE(thread->getName() << " begin done, wait start...");
        thread->begin_sem_.signal();
        thread->start_sem_.waitConsume();
        LOG_VERBOSE(thread->getName() << " got start, run...");
        thread->run();
        LOG_VERBOSE(thread->getName() << " run done, signal...");
        thread->is_producing_ = false;
        thread->run_sem_.signal();
        LOG_VERBOSE(thread->getName() << " drain...");
        thread->drain();
        LOG_VERBOSE(thread->getName() << " drain done, end...");
        thread->is_draining_ = false;
        thread->end();
        LOG_VERBOSE(thread->getName() << " end done");
    }
}

bool agm::Semaphore::test() throw() {
    return value_;
}

void agm::Semaphore::waitConsume() throw() {
    std::unique_lock<std::mutex> ul(mutex_);
    while (value_ == false) {
        cond_.wait(ul);
    }
    // consume the signal
    value_ = false;
}

void agm::Semaphore::waitPreserve() throw() {
    std::unique_lock<std::mutex> ul(mutex_);
    while (value_ == false) {
        cond_.wait(ul);
    }
    // preserve the signal
    //value_ = false;
}

void agm::Semaphore::signal() throw() {
    /*
    it sure looks weird to set grab a lock simply
    to set a boolean.
    but...
    other threads depend on this value not changing
    while they are looking at it.
    so we can't change it while someone else is
    holding the lock.
    */
    {
        std::unique_lock<std::mutex> ul(mutex_);
        value_ = true;
    }
    cond_.notify_all();
}

agm::Thread::Thread(
    const char *name
) throw() :
    name_(name),
    thread_(nullptr),
    begin_sem_(),
    start_sem_(),
    run_sem_(),
    is_producing_(true),
    is_draining_(true) {
}

agm::Thread::~Thread() throw() {
    /*
    at this point thread_ should be null.
    probably want to whine if it isn't.
    */
}

std::string agm::Thread::getName() const throw() {
    return name_;
}

void agm::Thread::init() throw() {
    thread_ = new std::thread(runThread, this);
    begin_sem_.waitConsume();
}

void agm::Thread::startProducing() throw() {
    start_sem_.signal();
}

void agm::Thread::stopProducing() throw() {
    is_producing_ = false;
    unblock();
    run_sem_.waitConsume();
}

void agm::Thread::stopCompletely() throw() {
    is_draining_ = false;
    unblock();
    if (thread_->joinable()) {
        thread_->join();
    }
    delete thread_;
    thread_ = nullptr;
}

void agm::Thread::startAll(
    std::vector<Thread *> &threads
) throw() {
    for (auto th: threads) {
        LOG_VERBOSE(th->getName() << " init...");
        th->init();
        LOG_VERBOSE(th->getName() << " init done");
    }
    for (auto th: threads) {
        LOG_VERBOSE(th->getName() << " producing ...");
        th->startProducing();
        LOG_VERBOSE(th->getName() << " producing done");
    }
}

void agm::Thread::stopAll(
    std::vector<Thread *> &threads
) throw() {
    for (auto it = threads.rbegin(); it != threads.rend(); ++it) {
        auto th = *it;
        LOG_VERBOSE(th->getName() << " stop producing...");
        th->stopProducing();
        LOG_VERBOSE(th->getName() << " stop producing done");
    }
    for (auto it = threads.rbegin(); it != threads.rend(); ++it) {
        auto th = *it;
        LOG_VERBOSE(th->getName() << " stop completely...");
        th->stopCompletely();
        LOG_VERBOSE(th->getName() << " stop completely done...");
    }
    for (auto it = threads.rbegin(); it != threads.rend(); ++it) {
        auto th = *it;
        auto name = th->getName();
        LOG_VERBOSE(th->getName() << " delete...");
        delete th;
        *it = nullptr;
        LOG_VERBOSE(name << " delete done...");
    }
}

void agm::Thread::runAll(
    std::vector<Thread *> &threads
) throw() {
    startAll(threads);
    master::waitDone();
    stopAll(threads);
}

bool agm::Thread::isProducing() throw() {
    return is_producing_;
}

bool agm::Thread::isDraining() throw() {
    return is_draining_;
}

void agm::Thread::stopProducingSelf() throw() {
    is_producing_ = false;
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
