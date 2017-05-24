/**
 * @file MsdPhysics.cpp
 * @author Kishalay Kundu <kishalay.kundu@gmail.com>
 * @section LICENSE
 * See LICENSE.txt included in this package
 *
 * @section DESCRIPTION
 * See MsdPhysics.h.
 */

#include <memory>

#include "tinyxml2.h"

#include "CUDA/CUDAUtils.h"

#include "Asset/Asset.h"
#include "MsdRender.h"
#include "MsdPhysics.h"

using tinyxml2::XMLElement;

namespace Sim {
	namespace Assets {

		bool MsdPhysics::Initialize (XMLElement& config, Asset* asset)
		{
			MsdRender* c = (asset->Get <Assets::MsdRender> (AssetComponentType::Render)).get ();

			LOG_CUDA_RESULT (cuGraphicsGLRegisterBuffer (_vertices, c->_positionBuffer, CU_GRAPHICS_REGISTER_FLAGS_NONE));

			// load spring indices from file


			return true;
		}

		void MsdPhysics::Cleanup ()
		{

		}
	}
}
