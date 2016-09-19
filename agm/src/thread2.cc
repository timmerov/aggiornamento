/*
Copyright (C) 2012-2016 tim cotter. All rights reserved.
*/

/**
thread class implementation.

tbd
**/

#include <aggiornamento/aggiornamento.h>
#include <aggiornamento/log.h>
#include <aggiornamento/master.h>
#include <aggiornamento/thread2.h>

// pick one
#undef LOG_VERBOSE
//#define LOG_VERBOSE LOG
#define LOG_VERBOSE(...)


// use an anonymous namespace to avoid name collisions at link time
namespace agm {
    void runThread2(agm::Thread2 *thread) throw() {
        LOG_VERBOSE(thread->getName() << " begin...");
        thread->begin();
        LOG_VERBOSE(thread->getName() << " begin done, wait start...");
        thread->begin_sem_.signal();
        thread->start_sem_.waitConsume();
        LOG_VERBOSE(thread->getName() << " got start, run...");
        thread->run();
        LOG_VERBOSE(thread->getName() << " run done, end...");
        thread->is_running_ = false;
        thread->end();
        LOG_VERBOSE(thread->getName() << " end done");
    }
}

agm::Thread2::Thread2(
    const char *name
) throw() :
    name_(name),
    thread_(nullptr),
    begin_sem_(),
    start_sem_(),
    is_running_(true) {
}

agm::Thread2::~Thread2() throw() {
    /*
    at this point thread_ should be null.
    probably want to whine if it isn't.
    */
}

std::string agm::Thread2::getName() const throw() {
    return name_;
}

void agm::Thread2::init() throw() {
    thread_ = new std::thread(runThread2, this);
    begin_sem_.waitConsume();
}

void agm::Thread2::start() throw() {
    start_sem_.signal();
}

void agm::Thread2::stop() throw() {
    is_running_ = false;
}

void agm::Thread2::waitExit() throw() {
    is_running_ = false;
    if (thread_->joinable()) {
        thread_->join();
    }
    delete thread_;
    thread_ = nullptr;
}

void agm::Thread2::startAll(
    std::vector<Thread2 *> &threads,
    std::vector<Container *> &containers
) throw() {
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

void agm::Thread2::stopAll(
    std::vector<Thread2 *> &threads,
    std::vector<Container *> &containers
) throw() {
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

void agm::Thread2::runAll(
    std::vector<Thread2 *> &threads,
    std::vector<Container *> &containers
) throw() {
    startAll(threads, containers);
    master::waitDone();
    stopAll(threads, containers);
}

bool agm::Thread2::isRunning() throw() {
    return is_running_;
}

void agm::Thread2::run() throw() {
    while (isRunning()) {
        runOnce();
    }
}
