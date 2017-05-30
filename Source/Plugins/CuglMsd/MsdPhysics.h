/**
 * @file MsdPhysics.h
 * @author Kishalay Kundu <kishalay.kundu@gmail.com>
 * @section LICENSE
 * See LICENSE.txt included in this package
 *
 * @section DESCRIPTION
 * The CUDA based physics component for the CuglMsd plugin. It is
 * extended from Asset Component type.
 */
#pragma once

#include "tinyxml2.h"

#include "CUDA/CUDAUtils.h"
#include "Asset/Asset.h"
#include "Asset/Component.h"

namespace Sim {
	namespace Assets {

		class MsdPhysics : public Component {

		protected:
			CUgraphicsResource* _vertices = nullptr;

			unsigned int _numSprings = 0;
			CUdeviceptr _indices = 0;

		public:
			MsdPhysics () = default;
			~MsdPhysics () {Cleanup ();}

			MsdPhysics (const MsdPhysics&) = delete;
			MsdPhysics& operator = (const MsdPhysics&) = delete;

			AssetComponentType Type () const final {return AssetComponentType::Physics;}

			bool Initialize (tinyxml2::XMLElement& config, Asset* asset) override;
			void Update () override {}
			void Cleanup () override;
		};
	}
}
