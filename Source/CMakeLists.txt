# Top-level CMAKE file responsible for folders in Source directory

################## Set CXX Compile and Linker Flags ###################

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -march=native -Wall -Wextra -Wno-unused-parameter")

# Release
if (CMAKE_BUILD_TYPE STREQUAL "Release")
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE} -O2")
	
# Release with log
elseif (CMAKE_BUILD_TYPE STREQUAL "ReleaseWithLog")
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -O2")
	
# Debug or no specified build type
else ()
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG} -fno-inline")
endif ()

################## Optionally enabled logging output ##################

if (NOT CMAKE_BUILD_TYPE STREQUAL "Release")
	option (SIM_LOG_ENABLED "Log Enabled" ON)
endif ()

############## Set default vector size to be used by GPU ##############

if (NOT VECTOR3_ENABLED OR VECTOR3_ENABLED STREQUAL "OFF")
	option (SIM_VECTOR4_ENABLED "Vector4" ON)
else ()
	option (SIM_VECTOR3_ENABLED "Vector3" ON)
endif ()

################# Optionally set double precision flag ################

if (DOUBLE_PRECISION STREQUAL "ON")
	option (SIM_DOUBLE_PRECISION "Double Precision" ON)
endif ()

############# Set Graphics package to be used by compiler #############

if (NOT GPU_PACKAGE OR GPU_PACKAGE STREQUAL "OpenGL")
	option (SIM_GL_ENABLED "OpenGL" ON)
elseif (GPU_PACKAGE STREQUAL "Vulkan")
	option (SIM_VK_ENABLED "Vulkan" ON)
elseif (GPU_PACKAGE STREQUAL "DirectX")
	option (SIM_DX_ENABLED "DirectX" ON)
endif ()

############# Set Compute package to be used by compiler ##############

if (COMPUTE_PACKAGE STREQUAL "OpenCL")
	option (SIM_CL_ENABLED "OpenCL" ON)
elseif (COMPUTE_PACKAGE STREQUAL "CUDA")
	option (SIM_CUDA_ENABLED "CUDA" ON)
elseif (COMPUTE_PACKAGE STREQUAL "DirectCompute")
	option (SIM_DIRECTCOMPUTE_ENABLED "DirectCompute" ON)
endif ()

############ Set Scheduler package to be used by compiler #############

if (SCHEDULER_PACKAGE STREQUAL "IntelTBB")
	option (SIM_TBB_SCHEDULER_ENABLED "IntelTBB" ON)
elseif (SCHEDULER_PACKAGE STREQUAL "Threads")
	option (SIM_THREAD_SCHEDULER_ENABLED "Threads" ON)
endif ()

################# Find Platform-independent libraries #################

find_library (XML_LIB tinyxml2 PATHS ${CMAKE_SOURCE_DIR}/Lib NO_DEFAULT_PATH)
message (STATUS "")
message (STATUS "XML library: " ${XML_LIB})

find_library (PNG_LIB lodepng PATHS ${CMAKE_SOURCE_DIR}/Lib NO_DEFAULT_PATH)
message (STATUS "")
message (STATUS "PNG library: " ${PNG_LIB})

find_library (TIFF_LIB tinytiff PATHS ${CMAKE_SOURCE_DIR}/Lib NO_DEFAULT_PATH)
message (STATUS "")
message (STATUS "TIFF library: " ${TIFF_LIB})

################## Add Platform Specific Cmake files ##################

if (APPLE) # MAC OS X
	include (${SIM_SOURCE_DIR}/CMakeLists.MacOSX.txt)
elseif (UNIX) # Linux
	include (${SIM_SOURCE_DIR}/CMakeLists.Linux.txt)
elseif (WIN32) #Windows
	include (${SIM_SOURCE_DIR}/CMakeLists.Win32.txt)
endif ()

#################### Add source code directories #####################

add_subdirectory (TestKitchen)
add_subdirectory (ToolBox)
add_subdirectory (Apps)
add_subdirectory (Plugins)

######### Set up Config.h.in to define variables in Config.h #########

# IMPORTANT: This line should be the last line after all option flags are set
configure_file (${SIM_SOURCE_DIR}/Common/Config.h.in ${SIM_SOURCE_DIR}/Common/Config.h)
