/*
Copyright (C) 2012-2016 tim cotter. All rights reserved.
*/

#pragma once

#if !defined(INCLUDED_AGM_HPP)
#error you must #include <aggiornamento/aggiornamento.h>
#endif

// speed up builds
#if !defined(WIN32_LEAN_AND_MEAN)
#define WIN32_LEAN_AND_MEAN
#endif

// this interferes with c++11 std::max et al
#if !defined(NOMINMAX)
#define NOMINMAX 1
#endif

// windows implementation
#include <windows.h>
