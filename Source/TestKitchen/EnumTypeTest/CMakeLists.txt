# Cmake file for the IdGenerator
project (TET CXX)

# Set include directories
include_directories (./ ${SIM_SOURCE_DIR}/Common)

# Set source files
set (TET_SRCS ./main.cpp)

# Set and link target
add_executable (enumtest ${TET_SRCS})
install (TARGETS enumtest DESTINATION Bin)

# Set compiler flags in addition to the globally set ones
set (TET_COMPILE_FLAGS ${CMAKE_CXX_FLAGS})
set_target_properties (enumtest PROPERTIES COMPILE_FLAGS ${TET_COMPILE_FLAGS})