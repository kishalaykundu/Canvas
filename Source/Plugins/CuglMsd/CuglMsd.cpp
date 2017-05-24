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

#include "Asset/Geometry.h"
#include "Driver/Driver.h"

#include "MsdRender.h"
#include "MsdPhysics.h"
#include "CuglMsd.h"

using std::shared_ptr;
using std::make_shared;
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
		shared_ptr <Assets::Component> gc = make_shared <Assets::Geometry> ();
		if (!gc->Initialize (config, asset)){
			LOG_ERROR ("Could not initialize geometry component");
			gc.reset ();
			return false;
		}
		asset->Add (AssetComponentType::Geometry, gc);

		return true;
	}

	bool CuglMsd::InitializeRender (tinyxml2::XMLElement& config, Asset* asset)
	{
		shared_ptr <Assets::Component> rc = make_shared <Assets::MsdRender> ();
		if (!rc->Initialize (config, asset)){
			LOG_ERROR ("Could not initialize render component");
			rc.reset ();
			return false;
		}
		asset->Add (AssetComponentType::Render, rc);
		return true;
	}

	bool CuglMsd::InitializePhysics (tinyxml2::XMLElement& config, Asset* asset)
	{
		shared_ptr <Assets::Component> pc = make_shared <Assets::MsdPhysics> ();
		if (!pc->Initialize (config, asset)){
			LOG_ERROR ("Could not initialize render component");
			pc.reset ();
			return false;
		}
		asset->Add (AssetComponentType::Physics, pc);
		return true;
	}
}
