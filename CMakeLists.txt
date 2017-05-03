# Minimum cmake version set to 3.0
cmake_minimum_required (VERSION 3.0 FATAL_ERROR)

# Set g++ as compiler if on non-Windows platforms
if (UNIX OR APPLE)
set (CMAKE_CXX_COMPILER g++)
endif ()

# Top Level CMakeLists.txt file for the Canvas system
project (SIMULATE CXX)

# Allows non-verbose formats for cmake loop constructs
set (CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS true)

# This allows the wildcard "*" when specifying files
if (COMMAND cmake_policy)
	cmake_policy (SET CMP0060 NEW)
endif (COMMAND cmake_policy)

# Set shared libraries to be built
set (BUILD_SHARED_LIBS ON)

# Set installation path
set (CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR})

# Set the source directory location
set (SIM_SOURCE_DIR ${CMAKE_SOURCE_DIR}/Source/)
add_subdirectory (Source)

###################### ECHO CUSTOM CMAKE OPTIONS (WINDOWS) ######################
###################### FOR THOSE NOT USING INSTALL SCRIPT  ######################
if (WIN32)
	message (STATUS "")
	message (STATUS "\t\t---PLEASE READ CAREFULLY---")
	message (STATUS "")
	message (STATUS "   CMAKE BUILD OPTIONS:")
	message (STATUS "")
	message (STATUS "\t-DCMAKE_BUILD_TYPE=None/Debug/Release/ReleaseWithLog (default: None)")
	message (STATUS "\t-DCMAKE_VERBOSE_MAKEFILE=ON/OFF (default: OFF)")
	message (STATUS "\t-DDOUBLE_PRECISION=ON/OFF (default: OFF)")
	message (STATUS "\t-DWITH_VECTOR3=ON/OFF (default: OFF [Vector4 enabled])")
	message (STATUS "\t-DGPU_PACKAGE=OpenGL/Vulkan/DirectX (default: OpenGL)")
	message (STATUS "\t-DGPU_INCLUDE_PATH=<include-path> (for customizable paths)")
	message (STATUS "\t-DGPU_PACKAGE_LOCATION=<location> (for customizable paths)")
	message (STATUS "\t-DCOMPUTE_PACKAGE=None/CUDA/OpenCL/DirectCompute (default: None)")
	message (STATUS "\t-DCOMPUTE_INCLUDE_PATH=<include-path> (for customizable paths)")
	message (STATUS "\t-DCOMPUTE_PACKAGE_LOCATION=<location> (for customizable paths)")
	message (STATUS "\t-DSCHEDULER_PACKAGE=None/IntelTBB/Threads (default: None)")
	message (STATUS "\t-DSCHEDULER_INCLUDE_PATH=<include-path> (for customizable paths)")
	message (STATUS "\t-DSCHEDULER_PACKAGE_LOCATION=<location> (for customizable paths)")
	message (STATUS "")
endif ()
