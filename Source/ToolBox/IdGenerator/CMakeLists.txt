# Cmake file for the IdGenerator
project (IDGEN CXX)

# Set include directories
include_directories (./ ${SIM_SOURCE_DIR}/Common ${SIM_SOURCE_DIR}/Packages/TinyXML/)

# Set linked libraries
set (IDGEN_REQUIRED_LIBS ${XML_LIB})

# Set source files
set (IDGEN_SRCS
	${SIM_SOURCE_DIR}/Common/ConfigParser.cpp
	./UIDGenerator.cpp
	./main.cpp)

# Set and link target
add_executable (generateIds ${IDGEN_SRCS})
target_link_libraries (generateIds ${IDGEN_REQUIRED_LIBS})
install (TARGETS generateIds DESTINATION Bin)

# Set compiler flags in addition to the globally set ones
set (IDGEN_COMPILE_FLAGS ${CMAKE_CXX_FLAGS})
set_target_properties (generateIds PROPERTIES COMPILE_FLAGS ${IDGEN_COMPILE_FLAGS})
