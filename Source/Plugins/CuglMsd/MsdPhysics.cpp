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

#include "Log.h"
#include "CUDA/CUDAUtils.h"

#include "MeshUtils.h"
#include "Asset/Asset.h"
#include "MsdRender.h"
#include "MsdPhysics.h"

using tinyxml2::XMLElement;

namespace Sim {
	namespace Assets {

		bool MsdPhysics::Initialize (XMLElement& element, Asset* asset)
		{
			MsdRender* mr = (asset->Get <Assets::MsdRender> (AssetComponentType::Render)).get ();

			// register the vertex position buffer from GL (will be used to map and use later)
			LOG_CUDA_RESULT (cuGraphicsGLRegisterBuffer (_vertices, mr->_positionBuffer, CU_GRAPHICS_REGISTER_FLAGS_NONE));

			// load spring indices from file
			XMLElement* elem = element.FirstChildElement ("Edges");
			if (elem == nullptr){
				LOG_ERROR ("Spring index profile not specified in config");
				return false;
			}
			const char* file = elem->Attribute ("Location");
			if (file == nullptr){
				LOG_ERROR ("No spring index file in config");
				return false;
			}

			_numSprings = MeshUtils::MeshFileElementCount (file);
			if (!_numSprings){
				LOG_ERROR ("Could not read element count in " << file);
				return false;
			}

			unsigned int* springs = new unsigned int [2*_numSprings];
			if (!MeshUtils::IndexLoad <2>(file, springs)){
				LOG_ERROR ("Could not successfully read elements from " << file);
				delete [] springs;
				return false;
			}
			LOG_CUDA_RESULT (cuMemAlloc (&_indices, 2*sizeof (unsigned int)*_numSprings));
			LOG_CUDA_RESULT (cuMemcpyHtoD (_indices, springs, 2*sizeof (unsigned int)*_numSprings));

			delete [] springs;

			return true;
		}

		void MsdPhysics::Cleanup ()
		{

		}
	}
}
