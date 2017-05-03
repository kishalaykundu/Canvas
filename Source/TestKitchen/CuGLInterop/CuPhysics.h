/**
 * @file CuPhysics.h
 * @author Kishalay Kundu <kishalay.kundu@gmail.com>
 * @section LICENSE
 * See LICENSE.txt included in this package
 *
 * @section DESCRIPTION
 * CUDA based physics computing thread.
 */
#pragma once

#include <memory>

#include "GL/GLUtils.h"

namespace Sim {

	class GLDisplay;

	class CuPhysics {

	protected:
		bool _runFlag;
		::Display* _display;
		::GLXContext _context;

	public:
		bool Initialize (std::shared_ptr <GLDisplay>&);
		void Run ();
		void Cleanup ();
	};
}
