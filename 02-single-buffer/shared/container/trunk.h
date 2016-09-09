/*
Copyright (C) 2012-2016 tim cotter. All rights reserved.
*/

/*
single buffer example.
trunk interface.

anyone can put a string into or copy a string from the trunk.
*/

#pragma once

class Trunk {
protected:
    Trunk();
public:
    Trunk(const Trunk &) = delete;
    virtual ~Trunk();

    // master thread creates the trunk.
    // master thread deletes the trunk.
    static Trunk *create() throw();

    // the string in the trunk is at most this big.
    static const int kMaxStringSize = 100;

    // overwrites whatever was in the trunk.
    void putString(const char *buffer) throw();

    // copies the string from the trunk.
    // the string in the trunk is preserved.
    void getString(char *buffer, int size) const throw();

    // template divines size of getString buffer
    template <int size>
    void getString(
       char (&buffer)[size]
    ) throw() {
        getString(buffer, size);
    }
};
