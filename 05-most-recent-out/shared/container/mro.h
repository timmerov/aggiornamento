/*
Copyright (C) 2012-2016 tim cotter. All rights reserved.
*/

/*
most recent out example.
mro interface.

tk
*/

#pragma once


class Mro {
protected:
    Mro();
public:
    Mro(const Mro &) = delete;
    virtual ~Mro();

    /*
    master thread creates the mro.
    master thread deletes the trunk.
    */
    static Mro *create(int size) throw();

    /*
    return the size of the buffer.
    */
    int getSize() throw();

    /*
    overwrites whatever was in the trunk.
    */
    void putString(const char *buffer) throw();

    /*
    copies the string from the trunk.
    the string in the trunk is preserved.
    */
    void getString(char *buffer, int size) const throw();

    /*
    template divines size of getString buffer
    */
    template <int size>
    void getString(
       char (&buffer)[size]
    ) throw() {
        getString(buffer, size);
    }
};
