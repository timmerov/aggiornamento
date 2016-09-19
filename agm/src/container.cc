/*
Copyright (C) 2012-2016 tim cotter. All rights reserved.
*/

/**
container class implementation.

tbd
**/

#include <aggiornamento/aggiornamento.h>
#include <aggiornamento/log.h>
#include <aggiornamento/container.h>

// pick one
#undef LOG_VERBOSE
#define LOG_VERBOSE LOG
//#define LOG_VERBOSE(...)


agm::Container::Container(
    const char *name
) throw() :
    name_(name) {
}

agm::Container::~Container() throw() {
}

std::string agm::Container::getName() const throw() {
    return name_;
}
