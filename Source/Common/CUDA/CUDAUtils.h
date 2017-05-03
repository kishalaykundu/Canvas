/**
 * @file CUDAUtils.h
 * @author Kishalay Kundu <kishalay.kundu@gmail.com>
 * @section LICENSE
 * See LICENSE.txt included in this package
 *
 * @section DESCRIPTION
 * Platform-dependent CUDA header files. Must always be included
 * after the Graphics related headers.
 */
#pragma once

#include "Config.h"

#ifdef SIM_GL_ENABLED
#	include "GL/GLUtils.h"
#endif

// macro-definition to enable separate default streams per thread
#define CUDA_API_PER_THREAD_DEFAULT_STREAM

extern "C"{
#	include <cuda.h>
#	include <cuda_runtime_api.h>

#	ifdef SIM_GL_ENABLED
#		include <cudaGL.h>
#	endif
}

#ifndef NDEBUG
#include <cstring>

namespace Sim {
	void _checkCUDAError (cudaError_t error, const char* file, int line);
	void _checkCUDAResult (CUresult, const char* file, int line);
}

#	define LOG_CUDA_ERROR(x) _checkCUDAError ((x), basename (__FILE__), __LINE__)
#	define LOG_CUDA_RESULT(x) _checkCUDAResult ((x), basename (__FILE__), __LINE__)
#else
#	define LOG_CUDA_ERROR(x) (x)
#	define LOG_CUDA_RESULT(x) (x)
#endif
