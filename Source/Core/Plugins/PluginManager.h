/**
 * @file PluginManager.h
 * @author Kishalay Kundu <kishalay.kundu@gmail.com>
 * @section LICENSE
 * See LICENSE.txt included in this package
 *
 * @section DESCRIPTION
 * The plugin manager class that encapsulates all the plugins and the
 * shared libraries they are loaded from, in the Canvas Framework.
 */
#pragma once

#include <memory>
#include <map>

#include "tinyxml2.h"
#include "Types.h"

namespace Sim {

	class SharedLib;
	class Plugin;

	class PluginManager {

		protected:
			std::map <PluginType, std::unique_ptr <SharedLib> > _libs;
			std::map <PluginType, std::unique_ptr <Plugin> > _plugins;

		public:
			PluginManager () = default;
			~PluginManager ();

			// forbidden copy constructor and assignment operator
			PluginManager (const PluginManager&) = delete;
			PluginManager& operator = (const PluginManager&) = delete;

			bool Initialize (const char* config);
			void Cleanup ();

			bool Add (PluginType id, std::unique_ptr <Plugin>& p);
			Plugin* Get (PluginType id);

		protected:
			bool AddLib (tinyxml2::XMLElement& element);
			bool LoadPlugin (PluginType id);
	};
}
