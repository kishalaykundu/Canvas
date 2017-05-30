/**
 * @file GLXWindow.h
 * @author Kishalay Kundu <kishalay.kundu@gmail.com>
 * @section LICENSE
 * See LICENSE.txt included in this package
 *
 * @section DESCRIPTION
 * The OpenGL-based XWindow class for the Canvas system. This is
 * an extension of the generic OpenGL Window interface.
 */
#pragma once

#include "GL/GLUtils.h"
#include "Render/Window.h"

namespace Sim {

	class GLXWindow : public Window {

		friend class GLManager;

	protected:
		// X-Window Attributes
		::Window _window = 0;
		::Display *_display = nullptr;
		::GLXFBConfig _config = nullptr;
		::GLXContext _context = 0;
		int* _contextAttributes = nullptr;

		::XWindowAttributes _attributes;
		::XEvent _event;

	public:
		GLXWindow () = delete;
		~GLXWindow () {Cleanup ();}

		GLXWindow (const GLXWindow&) = delete;
		GLXWindow& operator = (const GLXWindow&) = delete;

		GLXWindow (unsigned int w, unsigned int h, int c) : Window (w, h, c) {}

		bool Initialize () override;
		void Update () override;
		void Cleanup () override;

		void Resize (unsigned int width, unsigned int height) override;
		void SwapBuffers () override;

		void MakeContextCurrent () override;
		void ReleaseContext () override;

		bool NewContext (Display*& display, GLXContext& newContext);
	};
}
