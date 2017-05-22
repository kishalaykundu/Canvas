# Cmake file for the Canvas Linux Application layer
project (APP CXX)

# Set include directory paths
include_directories (
	${GPU_INCLUDE_PATH}
	${COMPUTE_INCLUDE_PATH}
	${SCHEDULER_INCLUDE_PATH}
	${SIM_SOURCE_DIR}/Packages/FastCallback
	${SIM_SOURCE_DIR}/Packages/TBB/include
	${SIM_SOURCE_DIR}/Packages/TinyXML
	${SIM_SOURCE_DIR}/Common
	${SIM_SOURCE_DIR}/Core
	${SIM_SOURCE_DIR}/Apps)

# Set essential library links
set (APP_REQUIRED_LIBS ${APP_REQUIRED_LIBS} ${DL_LIB} ${MATH_LIB} ${THREAD_LIB} ${XML_LIB})
		
# Add OpenGL libraries if they are enabled
if (NOT GPU_PACKAGE OR GPU_PACKAGE STREQUAL "OpenGL")
	set (APP_REQUIRED_LIBS ${APP_REQUIRED_LIBS} ${OPENGL_LIBS})
endif ()

# Add CUDA or OpenCL libraries if they are enabled
if (COMPUTE_PACKAGE STREQUAL "CUDA")
	set (APP_REQUIRED_LIBS ${APP_REQUIRED_LIBS} ${CUDA_LIBS})
endif ()

if (COMPUTE_PACKAGE STREQUAL "OpenCL")
	set (APP_REQUIRED_LIBS ${APP_REQUIRED_LIBS} ${CL_LIB})
endif ()

# Add TBB Scheduler library if it is enabled
if (SCHEDULER_PACKAGE STREQUAL "IntelTBB")
	set (APP_REQUIRED_LIBS ${APP_REQUIRED_LIBS} ${TBB_LIBS})
endif ()
	
# Set Core directory path
set (SIM_CORE_DIR ${SIM_SOURCE_DIR}/Core)

# Add source files from other folders
set (APP_SRCS ${APP_SRCS}
	${SIM_SOURCE_DIR}/Common/GL/GLUtils.cpp
	${SIM_SOURCE_DIR}/Common/CUDA/CUDAUtils.cpp
	${SIM_SOURCE_DIR}/Common/Vector.cpp
	${SIM_SOURCE_DIR}/Common/ConfigParser.cpp)

# Add core source files
file (GLOB ASSETS_DIR_SRCS "${SIM_CORE_DIR}/Asset/*.cpp")
file (GLOB DRIVER_DIR_SRCS "${SIM_CORE_DIR}/Driver/*.cpp")
file (GLOB GRAPHICS_DIR_SRCS "${SIM_CORE_DIR}/Render/*.cpp")
file (GLOB EVENTS_DIR_SRCS "${SIM_CORE_DIR}/Events/*.cpp")
file (GLOB HPC_DIR_SRCS "${SIM_CORE_DIR}/Compute/*.cpp")
file (GLOB PLUGINS_DIR_SRCS "${SIM_CORE_DIR}/Plugin/*.cpp")
file (GLOB TASKS_DIR_SRCS "${SIM_CORE_DIR}/Tasks/*.cpp")

set (APP_SRCS ${APP_SRCS}
	${ASSETS_DIR_SRCS}
	${DRIVER_DIR_SRCS}
	${GRAPHICS_DIR_SRCS}
	${EVENTS_DIR_SRCS}
	${HPC_DIR_SRCS}
	${PLUGINS_DIR_SRCS}
	${TASKS_DIR_SRCS})

# Add application specific source files
if (NOT GPU_PACKAGE OR GPU_PACKAGE STREQUAL "OpenGL")
	file (GLOB GL45_DIR_SRCS "${SIM_CORE_DIR}/Render/GL45/*.cpp")
	set (APP_SRCS ${APP_SRCS} ${GL45_DIR_SRCS})
	
elseif (GPU_PACKAGE STREQUAL "Vulkan")
	file (GLOB VK_DIR_SRCS "${SIM_CORE_DIR}/Render/VK/*.cpp")
	set (APP_SRCS ${APP_SRCS} ${VK_DIR_SRCS})

endif ()


if (COMPUTE_PACKAGE STREQUAL "CUDA")
	file (GLOB CUDA_DIR_SRCS "${SIM_CORE_DIR}/Compute/Cuda/*.cpp")
	set (APP_SRCS ${APP_SRCS} ${CUDA_DIR_SRCS})
	
elseif (COMPUTE_PACKAGE STREQUAL "CL")
	file (GLOB CUDA_DIR_SRCS "${SIM_CORE_DIR}/Compute/CL/*.cpp")
	set (APP_SRCS ${APP_SRCS} ${CL_DIR_SRCS})
	
endif ()

if (SCHEDULER_PACKAGE STREQUAL "IntelTBB")
	file (GLOB TBB_DIR_SRCS "${SIM_CORE_DIR}/Tasks/TBB/*.cpp")
	set (APP_SRCS ${APP_SRCS} ${TBB_DIR_SRCS})
	
elseif (SCHEDULER_PACKAGE STREQUAL "Threads")
	file (GLOB THREAD_DIR_SRCS "${SIM_CORE_DIR}/Tasks/Threads/*.cpp")
	set (APP_SRCS ${APP_SRCS} ${THREAD_DIR_SRCS})
endif ()

file (GLOB APP_DIR_SRCS "${SIM_SOURCE_DIR}/Apps/LinuxApp/*.cpp")
set (APP_SRCS ${APP_SRCS} ${APP_DIR_SRCS})

# Set and link target
add_executable (simulate ${APP_SRCS})
target_link_libraries (simulate ${APP_REQUIRED_LIBS})
install (TARGETS simulate DESTINATION Bin)

# Set compiler flags in addition to the globally set ones
set (APP_COMPILE_FLAGS "${CMAKE_CXX_FLAGS} -rdynamic -DU_SHOW_CPLUSPLUS_API=0")
set_target_properties (simulate PROPERTIES COMPILE_FLAGS ${APP_COMPILE_FLAGS})
	