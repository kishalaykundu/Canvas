/**
 * @file CUDAUtils.cpp
 * @author Kishalay Kundu <kishalay.kundu@gmail.com>
 * @section LICENSE
 * See LICENSE.txt included in this package
 *
 * @section DESCRIPTION
 * See CUDAUtils.h.
 */

#include <iostream>
#include "CUDA/CUDAUtils.h"

namespace Sim {

	void _checkCUDAError (cudaError_t error, const char* file, int line)
	{
		if (error != cudaSuccess){
			std::cerr << file << " [" << line << "]: CUDA Error: " << cudaGetErrorString (error) << std::endl;
		}
	}

	void _checkCUDAResult (CUresult result, const char* file, int line)
	{
		if (result != CUDA_SUCCESS){
			const char* eString;
			cuGetErrorName (result, &eString);
			std::cerr << file << " [" << line << "]: CUDA Error: " << eString;
			cuGetErrorString (result, &eString);
			std::cerr << " (" << eString << ")" << std::endl;
		}
	}
}
