/**
 * @file PluginManager.cpp
 * @author Kishalay Kundu <kishalay.kundu@gmail.com>
 * @section LICENSE
 * See LICENSE.txt included in this package
 *
 * @section DESCRIPTION
 * See PluginManager.h.
 */

#include <string>
#include <sstream>
#include <memory>
#include <map>

#include "tinyxml2.h"

#include "Types.h"
#include "Log.h"
#include "ConfigParser.h"

#include "Plugins/Plugin.h"
#include "Plugins/SharedLib.h"
#include "Plugins/PluginManager.h"

using std::ostringstream;
using std::move;
using std::unique_ptr;
using std::make_unique;

using tinyxml2::XMLElement;
using tinyxml2::XMLError;
using tinyxml2::XML_SUCCESS;

namespace Sim {

	PluginManager::~PluginManager () {Cleanup ();}

	bool PluginManager::Initialize (const char* config)
	{
		ConfigParser parser;
		if (!parser.Initialize (config, "PluginsConfig")){
			LOG_ERROR ("Could not initialize parser for " << config);
			return false;
		}

		XMLElement* element = parser.GetElement ("Plugins");
		if (element == nullptr){
			LOG_ERROR ("No plugin libraries specified in " << config);
			return false;
		}
		int count = 0;
		XMLError error = element->QueryIntAttribute ("Count", &count);
		if (error != XML_SUCCESS || count <= 0){
			LOG_ERROR ("Some error occured while reading \'Count\' attribute of Plugins in " << config);
			return false;
		}

		XMLElement* liblist = element->FirstChildElement ("Plugin");
		if (liblist == nullptr){
			LOG_ERROR (config << "has no \'Plugin\' members");
			return false;
		}

		// Add 'count' number of shared library to _libs
		int counter = 0;
		while (liblist != nullptr && counter < count){

			if (!AddLib (*liblist)){
				LOG_ERROR ("Could not add plugin library from " << config);
				Cleanup ();
				return false;
			}

			liblist = liblist->NextSiblingElement ("Plugin");
			++counter;
		}

		// initialize the _plugins map
		for (auto &lib : _libs){
			unique_ptr <Plugin> p;
			_plugins [lib.first] = std::move(p);
		}

		// start the corresponding plugins
		for (auto &lib : _libs){
			if (!LoadPlugin (lib.first)){
				LOG_ERROR ("Could not load plugin from " << lib.first);
				Cleanup ();
				return false;
			}
		}

		LOG ("Plugin manager initialized");
		return true;
	}

	void PluginManager::Cleanup ()
	{
		_plugins.clear ();

		for (auto &lib : _libs){
			ostringstream str;
			str << lib.first;

			if (!lib.second->Unload (str.str ())){
				LOG_WARNING (lib.first << "could not be unloaded");
			}
			_libs.erase (lib.first);
		}
		_libs.clear ();
	}

	// function to add plugin (used by external libraries to register)
 	bool PluginManager::Add (PluginType id, unique_ptr <Plugin>& plugin)
	{
		if (_plugins [id].get () != nullptr){
			LOG_ERROR (id << " already exists. Plugin not added");
			return false;
		}
		_plugins [id] = move (plugin);
		return true;
	}

	// returns pointer to plugin
	Plugin* PluginManager::Get (PluginType id)
	{
#		ifndef NDEBUG
		auto p = _plugins.find (id);
		if (p != _plugins.end ()){
			return p.second->get ();
		}

		LOG_WARNING ("Could not find " << id << " plugin");
		return nullptr;
#		else
		return _plugins [id].get ();
#		endif
	}

	// function to add library
	bool PluginManager::AddLib (XMLElement& element)
	{
		const char* name = nullptr;
		name = element.Attribute ("Name");
		if (name == nullptr){
			LOG_ERROR ("Plugin element in config file has no attribute called \'Name\'");
			return false;
		}
		PluginType id = PluginTypeByName (name);
		if (id == PluginType::Unknown){
			LOG_ERROR ("Plugin name " << name << " not recognizable. It should exist in Types.h file to be recognized.");
			return false;
		}
		auto lib = _libs.find (id);
		if (lib != _libs.end ()){
			LOG_WARNING (name << " already exists. Library not added");
			return false;
		}

		// set appropriate name with path
		string libNameWithPath;
		const char* libfolder = nullptr;
		libfolder = element.Attribute ("Location");
		if (libfolder != nullptr){
			libNameWithPath = libfolder;
			if (libNameWithPath [libNameWithPath.size () - 1] != '/'){
				libNameWithPath += "/";
			}
		}

		libNameWithPath += name;
#		ifndef NDEBUG
		libNameWithPath += "-debug";
#		endif

		const char* suffix = nullptr;
#		if defined (__linux__)
		libNameWithPath += "lib";
		suffix = ".so";

#		elif defined (__APPLE__)
		libNameWithPath += "lib";
		suffix = ".dylib";

#		elif defined (_WIN32)
		suffix = ".dll";
#		endif
		libNameWithPath += suffix;

#		ifdef __GNUC__
		__extension__
#		endif

		_libs [id] = make_unique <SharedLib> ();
		if (!_libs [id]->Load (libNameWithPath)){
			LOG_ERROR ("Could not load " << name << " library");
			return false;
		}

		return true;
	}

	// function to load lib (the id here definitely exists in _libs)
	bool PluginManager::LoadPlugin (PluginType id)
	{
#		ifdef __GNUC__
		__extension__
#		endif
		NewPlugin start = (NewPlugin)(_libs [id]->GetSymbol ("StartPlugin"));
		if (!start){
			LOG_ERROR ("Function \'StartPlugin\' not found in " << id);
			return false;
		}
		start (id);

		return true;
	}
}
