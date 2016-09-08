#
# Copyright (C) 2012-2016 tim cotter. All rights reserved.
#

# search for "you need to"

# you need to set these.
set(THIS_LIB_NAME agm)
set(THIS_LIB_PATH aggiornamento)

# set the target.
set(THIS_LIB_TYPE STATIC)
message("-- Adding ${THIS_LIB_TYPE} library ${THIS_LIB_NAME}...")

# gather the source files.
file(GLOB_RECURSE THIS_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h)

# gather the header files.
file(GLOB_RECURSE THIS_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/inc/*.h)

# create the library
add_library(${THIS_LIB_NAME} ${THIS_LIB_TYPE} ${THIS_SOURCE} ${THIS_INCLUDES})

# add _d to the debug target name
set_target_properties(${THIS_LIB_NAME} PROPERTIES DEBUG_POSTFIX _d)

# add the include directores
set(INCLUDES
    inc
)
include_directories(${INCLUDES})
