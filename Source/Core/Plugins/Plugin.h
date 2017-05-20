/**
 * @file Plugin.h
 * @author Kishalay Kundu <kishalay.kundu@gmail.com>
 * @section LICENSE
 * See LICENSE.txt included in this package
 *
 * @section DESCRIPTION
 * The plugin interface for the driver in the Canvas framework. The
 * specific implementation is left to the individual implementation
 * libraries. Each plugin is responsible for implementing customized
 * task/algorithm.
 */
#pragma once

#include <memory>
#include "tinyxml2.h"

#include "Preprocess.h"
#include "Types.h"

namespace Sim {

	class Asset;

	class Plugin {

		public:
			Plugin () = default;
			virtual ~Plugin () = default;

			Plugin (const Plugin& p) = delete;
			Plugin& operator = (const Plugin& p) = delete;

			virtual const char* Name () const = 0;

			virtual bool AddAssetComponent (tinyxml2::XMLElement& config, AssetComponentType, Asset* asset) = 0;
			virtual void Cleanup () = 0;
	};

	typedef void (*NewPlugin)(Sim::PluginType);

	// interface for constructor from dynamically loaded libraries.
	extern "C" EXPORT void StartPlugin (Sim::PluginType);
}
