/*
Copyright (C) 2012-2016 tim cotter. All rights reserved.
*/

/**
single buffer example.
trunk interface.

anyone can put a string into or copy a string from the trunk.

multiple producers, multiple consumers.

the string is protected by a lock.
there is no other synchronization.
there is no signalling.
**/

#pragma once

#include <aggiornamento/aggiornamento.h>
#include <aggiornamento/container.h>


class Trunk : public agm::Container {
protected:
    Trunk() noexcept;
public:
    Trunk(const Trunk &) = delete;
    virtual ~Trunk() noexcept;

    /*
    master thread creates the trunk.
    master thread deletes the trunk.
    */
    static Trunk *create(int size) noexcept;

    /*
    return the size of the buffer.
    */
    int getSize() noexcept;

    /*
    overwrites whatever was in the trunk.
    */
    void putString(const char *buffer) noexcept;

    /*
    copies the string from the trunk.
    the string in the trunk is preserved.
    */
    void getString(char *buffer, int size) const noexcept;

    /*
    template divines size of getString buffer
    */
    template <int size>
    void getString(
       char (&buffer)[size]
    ) noexcept {
        getString(buffer, size);
    }

    /*
    Container interface.
    unblock threads.
    we cannot block threads.
    **
    virtual void unblock() noexcept;
    */
};
