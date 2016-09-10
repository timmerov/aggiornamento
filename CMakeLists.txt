#
# Copyright (C) 2012-2016 tim cotter. All rights reserved.
#

# search for "you need to"

# you need to set these:
set(THIS_PROJECT_NAME aggiornamento)
set(THIS_CMAKE_DIR "${CMAKE_SOURCE_DIR}/cmake")

# cmake boilerplate
include("${THIS_CMAKE_DIR}/scripts/preamble.txt")

# more cmake boilerplate
# unfortunately, we can't put this in preamble.
cmake_minimum_required(VERSION ${THIS_CMAKE_VERSION})

# add the targets
add_subdirectory(agm)
add_subdirectory(00-helloworld)
add_subdirectory(01-thread)
add_subdirectory(02-single-buffer)
add_subdirectory(03-double-buffer)
