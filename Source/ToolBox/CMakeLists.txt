# Add all the folders for the toolbox/utilities system

add_subdirectory (IdGenerator)
add_subdirectory (ProcessVM)
add_subdirectory (SystemTest)
add_subdirectory (VoxelToMesh)

if (NOT GPU_PACKAGE OR GPU_PACKAGE STREQUAL "OpenGL")
	add_subdirectory (GLTextureAtlas)
	add_subdirectory (GLViewVM)
endif ()