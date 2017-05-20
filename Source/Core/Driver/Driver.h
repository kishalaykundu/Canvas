/**
 * @file Driver.h
 * @author Kishalay Kundu <kishalay.kundu@gmail.com>
 * @section LICENSE
 * See LICENSE.txt included in this package
 *
 * @section DESCRIPTION
 * The abstract factory responsible for adding and maintaining
 * the different manager (factory) modules. This is designed so
 * that the concrete apps (clients) can, depending on the input
 * config file spawn and run different modules.
 */
#pragma once

#include <string>
#include <map>
#include <memory>

#include "Log.h"
#include "Types.h"

#include "Events/EventManager.h"

#include "Plugins/PluginManager.h"
#include "Plugins/Plugin.h"

#include "Assets/AssetManager.h"
#include "Assets/Asset.h"


namespace Sim {

	class Driver {

	protected:
		bool _runFlag = false;
		static Driver* _instance;

		std::unique_ptr <EventManager> _eventManager;
		std::unique_ptr <PluginManager> _pluginManager;
		std::unique_ptr <AssetManager> _assetManager;

		Driver () = default;
		Driver (const Driver&) = delete;
		Driver& operator = (const Driver&) = delete;

	public:
		static Driver& Instance () {return *_instance;}

		virtual ~Driver () = default;

		virtual bool Initialize (const char* configfile) = 0;
		virtual void Run () = 0;
		virtual void Cleanup () = 0;

		void Quit () {_runFlag = false;}

		bool AddPlugin (PluginType id, std::unique_ptr <Plugin>& plugin)
		{
			std::unique_ptr <Plugin> p (std::move (plugin));
			return _pluginManager->Add (id, p);
		}

		bool AddAsset (AssetId id, std::shared_ptr <Asset> asset)
		{
			return _assetManager->Add (id, asset);
		}

		Plugin* GetPlugin (PluginType id)
		{
			return _pluginManager->Get (id);
		}

		std::shared_ptr <Asset> GetAsset (AssetId id)
		{
			return _assetManager->Get (id);
		}

	protected:
		virtual bool InitializeEventManager (const char* config);
		virtual bool InitializePluginManager (const char* config);
		virtual bool InitializeAssetManager (const char* config);
	};
}
