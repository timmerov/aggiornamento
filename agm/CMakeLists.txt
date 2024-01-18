#
# Copyright (C) 2012-2024 tim cotter. All rights reserved.
#

# search for "you need to"

# you need to set these.
set(THIS_TARGET_NAME agm)

# set the target.
set(THIS_LIB_TYPE STATIC)
message("-- Adding ${THIS_LIB_TYPE} library ${THIS_TARGET_NAME}...")

# gather the source files.
file(GLOB_RECURSE THIS_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h)

# gather the header files.
file(GLOB_RECURSE THIS_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/inc/*.h)

# create the library
add_library(${THIS_TARGET_NAME} ${THIS_LIB_TYPE} ${THIS_SOURCE} ${THIS_INCLUDES})

# define the target in the source
target_compile_definitions(${THIS_TARGET_NAME} PRIVATE AGM_TARGET=${THIS_TARGET_NAME})

# add _d to the debug target name
set_target_properties(${THIS_TARGET_NAME} PROPERTIES DEBUG_POSTFIX _d)

# put the library in the agm/lib directory
set_target_properties(${THIS_TARGET_NAME} PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/agm/lib
)

# add the include directores
set(INCLUDES
    inc
)
include_directories(${INCLUDES})
