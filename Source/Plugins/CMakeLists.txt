# Top level Cmake file for the Plugins folder

if ((NOT GPU_PACKAGE OR GPU_PACKAGE STREQUAL "OpenGL") AND (COMPUTE_PACKAGE STREQUAL "CUDA"))
	add_subdirectory (CuglMsd)
endif ()
