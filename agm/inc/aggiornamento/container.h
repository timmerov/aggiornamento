/*
Copyright (C) 2012-2016 tim cotter. All rights reserved.
*/

/**
container class interface.

init is called after everything is created but before
threads are started.

unblock is called to unblock any thread that might
be waiting for data to appear in the container.
containers should not block threads after unblock is called.

exit is called after all threads have stopped but before
any container has been deleted.
**/

#pragma once

#include <aggiornamento/aggiornamento.h>

#include <string>


namespace agm {
    class Container {
    public:
        Container(const char *name) noexcept;
        Container(const Container &) = delete;
        virtual ~Container() noexcept;

        /*
        returns the name of the container.
        */
        std::string getName() const noexcept;

        /*
        called by master thread.
        */
        virtual void init() noexcept {
        }

        /*
        called by master thread.
        unblock all threads that might be waiting
        for data stored in this container.
        stop blocking threads forever.
        */
        virtual void unblock() noexcept {
        }

        /*
        called by master thread.
        */
        virtual void exit() noexcept {
        }

    private:
        std::string name_;
    };
}
