/**
 * @file CuglMsd.cpp
 * @author Kishalay Kundu <kishalay.kundu@gmail.com>
 * @section LICENSE
 * See LICENSE.txt included in this package
 *
 * @section DESCRIPTION
 * See CuglMsd.h.
 */
#include <memory>
#include "tinyxml2.h"

#include "Preprocess.h"
#include "Log.h"
#include "Types.h"
#include "Driver/Driver.h"

using std::unique_ptr;
using std::make_unique;
using tinyxml2::XMLElement;

namespace Sim {

	extern "C" EXPORT void StartPlugin (PluginType id)
	{
		unique_ptr <Plugin> plugin = make_unique <CuglMsd> ();
		if (!Driver::Instance ().AddPlugin (id, plugin)){
			LOG_ERROR ("Could not add CuglMsd to the App");
		}
	}

	bool CuglMsd::AddAssetComponent (XMLElement& config, AssetComponentType type, Asset* asset)
	{
		switch (type){
		case AssetComponentType::Geometry:
			return InitializeGeometry (config, asset);

		case AssetComponentType::Render:
			return InitializeRender (config, asset);

		case AssetComponentType::Physics:
			return InitializePhysics (config, asset);

		default:
			LOG_ERROR ("Invalid component type specified: " << type);
		}
		return false;
	}

	bool CuglMsd::InitializeGeometry (tinyxml2::XMLElement& config, Asset* asset)
	{
		return true;
	}

	bool CuglMsd::InitializeRender (tinyxml2::XMLElement& config, Asset* asset)
	{
		return true;
	}

	bool CuglMsd::InitializePhysics (tinyxml2::XMLElement& config, Asset* asset)
	{
		return true;
	}
}
