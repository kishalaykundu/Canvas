/**
 * @file GLUtils.h
 * @author Kishalay Kundu <kishalay.kundu@gmail.com>
 * @section LICENSE
 * See LICENSE.txt included in this package
 *
 * @section DESCRIPTION
 * Platform-dependent OpenGL header files.
 */
#pragma once

#define GL_GLEXT_PROTOTYPES 1

#if defined (__linux__) || defined (__APPLE__) || defined (MACOSX)
#	define GLX_GLXEXT_PROTOTYPES 1
#	define SIM_GLX_MIN_MAJOR_VERSION 1
#	define SIM_GLX_MIN_MINOR_VERSION 4
#endif

extern "C" {

#if defined( __APPLE__ ) || defined( MACOSX )
#	include <OpenGL/gl.h>
#	include <OpenGL/glext.h>
#	include <OpenGL/glx.h>
#	include <OpenGL/glxext.h>
#else
#	include <GL/gl3w.h>
#	include <GL/gl.h>
#	include <GL/glx.h>
#	include <GL/glxext.h>

#endif

#if defined (__linux__) || defined (__APPLE__) || defined (MACOSX)
#	include <X11/X.h>
#	include <X11/Xlib.h>
#endif

}

#define SIM_GL_MIN_MAJOR_VERSION 4
#define SIM_GL_MIN_MINOR_VERSION 5

#define SIM_GLX_MIN_MAJOR_VERSION 1
#define SIM_GLX_MIN_MINOR_VERSION 4

#if defined (__linux__) || defined (__APPLE__) || defined (MACOSX)
	typedef GLXContext GPU_CONTEXT;
#endif

#if defined(GL_DEBUG_TYPE_ERROR) && defined(GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR) && \
	defined(GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR) && defined(GL_DEBUG_TYPE_PORTABILITY) && \
	defined(GL_DEBUG_TYPE_PERFORMANCE) && defined(GL_DEBUG_TYPE_OTHER)
#	define SIM_GL_DEBUG_ENABLED 1
#endif

#ifndef NDEBUG
#	include <cstring> // basename (...) is defined in cstring

	namespace Sim {
		void _checkGLError (const char* file, int line);
	}

#	define LOG_GL_ERROR() _checkGLError(basename(__FILE__), __LINE__)

# ifdef SIM_GL_DEBUG_ENABLED
	namespace Sim {
		void APIENTRY GLDebugMessage (GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
					const GLchar* message, const void* parameters);
	}
#	endif

#else
#	define LOG_GL_ERROR ()
#endif
