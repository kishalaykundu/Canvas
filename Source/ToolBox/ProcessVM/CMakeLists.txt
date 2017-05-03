# Cmake file for the ProcessVM
project (PVM CXX)

# Set include directories
include_directories (
	./
	${SIM_SOURCE_DIR}/Packages/TinyXML
	${SIM_SOURCE_DIR}/Packages/TinyTIFF
	${SIM_SOURCE_DIR}/Packages/LodePNG
	${SIM_SOURCE_DIR}/Common)
	
# Set dependent libraries
set (PVM_REQUIRED_LIBS ${PVM_REQUIRED_LIBS} ${XML_LIB} ${PNG_LIB} ${TIFF_LIB})

# Set source files
set (PVM_SRCS
	${SIM_SOURCE_DIR}/Common/ConfigParser.cpp
	${SIM_SOURCE_DIR}/Common/PNGUtils.cpp
	./VMProcessor.cpp
	./main.cpp)

# Set and link target
add_executable (processvm ${PVM_SRCS})
target_link_libraries (processvm ${PVM_REQUIRED_LIBS})
install (TARGETS processvm DESTINATION Bin)

# Set compiler flags in addition to the globally set ones
set (PVM_COMPILE_FLAGS ${CMAKE_CXX_FLAGS})
set_target_properties (processvm PROPERTIES COMPILE_FLAGS ${PVM_COMPILE_FLAGS})
	