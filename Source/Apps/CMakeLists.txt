# Cmake file to make OS-specific driven Canvas simulation application

if (UNIX)
	add_subdirectory (LinuxApp)
endif ()
