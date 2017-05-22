/**
 * @file RenderManager.h
 * @author Kishalay Kundu <kishalay.kundu@gmail.com>
 * @section LICENSE
 * See LICENSE.txt included in this package
 *
 * @section DESCRIPTION
 * The generic Render Manager interface.
 */
#pragma once

#include <memory>

#include "Render/Window.h"

namespace Sim {

	class RenderManager {

	protected:
		std::unique_ptr <Window> _window;

	public:
		RenderManager () = default;
		virtual ~RenderManager () = default;

		RenderManager (const RenderManager&) = delete;
		RenderManager& operator = (const RenderManager&) = delete;

		virtual bool Initialize (const char* config) = 0;
		virtual void Update () = 0;
		virtual void Cleanup () = 0;
	};
}
