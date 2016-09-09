/*
Copyright (C) 2012-2016 tim cotter. All rights reserved.
*/

/*
single buffer example.

in this example, alice and bob are threads that can put things
into and take things out of a trunk.
*/

#include <aggiornamento/aggiornamento.h>
#include <container/trunk.h>

#include <condition_variable>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>


// use an anonymous namespace to avoid name collisions at link time.
namespace {
    class Thread {
    public:
        Thread() throw();
        Thread(const Thread &) = delete;
        virtual ~Thread() throw();

        /*
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
            wait join
                                    end()
        */

        /* api for the master thread that creates this one */

        /*
        called by master thread.
        starts the thread and waits for its begin function to return.
        the thread's begin function is called.
        the thread is blocked until startProducing is called.
        */
        void init() throw();

        /*
        called by master thread.
        tells the thread to start producing.
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
        tells the thread to stop draining data and waits until end returns.
        calls the thread's unblock function.
        the thread's end function is called.
        */
        void stopCompletely() throw();

        // initialize and run a vector of threads.
        static void runAll(std::vector<Thread *> threads) throw();

        /* api for the created thread */

        bool isProducing() throw();
        bool isDraining() throw();

        /* api that can be implemented by the created thread. */

        /*
        called by created thread.
        perform initialization.
        like allocate memory.
        do not produce nor consume data.
        */
        virtual void begin() throw() {
        }

        /*
        called by created thread.
        default implementation is to call
        runOnce as long as isProducing() is true;
        */
        virtual void run() throw();

        /*
        called by created thread.
        called by the default implementation of run
        as long as isProducing is true.
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
        */
        virtual void drainOnce() throw() {
        }

        /*
        === WARNING ===
        called by master thread.
        ===
        called by the master thread to unblock the thread.
        the implementation of run, runOnce, drain, drainOnce
        may block waiting for data.
        this function should unblock the thread.
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

    Thread::Thread() throw() :
        thread_(nullptr),
        mutex_(),
        begin_cv_(),
        start_cv_(),
        run_cv_(),
        is_producing_(true),
        is_draining_(true) {
    }

    Thread::~Thread() throw() {
        /*
        at this point thread_ should be null.
        probably want to whine if it isn't.
        */
    }

    void runThread(Thread *thread) throw() {
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

    void Thread::init() throw() {
        thread_ = new std::thread(runThread, this);
        {
            std::unique_lock<std::mutex> lock(mutex_);
            begin_cv_.wait(lock);
        }
    }

    void Thread::startProducing() throw() {
        start_cv_.notify_one();
    }

    void Thread::stopProducing() throw() {
        is_producing_ = false;
        unblock();
        {
            std::unique_lock<std::mutex> lock(mutex_);
            run_cv_.wait(lock);
        }
    }

    void Thread::stopCompletely() throw() {
        is_draining_ = false;
        unblock();
        thread_->join();
        delete thread_;
        thread_ = nullptr;
    }

    void Thread::runAll(
        std::vector<Thread *> threads
    ) throw() {
        for (auto th: threads) {
            th->init();
        }
        for (auto th: threads) {
            th->startProducing();
        }
        for (auto th: threads) {
            th->stopProducing();
        }
        for (auto th: threads) {
            th->stopCompletely();
        }
        for (auto th: threads) {
            delete th;
        }
    }

    bool Thread::isProducing() throw() {
        return is_producing_;
    }

    bool Thread::isDraining() throw() {
        return is_draining_;
    }

    void Thread::run() throw() {
        while (isProducing()) {
            runOnce();
        }
    }

    void Thread::drain() throw() {
        while (isDraining()) {
            drainOnce();
        }
    }

    Thread *createAlice(
        Trunk *trunk
    ) throw() {
        (void) trunk;
        return nullptr;
    }

    Thread *createBob(
        Trunk *trunk
    ) throw() {
        (void) trunk;
        return nullptr;
    }
}

//extern agm::Thread *createAlice(Trunk *trunk);
//extern agm::Thread *createBob(Trunk *trunk);

int main(
    int argc, char *argv[]
) throw() {
    (void) argc;
    (void) argv;

    // create the containers.
    // use unique_ptr so they're deleted at end of scope.
    std::unique_ptr<Trunk> trunk(Trunk::create());

    // create the threads.
    std::vector<Thread *> threads;
    threads.push_back(createAlice(trunk.get()));
    threads.push_back(createBob(trunk.get()));

    Thread::runAll(threads);

    return 0;
}
