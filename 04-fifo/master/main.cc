/*
Copyright (C) 2012-2016 tim cotter. All rights reserved.
*/

/**
message queue example.

two thread alice and bob exchange messages via a queue.
**/

#include <aggiornamento/aggiornamento.h>
#include <aggiornamento/log.h>
#include <aggiornamento/thread.h>
#include <container/message-queue.h>


extern agm::Thread *createAlice(MessageQueue *db);
extern agm::Thread *createBob(MessageQueue *db);

// use anonymous namespace to avoid collisions at link time.
namespace {
    static const int kMessageSize = 100;
    static const int kMaxNumMessages = 10;
}

int main(
    int argc, char *argv[]
) throw() {
    (void) argc;
    (void) argv;

    agm::log::init(AGM_TARGET_NAME ".log");

    // create the containers.
    auto message_queue = MessageQueue::create(kMessageSize, kMaxNumMessages);

    // use unique_ptr so they're deleted at end of scope.
    std::unique_ptr<MessageQueue> auto_0(message_queue);

    // create the threads.
    std::vector<agm::Thread *> threads;
    threads.push_back(createAlice(message_queue));
    threads.push_back(createBob(message_queue));

    // run the threads.
    agm::Thread::runAll(threads);

    return 0;
}
