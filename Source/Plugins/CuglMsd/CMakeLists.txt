# CMake file for OpenGL-CUDA based Mass Spring Damping simulation library
project (CUGLMSD CXX)

# Add include directories
include_directories (
	${GPU_INCLUDE_PATH}
	${COMPUTE_INCLUDE_PATH}
	${SIM_SOURCE_DIR}/Packages/TinyXML
	${SIM_SOURCE_DIR}/Packages/LodePNG
	${SIM_SOURCE_DIR}/Packages/FastCallback
	./
	${SIM_SOURCE_DIR}/Common/
	${SIM_SOURCE_DIR}/Core/
	${SIM_SOURCE_DIR}/Apps/)

# Set essential library links
set (CUGLMSD_REQUIRED_LIBS ${CUGLMSD_REQUIRED_LIBS} ${MATH_LIB} ${XML_LIB} ${PNG_LIB} ${OPENGL_LIBS} ${CUDA_LIBS})

# Add source files from other folders
set (CUGLMSD_SRCS ${CUGLMSD_SRCS}
	${SIM_SOURCE_DIR}/Common/GL/GLUtils.cpp
	${SIM_SOURCE_DIR}/Common/PNGUtils.cpp
	${SIM_SOURCE_DIR}/Common/Vector.cpp
	${SIM_SOURCE_DIR}/Common/ConfigParser.cpp)

# Add local source files
file (GLOB CUGLMSD_DIR_SRCS "*.cpp")

set (CUGLMSD_SRCS ${CUGLMSD_SRCS} ${CUGLMSD_DIR_SRCS})

# Set library name
if (NOT CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "Debug")
    set (CUGLMSD_LIB "CuglMsd-debug")
else ()
    set (CUGLMSD_LIB "CuglMsd")
endif ()

# Final set up to build the CuglMsd library
add_library (${CUGLMSD_LIB} SHARED ${CUGLMSD_SRCS})
target_link_libraries (${CUGLMSD_LIB} ${CUGLMSD_REQUIRED_LIBS})
install (TARGETS ${CUGLMSD_LIB} DESTINATION Lib)

# Set compiler flags in addition to the globally set ones
set (CUGLMSD_COMPILE_FLAGS ${CMAKE_CXX_FLAGS})
set_target_properties (${CUGLMSD_LIB} PROPERTIES COMPILE_FLAGS ${CUGLMSD_COMPILE_FLAGS})
