/**
 * @file AssetManager.h
 * @author Kishalay Kundu <kishalay.kundu@gmail.com>
 * @section LICENSE
 * See LICENSE.txt included in this package
 *
 * @section DESCRIPTION
 * The factory class for assets in the Canvas framework.
 */
#pragma once

#include <memory>
#include <map>

#include "Types.h"

namespace Sim {

	class Asset;

	class AssetManager {

	protected:
		std::map <AssetId, std::shared_ptr <Asset> > _assets;

	public:
		AssetManager () = default;
		~AssetManager ();

		AssetManager (const AssetManager& am) = delete;
		AssetManager& operator = (const AssetManager& am) = delete;

		bool Initialize (const char* config);
		void Cleanup ();

		bool Add (AssetId id, std::shared_ptr <Asset> asset);
		std::shared_ptr <Asset> Get (AssetId id);

	protected:
		bool Load (tinyxml2::XMLElement&);
	};
}
