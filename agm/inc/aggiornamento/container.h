/*
Copyright (C) 2012-2016 tim cotter. All rights reserved.
*/

/**
container class interface.

tbd
**/

#pragma once

#include <aggiornamento/aggiornamento.h>

#include <string>


namespace agm {
    class Container {
    public:
        Container(const char *name) throw();
        Container(const Container &) = delete;
        virtual ~Container() throw();

        /*
        returns the name of the container.
        */
        std::string getName() const throw();

        /*
        called by master thread.
        */
        virtual void init() throw() {
        }

        /*
        called by master thread.
        unblock all threads that might be waiting
        for data stored in this container.
        stop blocking threads forever.
        */
        virtual void unblock() throw() {
        }

        /*
        called by master thread.
        */
        virtual void exit() throw() {
        }

    private:
        std::string name_;
    };
}
