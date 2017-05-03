/**
 * @file Manager.h
 * @author Kishalay Kundu <kishalay.kundu@gmail.com>
 * @section LICENSE
 * See LICENSE.txt included in this package
 *
 * @section DESCRIPTION
 * The interface to the manager class.
 */
#pragma once

namespace Sim {

	class Manager {

	public:
		Manager () = default;
		Manager (const Manager&) = delete;
		Manager& operator = (const Manager&) = delete;

		virtual ~Manager () = default;

		virtual bool Initialize (const char* configfile) = 0;
		virtual void Run () = 0;
		virtual void Cleanup () = 0;
	};
}
