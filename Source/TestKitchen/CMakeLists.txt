#if (NOT GPU_PACKAGE OR GPU_PACKAGE STREQUAL "OpenGL" AND COMPUTE_PACKAGE STREQUAL "CUDA")
#	add_subdirectory (CuGLInterop)
#endif ()

add_subdirectory (EnumTypeTest)