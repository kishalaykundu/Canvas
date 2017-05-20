/**
 * @file LinuxDriver.cpp
 * @author Kishalay Kundu <kishalay.kundu@gmail.com>
 * @section LICENSE
 * See LICENSE.txt included in this package
 *
 * @section DESCRIPTION
 * See LinuxDriver.h.
 */

#include <memory>

#include "tinyxml2.h"

#include "Log.h"
#include "ConfigParser.h"
#include "Driver/Driver.h"
#include "LinuxApp/LinuxDriver.h"

using tinyxml2::XMLElement;

namespace Sim {

	Driver* Driver::_instance = new LinuxDriver ();

	LinuxDriver::~LinuxDriver () {Cleanup ();}

	bool LinuxDriver::Initialize (const char* config)
	{
		if (config == nullptr){
			LOG_ERROR ("No input configuration file specified");
			return false;
		}

		// read configuration file
		ConfigParser parser;
		if (!parser.Initialize (config, "AppConfig")){
			LOG_ERROR ("Could not initialize parser for " << config);
			return false;
		}

		// Initialize the event manager (always the first module to be initialized)
		XMLElement* element = parser.GetElement ("EventManager");
		if (element == nullptr){
			LOG_ERROR ("Event manager profile not found in " << config);
			return false;
		}
		if (!InitializeEventManager (element->Attribute ("Config"))){
			Cleanup ();
			return false;
		}
		element = nullptr;

		// Initialize plugin manager (loads all dynamic plugin libraries)
		element = parser.GetElement ("PluginManager");
		if (element == nullptr){
			LOG_ERROR ("Plugin manager profile not found in " << config);
			Cleanup ();
			return false;
		}
		if (!InitializePluginManager (element->Attribute ("Config"))){
			Cleanup ();
			return false;
		}
		element = nullptr;

		// Intialize asset factory and load all assets
		element = parser.GetElement ("AssetManager");
		if (element == nullptr){
			LOG_ERROR ("Asset manager profile not found in " << config);
			Cleanup ();
			return false;
		}
		if (!InitializeAssetManager (element->Attribute ("Config"))){
			Cleanup ();
			return false;
		}
		element = nullptr;

		_runFlag = true;
		LOG ("Linux driver initialized");
		return true;
	}

	void LinuxDriver::Run ()
	{

	}

	void LinuxDriver::Cleanup ()
	{
		_assetManager.reset ();
		_pluginManager.reset ();
		_eventManager.reset ();
	}
}
