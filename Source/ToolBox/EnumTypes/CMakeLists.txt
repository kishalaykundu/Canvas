# Cmake file for the EnumTypes that auto-generates the Types.h used in Canvas
project (ENT CXX)

# Set include directories
include_directories (./ ${SIM_SOURCE_DIR}/Common ${SIM_SOURCE_DIR}/Packages/TinyXML/)

# Set linked libraries
set (ENT_REQUIRED_LIBS ${XML_LIB})

# Set source files
set (ENT_SRCS
	${SIM_SOURCE_DIR}/Common/ConfigParser.cpp
	./main.cpp)

# Set and link target
add_executable (generateEnums ${ENT_SRCS})
target_link_libraries (generateEnums ${ENT_REQUIRED_LIBS})
install (TARGETS generateEnums DESTINATION Bin)

# Set compiler flags in addition to the globally set ones
set (ENT_COMPILE_FLAGS ${CMAKE_CXX_FLAGS})
set_target_properties (generateEnums PROPERTIES COMPILE_FLAGS ${ENT_COMPILE_FLAGS})
