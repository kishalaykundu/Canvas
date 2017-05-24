/**
 * @file Component.h
 * @author Kishalay Kundu <kishalay.kundu@gmail.com>
 * @section LICENSE
 * See LICENSE.txt included in this package
 *
 * @section DESCRIPTION
 * The generic component interface for the Asset class in the Canvas
 * framework. Different kinds of components derive from this class.
 * Each component is responsible for loading and managing its own data.
 * Each asset can own different types of components in a mix-and-match
 * sense.
 */
#pragma once

#include <string>

#include "tinyxml2.h"
#include "Types.h"

namespace Sim {

	class Asset;

	namespace Assets {

		class Component {

				friend class Sim::Asset;

			protected:
				Asset* _owner = nullptr;

			public:
				Component () = default;
				virtual ~Component () = default;

				Component (const Component& c) = default;
				Component& operator = (const Component& c) = default;

				virtual AssetComponentType Type () const = 0;

				virtual bool Initialize (tinyxml2::XMLElement& config, Asset* asset) = 0;
				virtual void Update () = 0;
				virtual void Cleanup () = 0;
		};
	}
}
