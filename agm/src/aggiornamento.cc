/*
Copyright (C) 2012-2016 tim cotter. All rights reserved.
*/

/**
implementation of utilities and platform abstractions.
**/

#include <aggiornamento/aggiornamento.h>


// this isn't in microsoft's version of c++11. weird.
#if defined(AGM_WINDOWS)
std::ostream & operator<<(
    std::ostream &s,
    const std::string &str
) throw() {
    s << str.c_str();
    return s;
}
#endif
