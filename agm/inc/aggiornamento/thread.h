/*
Copyright (C) 2012-2016 tim cotter. All rights reserved.
*/

/**
thread class interface.

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

#pragma once

#include <aggiornamento/aggiornamento.h>
#include <aggiornamento/container.h>
#include <aggiornamento/semaphore.h>

#include <condition_variable>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>


namespace agm {
    class Thread {
    public:
        Thread(const char *name) throw();
        Thread(const Thread &) = delete;
        virtual ~Thread() throw();

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
        the thread is blocked until start is called.
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
        does not block the calling thread.
        the thread's run routine should return.
        the thread is blocked until waitExit is called.
        */
        void stop() throw();

        /*
        called by the master thread.
        waits for the thread to exit (join, teminate).
        the thread's end function is called.
        */
        void waitExit() throw();

        /*
        called by master thread.
        utility function
        initialize and run a vector of threads and containers.
        */
        static void startAll(
            std::vector<Thread *> &threads,
            std::vector<Container *> &containers
        ) throw();

        /*
        called by master thread.
        utility function
        stop and delete a vector of threads and containers.
        */
        static void stopAll(
            std::vector<Thread *> &threads,
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
            std::vector<Thread *> &threads,
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
        Semaphore begun_sem_;
        Semaphore start_sem_;
        Semaphore finish_sem_;
        bool is_running_;

        friend void runThread2(Thread *thread) throw();
    };
}
