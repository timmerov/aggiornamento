#
# Copyright (C) 2012-2024 tim cotter. All rights reserved.
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

# set the project
project(${THIS_PROJECT_NAME})

# add the targets
add_subdirectory(agm)
add_subdirectory(00-helloworld)
add_subdirectory(01-thread)
add_subdirectory(02-single-buffer)
add_subdirectory(03-double-buffer)
add_subdirectory(04-fifo)
add_subdirectory(05-most-recent-out)
add_subdirectory(06-write-once-read-many)
add_subdirectory(07-lock-free-queue)
