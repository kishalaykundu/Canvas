/**
 * @file Driver.cpp
 * @author Kishalay Kundu <kishalay.kundu@gmail.com>
 * @section LICENSE
 * See LICENSE.txt included in this package
 *
 * @section DESCRIPTION
 * See Driver.h.
 */

#include <memory>

#include "Log.h"
#include "Plugin/SharedLib.h"
#include "Driver/Driver.h"

using std::make_unique;

namespace Sim {

	bool Driver::InitializeEventManager (const char* config)
	{
		_eventManager = make_unique <EventManager> ();
		if (!_eventManager->Initialize (config)){
			LOG_ERROR ("Event manager could not be initialize with " << config);
			return false;
		}
		return true;
	}

	bool Driver::InitializePluginManager (const char* config)
	{
		_pluginManager = make_unique <PluginManager> ();
		if (!_pluginManager->Initialize (config)){
			LOG_ERROR ("Plugin manager could not be initialize with " << config);
			return false;
		}
		return true;
	}

	bool Driver::InitializeAssetManager (const char* config)
	{
		_assetManager = make_unique <AssetManager> ();
		if (!_assetManager->Initialize (config)){
			LOG_ERROR ("Asset manager could not be initialize with " << config);
			return false;
		}
		return true;
	}
}
