/*
Copyright (C) 2012-2016 tim cotter. All rights reserved.
*/

/**
thread class interface.

tbd

the thread function operates something like this:
master                      thread
--------------------        --------------------
init()
    starts thread
    waits begin_sem
                            begin()
                            signal begin_sem
                            wait start_sem
start()
    signal start_sem
                            run()
                                while isDraining
                                    runOnce()


stop()
    isDraining = false

unblock containers

waitExit()
    join
                            end()
**/

#pragma once

#include <aggiornamento/aggiornamento.h>
#include <aggiornamento/container.h>
#include <aggiornamento/thread.h>

#include <condition_variable>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>


namespace agm {
    class Thread2 {
    public:
        Thread2(const char *name) throw();
        Thread2(const Thread2 &) = delete;
        virtual ~Thread2() throw();

        /**** public api ****/

        /*
        returns the name of the thread.
        */
        std::string getName() const throw();

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
        tells the thread to start.
        the thread's run function is called.
        */
        void start() throw();

        /*
        called by master thread.
        may be called by the thread.
        tells the thread to stop.
        does not wait for the thread to exit (join, terminate).
        */
        void stop() throw();

        /*
        called by the master thread.
        waits for the thread to exit (join, teminate).
        */
        void waitExit() throw();

        /*
        called by master thread.
        utility function
        initialize and run a vector of threads and containers.
        */
        static void startAll(
            std::vector<Thread2 *> &threads,
            std::vector<Container *> &containers
        ) throw();

        /*
        called by master thread.
        utility function
        stop and delete a vector of threads and containers.
        */
        static void stopAll(
            std::vector<Thread2 *> &threads,
            std::vector<Container *> &containers
        ) throw();

        /*
        called by master thread.
        utility function
        starts a vector of threads and containers.
        waits for one of them to stop the master thread.
        stops them.
        */
        static void runAll(
            std::vector<Thread2 *> &threads,
            std::vector<Container *> &containers
        ) throw();

        /**** api for the created thread ****/

        /*
        tells run and drain to stop.
        */
        bool isRunning() throw();

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
        runOnce as long as isRunning is true;
        */
        virtual void run() throw();

        /*
        called by created thread.
        called by the default implementation of run
        as long as isRunning is true.

        === caution ===
        runOnce should sleep or block if it has nothing to do.
        otherwise the thread could busy-loop.
        */
        virtual void runOnce() throw() {
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
        std::string name_;
        std::thread *thread_;
        Semaphore begin_sem_;
        Semaphore start_sem_;
        bool is_running_;

        friend void runThread2(Thread2 *thread) throw();
    };
}
