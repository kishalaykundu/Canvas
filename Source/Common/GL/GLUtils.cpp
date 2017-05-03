/**
 * @file GLUtils.cpp
 * @author Kishalay Kundu <kishalay.kundu@gmail.com>
 * @section LICENSE
 * See LICENSE.txt included in this package
 *
 * @section DESCRIPTION
 * See GLUtils.h.
 */

#	include <iostream>
#include "GL/GLUtils.h"

namespace Sim {

	void _checkGLError (const char* file, int line)
	{
		GLenum error (glGetError ());
		while (error != GL_NO_ERROR){

			std::cerr << file << " [" << line << "]: GL Error: GL_";
			switch (error){
			case GL_INVALID_ENUM:
				std::cerr << "INVALID_ENUM" << std::endl;
				break;
			case GL_INVALID_VALUE:
				std::cerr << "INVALID_VALUE" << std::endl;
				break;
			case GL_INVALID_OPERATION:
				std::cerr << "INVALID_OPERATION" << std::endl;
				break;
			case GL_STACK_OVERFLOW:
				std::cerr << "STACK_OVERFLOW" << std::endl;
				break;
			case GL_STACK_UNDERFLOW:
				std::cerr << "STACK_UNDERFLOW" << std::endl;
				break;
			case GL_OUT_OF_MEMORY:
				std::cerr << "OUT_OF_MEMORY" << std::endl;
				break;
			case GL_INVALID_FRAMEBUFFER_OPERATION:
				std::cerr << "INVALID_FRAMEBUFFER_OPERATION" << std::endl;
				break;
			default:
				std::cerr << "UNKNOWN_ERROR (" << error << ")" << std::endl;
			}

			error = glGetError ();
		}
	}

#	ifdef SIM_GL_DEBUG_ENABLED
	void APIENTRY GLDebugMessage (GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
			const GLchar* message, const void* parameters)
	{
		std::cerr << basename( __FILE__ ) << "[" << __LINE__ << "]: GL error:" << std::endl;
		std::cerr << "ID - [" << id << "] Type - GL_";
		switch (type) {
			case GL_DEBUG_TYPE_ERROR:
				std::cerr << "ERROR";
				break;
			case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
				std::cerr << "DEPRECATED_BEHAVIOR";
				break;
			case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
				std::cerr << "UNDEFINED_BEHAVIOR";
				break;
			case GL_DEBUG_TYPE_PORTABILITY:
				std::cerr << "PORTABILITY";
				break;
			case GL_DEBUG_TYPE_PERFORMANCE:
				std::cerr << "PERFORMANCE";
				break;
			case GL_DEBUG_TYPE_OTHER:
				std::cerr << "OTHER";
				break;
		}
		std::cerr << " Severity - ";
		switch (severity) {
			case GL_DEBUG_SEVERITY_LOW:
				std::cerr << "LOW";
				break;
			case GL_DEBUG_SEVERITY_MEDIUM:
				std::cerr << "MEDIUM";
				break;
			case GL_DEBUG_SEVERITY_HIGH:
				std::cerr << "HIGH";
				break;
		}
		std::cerr << std::endl;
		std::cerr << message << std::endl;
	}
#	endif

}
