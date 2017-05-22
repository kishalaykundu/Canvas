/**
 * @file CudaManager.cpp
 * @author Kishalay Kundu <kishalay.kundu@gmail.com>
 * @section LICENSE
 * See LICENSE.txt included in this package
 *
 * @section DESCRIPTION
 * See CudaManager.h.
 */
#include <memory>

#include "Log.h"
#include "Driver/Driver.h"

#ifdef SIM_GL_ENABLED
#	include "GL/GLUtils.h"
#	include "Render/GL45/GLManager.h"
#endif

#include "Compute/Cuda/CudaManager.h"

namespace Sim {

	bool CudaManager::Initialize (const char* configfile)
	{
#		ifdef SIM_GL_ENABLED

		GLManager* gm = static_cast <GLManager*> (&(Driver::Instance ().GetRenderManager ()));
		if (!gm->MakeNewContext (_display, _gpuContext)){
			LOG_ERROR ("Could not create GLX context for CUDA HPC Manager");
		  return false;
		}
		glXMakeContextCurrent (_display, 0, 0, _gpuContext); LOG_GL_ERROR ();
		XSync (_display, False);

#		endif

		LOG_CUDA_RESULT (cuInit (0));

		// set device and create context (only 1 device associated with current OpenGL context is queried)
		unsigned int numDevices = 0;
		CUdevice device [2];
		LOG_CUDA_RESULT (cuGLGetDevices (&numDevices, device, 1, CU_GL_DEVICE_LIST_ALL));
		LOG_CUDA_ERROR (cudaSetDevice (device [0]));
		LOG_CUDA_RESULT (cuGLCtxCreate (&_hpcContext, CU_CTX_SCHED_AUTO | CU_CTX_MAP_HOST, *device));
		LOG_CUDA_RESULT (cuCtxSetCurrent (_hpcContext));

		LOG ("CUDA high performance computing manager initialized");
		return true;
	}

	void CudaManager::Update ()
	{

	}

	void CudaManager::Cleanup ()
	{

	}

}
