#
# Copyright (C) 2012-2016 tim cotter. All rights reserved.
#

# search for "you need to"

# you need to set these:
set(THIS_PROJECT_NAME aggiornamento)
set(THIS_CMAKE_DIR "${CMAKE_SOURCE_DIR}/cmake")

# cmake boilerplate
include("${THIS_CMAKE_DIR}/scripts/preamble.txt")
include("${THIS_CMAKE_DIR}/scripts/source_glob.txt")

# more cmake boilerplate
# unfortunately, we can't put this in preamble.
cmake_minimum_required(VERSION ${THIS_CMAKE_VERSION})

# add the targets
add_subdirectory(amlib)
add_subdirectory(00-helloworld)
