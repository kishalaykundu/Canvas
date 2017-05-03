/**
 * @file Driver.cpp
 * @author Kishalay Kundu <kishalay.kundu@gmail.com>
 * @section LICENSE
 * See LICENSE.txt included in this package
 *
 * @section DESCRIPTION
 * See Driver.h.
 */

#include <string>
#include <map>
#include <memory>

#include "tinyxml2.h"

#include "Log.h"
#include "ConfigParser.h"
#include "Driver.h"
#include "Manager.h"

using std::string;
using std::unique_ptr;
using tinyxml2::XMLElement;

namespace Sim {

	Driver* Driver::_instance = new Driver ();

	Manager* Driver::Get (const string& name)
	{
#		ifndef NDEBUG
		auto m = _managers.find (name);
		if (m != _managers.end ()){
			return m->second.get ();
		}
		else {
			LOG_ERROR ("Could not find " << name << ".");
			return nullptr;
		}
#		endif

		return _managers [name].get ();
	}

	bool Driver::Add (const string& name, unique_ptr <Manager> mgr)
	{
		auto m = _managers.find (name);
		if (m != _managers.end ()){
			LOG_ERROR ("A manager by the name of " << name << "already exists.");
			return false;
		}
		_managers [name] = std::move (mgr);
		return true;
	}

	bool Driver::Initialize (const char* config)
	{
		ConfigParser parser;
		if (!parser.Initialize (config, "AppConfig")){
			LOG_ERROR ("Could not initialize xml parser for " << config);
			return false;
		}

		// initialize all the managers
		for (auto &mgr : _managers){
			XMLElement* element = parser.GetElement (mgr.first.c_str ());
			if (element == nullptr){
				LOG_ERROR ("Could not get " << mgr.first << " in " << config);
				Cleanup ();
				return false;
			}
			const char* cfg = element->Attribute ("Config");
			if (cfg == nullptr){
				LOG_ERROR (mgr.first << " in " << config << " does not name a config file");
				Cleanup ();
				return false;
			}
			if (!mgr.second->Initialize (cfg)){
				LOG_ERROR (mgr.first << " could not be initialized from input config file " << cfg);
				Cleanup ();
				return false;
			}
		}

		// Application is ready to run now
		_runFlag = true;
		return true;
	}

	void Driver::Cleanup ()
	{
		for (auto &mgr : _managers){
			mgr.second->Cleanup ();
		}
		_managers.clear ();
	}

	void Driver::Quit ()
	{
		_runFlag = false;
	}
}
