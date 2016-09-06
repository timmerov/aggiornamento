/*
Copyright (C) 2012-2016 tim cotter. All rights reserved.
*/

#pragma once

// c++ stl

// platform dependencies
#define INCLUDED_AMLIB_HPP
#if defined(AM_WINDOWS)
#include <amlib/platform/windows.h>
#elif defined(AM_UNIX)
#include <amlib/platform/unix.h>
#else
#error Please define your platform in cmake.
#endif

// amlib includes
