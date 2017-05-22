/**
 * @file CudaManager.h
 * @author Kishalay Kundu <kishalay.kundu@gmail.com>
 * @section LICENSE
 * See LICENSE.txt included in this package
 *
 * @section DESCRIPTION
 * CUDA based High Performance Computing Manager.
 */
#pragma once

#include <memory>

#include "Config.h"

#ifdef SIM_GL_ENABLED
#	include "GL/GLUtils.h"
#endif

#include "CUDA/CUDAUtils.h"
#include "Compute/ComputeManager.h"

namespace Sim {

	class CudaManager : public ComputeManager {

	protected:
#		ifdef SIM_GL_ENABLED
		::Display* _display = nullptr;
		::GLXContext _gpuContext = 0;
#		endif

		CUcontext _hpcContext = 0;

	public:
		CudaManager () = default;
		~CudaManager () {Cleanup ();}

		CudaManager (const CudaManager&) = delete;
		CudaManager& operator = (const CudaManager&) = delete;

		bool Initialize (const char* config) override;
		void Update () override;
		void Cleanup () override;

	};
}
