/**
 * @file AssetManager.cpp
 * @author Kishalay Kundu <kishalay.kundu@gmail.com>
 * @section LICENSE
 * See LICENSE.txt included in this package
 *
 * @section DESCRIPTION
 * See AssetManager.h.
 */

#include <map>
#include <memory>

#include "tinyxml2.h"

#include "Types.h"
#include "Log.h"

#include "ConfigParser.h"
#include "Assets/Asset.h"
#include "Assets/AssetManager.h"

using std::map;
using std::shared_ptr;
using std::make_shared;
using tinyxml2::XMLElement;

namespace Sim {

	AssetManager::~AssetManager () {Cleanup ();}

	bool AssetManager::Initialize (const char* config)
	{
		// read configuration file
		ConfigParser parser;
		if (!parser.Initialize (config, "AssetsConfig")){
			LOG_ERROR ("Could not initialize parser for " << config);
			return false;
		}

		XMLElement* element = parser.GetElement ("Assets");
		if (element == nullptr){
			LOG_ERROR ("No assets specified in " << config);
			return false;
		}

		if (!Load (*element)){
			LOG_ERROR ("Failed to initialize all assets from " << config);
			Cleanup ();
			return false;
		}
		LOG ("Asset manager initialized");
		return true;
	}

	void AssetManager::Cleanup ()
	{
		_assets.clear ();
	}

	bool AssetManager::Add (AssetId id, shared_ptr <Asset> asset)
	{
		if (_assets [id].get () != nullptr){
			LOG_ERROR (id << " already exists. Asset not added");
			return false;
		}
		_assets [id] = move (asset);
		return true;
	}

	shared_ptr <Asset> AssetManager::Get (AssetId id)
	{
#		ifndef NDEBUG

		auto f = _assets.find (id);
		if (f != _assets.end ()){
			return f->second;
		}
		LOG_ERROR ("Could not find asset with id " << id << ". Returning empty asset");

		shared_ptr <Asset> a;
		return a;

#		else
		return _assets [id];
#		endif
	}

	bool AssetManager::Load (XMLElement& element)
	{
		// put assets on to the map
		XMLElement* alist = element.FirstChildElement ("Asset");
		if (alist == nullptr){
			LOG_ERROR ("No \'Asset\' members specified in Asset config file");
			return false;
		}

		while (alist != nullptr){

			const char* name = nullptr;
			name = alist->Attribute ("Name");
			if (name == nullptr){
				LOG_ERROR ("No \'Name\' attribute found for " << alist->Value ());
				return false;
			}

			AssetId id = AssetIdByName (name);
			if (id == AssetId::Unknown){
				LOG_ERROR (id << " not a recognized id defined in Types.h");
				return false;
			}
			auto f = _assets.find (id);
			if (f != _assets.end ()){
				LOG_ERROR ("Duplicate asset id " << id << "found in config file (forbidden)");
				return false;
			}

			// add dummy asset ptr to map (this results in near-contiguous map)
			shared_ptr <Asset> p;
			_assets [id] = std::move(p);

			alist = alist->NextSiblingElement ("Asset");
		}

		// initialize the assets after they have been mapped (components remain uninitialized)
		alist = element.FirstChildElement ("Asset");

		while (alist != nullptr){

			const char* assettype = nullptr;
			assettype = alist->Attribute ("Type");
			if (assettype == nullptr){
				LOG_ERROR ("No type specified for Asset \'" << alist->Attribute ("Name"));
				return false;
			}
			AssetType type = AssetTypeByName (assettype);
			if (type == AssetType::Unknown){
				LOG_ERROR (alist->Attribute ("Name") << " type " << type << " not a recognized asset type defined in Types.h");
				return false;
			}

			AssetId id = AssetIdByName (alist->Attribute ("Name"));

			_assets [id] = make_shared <Asset> (type);
			_assets [id]->Initialize (*alist);

			alist = alist->NextSiblingElement ("Asset");
		}

		// initialize components now
		alist = element.FirstChildElement ("Asset");

		while (alist != nullptr){

			auto a = _assets.find (AssetIdByName (alist->Attribute ("Name")));
			if (!a->second->Load (*alist)){
				LOG_ERROR ("Could not load components for " << a->first);
				return false;
			}

			alist = alist->NextSiblingElement ("Asset");
		}

		return true;
	}

}
