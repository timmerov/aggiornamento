#
# Copyright (C) 2012-2016 tim cotter. All rights reserved.
#

# search for "you need to"

# you need to set these:
set(THIS_EXE_NAME single-buffer)

# log it
message("-- Adding executable ${THIS_EXE_NAME}...")

# gather the source files.
file(GLOB_RECURSE THIS_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/*.cc)

# gather the header files.
file(GLOB_RECURSE THIS_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

# add the executable with source and includes in separate groups
add_executable(${THIS_EXE_NAME} ${THIS_SOURCE} ${THIS_HEADERS})

# add _d to the debug target name
set_target_properties(${THIS_EXE_NAME} PROPERTIES DEBUG_POSTFIX _d)

# add the include directories
set(INCS
    ${CMAKE_CURRENT_SOURCE_DIR}/shared
    ${CMAKE_SOURCE_DIR}/agm/inc
)
include_directories(${INCS})

# add the libraries
set(LIBS
    agm
)
target_link_libraries(${THIS_EXE_NAME} ${LIBS})
