/*
Copyright (C) 2012-2016 tim cotter. All rights reserved.
*/

/**
thread class implementation.

the thread function operates something like this:
master                      thread
--------------------        --------------------
init()
    starts thread
    waits begun_sem
                            begin()
                            signal begun_sem
                            wait start_sem
start()
    signal start_sem
                            run()
                                while isRunning
                                    runOnce()
stop()
    isRunning = false
                            maybe waiting for data
containers unblock threads
                            runOnce exits
                            run exits
                            wait finish_sem
waitExit()
    signal finish_sem
    join
                            end()
**/

#include <aggiornamento/aggiornamento.h>
#include <aggiornamento/log.h>
#include <aggiornamento/master.h>
#include <aggiornamento/thread.h>

// pick one
#undef LOG_VERBOSE
//#define LOG_VERBOSE LOG
#define LOG_VERBOSE(...)


// use an anonymous namespace to avoid name collisions at link time
namespace agm {
    void runThread2(agm::Thread *thread) noexcept {
        LOG_VERBOSE(thread->getName() << " begin...");
        thread->begin();
        LOG_VERBOSE(thread->getName() << " begin done, wait start...");
        thread->begun_sem_.signal();
        thread->start_sem_.waitConsume();
        LOG_VERBOSE(thread->getName() << " got start, run...");
        thread->run();
        LOG_VERBOSE(thread->getName() << " run done, wait finish...");
        thread->is_running_ = false;
        thread->finish_sem_.waitConsume();
        LOG_VERBOSE(thread->getName() << " got finish. end...");
        thread->end();
        LOG_VERBOSE(thread->getName() << " end done");
    }
}

agm::Thread::Thread(
    const char *name
) noexcept :
    name_(name),
    thread_(nullptr),
    begun_sem_(),
    start_sem_(),
    finish_sem_(),
    is_running_(true) {
}

agm::Thread::~Thread() noexcept {
    /*
    at this point thread_ should be null.
    probably want to whine if it isn't.
    */
}

std::string agm::Thread::getName() const noexcept {
    return name_;
}

void agm::Thread::init() noexcept {
    thread_ = new std::thread(runThread2, this);
    begun_sem_.waitConsume();
}

void agm::Thread::start() noexcept {
    start_sem_.signal();
}

void agm::Thread::stop() noexcept {
    is_running_ = false;
}

void agm::Thread::waitExit() noexcept {
    finish_sem_.signal();
    if (thread_->joinable()) {
        thread_->join();
    }
    delete thread_;
    thread_ = nullptr;
}

void agm::Thread::startAll(
    std::vector<Thread *> &threads,
    std::vector<Container *> &containers
) noexcept {
    for (auto it: containers) {
        LOG_VERBOSE(it->getName() << " init...");
        it->init();
        LOG_VERBOSE(it->getName() << " init done");
    }
    for (auto th: threads) {
        LOG_VERBOSE(th->getName() << " init...");
        th->init();
        LOG_VERBOSE(th->getName() << " init done");
    }
    for (auto th: threads) {
        LOG_VERBOSE(th->getName() << " start ...");
        th->start();
        LOG_VERBOSE(th->getName() << " start done");
    }
}

void agm::Thread::stopAll(
    std::vector<Thread *> &threads,
    std::vector<Container *> &containers
) noexcept {
    for (auto it = threads.rbegin(); it != threads.rend(); ++it) {
        auto th = *it;
        LOG_VERBOSE(th->getName() << " stop...");
        th->stop();
        LOG_VERBOSE(th->getName() << " stop done");
    }
    for (auto it: containers) {
        LOG_VERBOSE(it->getName() << " unblock...");
        it->unblock();
        LOG_VERBOSE(it->getName() << " unblock done");
    }
    for (auto it = threads.rbegin(); it != threads.rend(); ++it) {
        auto th = *it;
        LOG_VERBOSE(th->getName() << " wait exit...");
        th->waitExit();
        LOG_VERBOSE(th->getName() << " wait exit done...");
    }
    for (auto it = threads.rbegin(); it != threads.rend(); ++it) {
        auto th = *it;
        auto name = th->getName();
        LOG_VERBOSE(name << " delete...");
        delete th;
        *it = nullptr;
        LOG_VERBOSE(name << " delete done...");
    }
    for (auto it = containers.rbegin(); it != containers.rend(); ++it) {
        auto can = *it;
        auto name = can->getName();
        LOG_VERBOSE(name << " delete...");
        delete can;
        *it = nullptr;
        LOG_VERBOSE(name << " delete done...");
    }
}

void agm::Thread::runAll(
    std::vector<Thread *> &threads,
    std::vector<Container *> &containers
) noexcept {
    startAll(threads, containers);
    master::waitDone();
    stopAll(threads, containers);
}

bool agm::Thread::isRunning() noexcept {
    return is_running_;
}

void agm::Thread::run() noexcept {
    while (isRunning()) {
        runOnce();
    }
}
