/*
Copyright (C) 2012-2016 tim cotter. All rights reserved.
*/

/*
thread class implementation.

=== caution ===
the functions in class are called by different threads.
a clear violation of the best practices.
every rule has its exception.
the functions clearly document the intended calling thread.

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
    waits begin_cv
                            begin()
                            begin_cv
                            wait start_cv
startProducing()
    set start_cv
                            run()
                                while isProducing
                                    runOnce()
...                         ...
stopProducing()
    isProducing = false
    unblock()
    wait run_cv
                            set run_cv
                            drain()
                                while isDraining
                                    drainOnce()
stopCompletely
    isDraining = false
    unblock()
    join
                            end()
*/

#pragma once

#include <aggiornamento/aggiornamento.h>

#include <condition_variable>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>


namespace agm {
    class Thread {
    public:
        Thread() throw();
        Thread(const Thread &) = delete;
        virtual ~Thread() throw();

        /**** api for the master thread that creates this thread ****/

        /*
        called by master thread.
        starts the thread and waits for its begin function to return.
        the thread's begin function is called.
        the thread is blocked until startProducing is called.
        */
        void init() throw();

        /*
        called by master thread.
        tells the thread to start producing data.
        the thread's run function is called.
        */
        void startProducing() throw();

        /*
        called by master thread.
        tells the thread to stop producing data.
        calls the thread's unblock function.
        waits for the thread's run function to return.
        the thread's drain function is called.
        */
        void stopProducing() throw();

        /*
        called by master thread.
        tells the thread to stop draining data and waits for the thread
        to join (terminate).
        calls the thread's unblock function.
        the thread's end function is called.
        */
        void stopCompletely() throw();

        /*
        called by master thread.
        utility function
        initialize and run a vector of threads.
        */
        static void startAll(std::vector<Thread *> threads) throw();

        /*
        called by master thread.
        utility function
        stop and delete a vector of threads.
        */
        static void stopAll(std::vector<Thread *> threads) throw();

        /**** api for the created thread ****/

        bool isProducing() throw();
        bool isDraining() throw();

        /**** api that can be implemented by the created thread. ****/

        /*
        called by created thread.
        perform initialization like allocate memory.
        do not produce nor consume data.
        */
        virtual void begin() throw() {
        }

        /*
        called by created thread.
        default implementation is to call
        runOnce as long as isProducing is true;
        */
        virtual void run() throw();

        /*
        called by created thread.
        called by the default implementation of run
        as long as isProducing is true.

        === caution ===
        runOnce should sleep or block if it has nothing to do.
        otherwise the thread could busy-loop.
        */
        virtual void runOnce() throw() {
        }

        /*
        called by created thread.
        default implementation is to call
        drainOnce as long as isDraining is true
        */
        virtual void drain() throw();

        /*
        called by created thread.
        call by the default implementation of drain
        as long as isDraining is true.

        === caution ===
        drainOnce should sleep or block if it has nothing to do.
        otherwise the thread could busy-loop.
        */
        virtual void drainOnce() throw() {
        }

        /*
        === WARNING ===
        called by master thread.
        ===============

        called by the master thread to unblock the thread.
        the implementation of run, runOnce, drain, drainOnce
        may block waiting for data.
        this function should unblock the thread so the thread
        run and drain functions can detect they should finish.
        */
        virtual void unblock() throw() {
        }

        /*
        called by created thread.
        perform de-initialization.
        like free memory.
        do not produce nor consume data.
        */
        virtual void end() throw() {
        }

    private:
        std::thread *thread_;
        std::mutex mutex_;
        std::condition_variable begin_cv_;
        std::condition_variable start_cv_;
        std::condition_variable run_cv_;
        bool is_producing_;
        bool is_draining_;

        friend void runThread(Thread *thread) throw();
    };
}
