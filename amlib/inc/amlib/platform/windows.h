/*
Copyright (C) 2012-2016 tim cotter. All rights reserved.
*/

#pragma once

#if !defined(INCLUDED_AMLIB_HPP)
#error you must #include <amlib/amlib.h>
#endif

// this interferes with c++11 std::max et al
#define NOMINMAX 1

// windows implementation
#include <windows.h>

// frikken microsoft
#define AM_THREAD_LOCAL __declspec(thread)
