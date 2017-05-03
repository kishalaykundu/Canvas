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
#include "Manager.h"
#include "Events/Event.h"

namespace Sim {

	class CircularQueue;

	typedef util::Callback <void (unsigned int)> EventListener;

	class EventManager : public Manager {

		private:
			unsigned int _index = 0;
			std::unique_ptr <CircularQueue> _queue;
			std::map <unsigned int, EventListener> _listeners;

		public:
			EventManager ();
			~EventManager ();

			EventManager (const EventManager&) = delete;
			EventManager& operator = (const EventManager&) = delete;

			virtual bool Initialize (const char* config);
			virtual void Run ();
			virtual void Cleanup ();

			bool AddListener (const EventListener&, EventType, unsigned int);
			bool RemoveListener (const EventListener&, EventType, unsigned int);

			bool QueueEvent (Event&);
	};
}
