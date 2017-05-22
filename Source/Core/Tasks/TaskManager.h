/**
 * @file TaskManager.h
 * @author Kishalay Kundu <kishalay.kundu@gmail.com>
 * @section LICENSE
 * See LICENSE.txt included in this package
 *
 * @section DESCRIPTION
 * The generic high performance computing Compute Manager interface.
 */
#pragma once

#include <memory>

namespace Sim {

	class TaskManager {

	public:
		TaskManager () = default;
		virtual ~TaskManager () = default;

		TaskManager (const TaskManager&) = delete;
		TaskManager& operator = (const TaskManager&) = delete;

		virtual bool Initialize (const char* config) {return true;}
		virtual void Update () {}
		virtual void Cleanup () {}
	};
}
