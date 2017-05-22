/**
 * @file CuglMsd.h
 * @author Kishalay Kundu <kishalay.kundu@gmail.com>
 * @section LICENSE
 * See LICENSE.txt included in this package
 *
 * @section DESCRIPTION
 * The OpenGL-CUDA based Mass spring damper plugin for the
 * Canvas system.
 */
#pragma once

#include "tinyxml2.h"

#include "Preprocess.h"
#include "Types.h"

#include "Asset/Asset.h"
#include "Plugin/Plugin.h"

namespace Sim {

	class EXPORT CuglMsd : public Plugin {

	public:
		CuglMsd () = default;
		~CuglMsd () = default;

		CuglMsd (const CuglMsd&) = delete;
		CuglMsd& operator = (const CuglMsd&) = delete;

		const char* Name () const override final {return "CuglMsd";}

		bool AddAssetComponent (tinyxml2::XMLElement& config, AssetComponentType type, Asset* asset) override;
		void Cleanup () override {}

	protected:
		bool InitializeGeometry (tinyxml2::XMLElement& config, Asset* asset);
		bool InitializeRender (tinyxml2::XMLElement& config, Asset* asset);
		bool InitializePhysics (tinyxml2::XMLElement& config, Asset* asset);
	};
}
