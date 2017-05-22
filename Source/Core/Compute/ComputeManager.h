/**
 * @file ComputeManager.h
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

	class ComputeManager {

	public:
		ComputeManager () = default;
		virtual ~ComputeManager () {Cleanup ();}

		ComputeManager (const ComputeManager&) = delete;
		ComputeManager& operator = (const ComputeManager&) = delete;

		virtual bool Initialize (const char* config) {return true;}
		virtual void Update () {}
		virtual void Cleanup () {}
	};
}
