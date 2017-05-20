/**
 * @file Asset.h
 * @author Kishalay Kundu <kishalay.kundu@gmail.com>
 * @section LICENSE
 * See LICENSE.txt included in this package
 *
 * @section DESCRIPTION
 * The asset class for the Canvas system, representing all simulated
 * bodies/entities. Assets are composed of different components based
 * on compositions of across different objects.
 */
#pragma once

#include <map>
#include <string>
#include <memory>

#include "tinyxml2.h"

#include "Preprocess.h"
#include "Log.h"
#include "Types.h"
#include "Assets/Component.h"

namespace Sim {

	class AssetManager;

	class EXPORT Asset {

		friend class AssetManager;

	protected:
		AssetType _type = AssetType::Unknown;
		std::map <AssetComponentType, std::shared_ptr <Assets::Component> > _components;

	public:
		Asset (AssetType t) : _type (t) {};
		~Asset ();

		Asset () = delete;
		Asset (const Asset&) = delete;
		Asset& operator = (const Asset&) = delete;

		AssetType Type () const {return _type;}

		bool Initialize (tinyxml2::XMLElement& element);
		void Cleanup ();

		void Add (AssetComponentType id, std::shared_ptr <Assets::Component> component)
		{
			component->_owner = const_cast <Asset*> (this);
			_components [id] = std::move (component);
		}

		template <class ComponentType> std::shared_ptr <ComponentType> Get (AssetComponentType id)
		{
			auto it = _components.find (id);

#			ifndef NDEBUG

			if (it != _components.end ()){
				std::shared_ptr <Assets::Component> component (it->second);
				return std::shared_ptr <ComponentType> (std::static_pointer_cast <ComponentType> (component));
			}
			LOG_ERROR ("Component" << id << "not found...returning empty component");
			return std::shared_ptr <ComponentType> ();

#			else

			std::shared_ptr <Assets::Component> component (it->second);
			return std::shared_ptr <ComponentType> (std::static_pointer_cast <ComponentType> (component));
#			endif
		}

	protected:
		bool Load (tinyxml2::XMLElement& element);
	};
}
