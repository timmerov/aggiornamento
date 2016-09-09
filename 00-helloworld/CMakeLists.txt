#
# Copyright (C) 2012-2016 tim cotter. All rights reserved.
#

# search for "you need to"

# you need to set these:
set(THIS_TARGET_NAME helloworld)

# log it
message("-- Adding executable ${THIS_TARGET_NAME}...")

# gather the source files.
file(GLOB_RECURSE THIS_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc)

# gather the header files.
file(GLOB_RECURSE THIS_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h)

# add the executable with source and includes in separate groups
add_executable(${THIS_TARGET_NAME} ${THIS_SOURCE} ${THIS_HEADERS})

# add _d to the debug target name
set_target_properties(${THIS_TARGET_NAME} PROPERTIES DEBUG_POSTFIX _d)

# define the target in the source
target_compile_definitions(${THIS_TARGET_NAME} PRIVATE AGM_TARGET=${THIS_TARGET_NAME})

# add the include directories
set(INCS
    ${CMAKE_SOURCE_DIR}/agm/inc
)
include_directories(${INCS})

# add the libraries
set(LIBS
    agm
)
target_link_libraries(${THIS_TARGET_NAME} ${LIBS})
