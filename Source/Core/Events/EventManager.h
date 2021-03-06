/**
 * @file EventManager.h
 * @author Kishalay Kundu <kishalay.kundu@gmail.com>
 * @section LICENSE
 * See LICENSE.txt included in this package
 *
 * @section DESCRIPTION
 * The EventManager system for the Chimera framework.
 */
#pragma once

#include <map>
#include <memory>

#include "Callback.h"
#include "CircularQueue.h"
#include "Events/Event.h"

namespace Sim {

	typedef util::Callback <void (unsigned int)> EventListener;

	class EventManager {

		private:
			unsigned int _index = 0;
			CircularQueue <Event, 64> _queue;
			std::map <unsigned int, EventListener> _listeners;

		public:
			EventManager () = default;
			~EventManager ();

			EventManager (const EventManager&) = delete;
			EventManager& operator = (const EventManager&) = delete;

			bool Initialize (const char* config);
			void Run ();
			void Cleanup ();

			bool AddListener (const EventListener&, EventType, unsigned int);
			bool RemoveListener (const EventListener&, EventType, unsigned int);

			bool QueueEvent (Event&);
	};
}
