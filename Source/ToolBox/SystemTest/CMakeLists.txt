# Cmake file for the SystemTest
project (SYSTEST CXX)

# Set include directories
include_directories (
	${GPU_INCLUDE_PATH} 
	${COMPUTE_INCLUDE_PATH}
	${SIM_SOURCE_DIR}/Common)
	
# Set dependent libraries
if (NOT GPU_PACKAGE OR GPU_PACKAGE STREQUAL "OpenGL")
	set (ST_REQUIRED_LIBS ${ST_REQUIRED_LIBS} ${OPENGL_LIBS})
elseif (GPU_PACKAGE STREQUAL "Vulkan")
	set (ST_REQUIRED_LIBS ${ST_REQUIRED_LIBS} ${VULKAN_LIBS})
elseif (GPU_PACKAGE STREQUAL "DirectX")
	set (ST_REQUIRED_LIBS ${ST_REQUIRED_LIBS} ${DIRECTX_LIBS})
endif ()

if (COMPUTE_PACKAGE STREQUAL "CUDA")
	set (ST_REQUIRED_LIBS ${ST_REQUIRED_LIBS} ${CUDA_LIBS})
endif ()
if (COMPUTE_PACKAGE STREQUAL "OpenCL")
	set (ST_REQUIRED_LIBS ${ST_REQUIRED_LIBS} ${CL_LIBS})
endif ()

# Set source files
set (ST_SRCS ./main.cpp)

# Set and link target
add_executable (systest ${ST_SRCS})
target_link_libraries (systest ${ST_REQUIRED_LIBS})
install (TARGETS systest DESTINATION Bin)

# Set compiler flags in addition to the globally set ones
set (ST_COMPILE_FLAGS ${CMAKE_CXX_FLAGS})
set_target_properties (systest PROPERTIES COMPILE_FLAGS ${ST_COMPILE_FLAGS})
