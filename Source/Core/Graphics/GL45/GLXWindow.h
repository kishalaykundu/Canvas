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
#include "Graphics/GL45/GLWindow.h"

namespace Sim {

	class GLXWindow : public GLWindow {

	protected:
		// X-Window Attributes
		::Window _window;
		::Display *_display = nullptr;
		::GLXFBConfig _config;
		::GLXContext _context = 0;
		int* _contextAttributes = nullptr;

		::XWindowAttributes _attributes;
		::XEvent _event;

	public:
		GLXWindow () = delete;
		~GLXWindow () {Cleanup ();}

		GLXWindow (const GLXWindow&) = delete;
		GLXWindow& operator = (const GLXWindow&) = delete;

		GLXWindow (unsigned int width, unsigned int height, unsigned int colorDepth);

		virtual bool Initialize (const char* config) override;
		virtual void Update () override;
		virtual void Cleanup () override;

		virtual void Resize (unsigned int width, unsigned int height) override;
		virtual void SwapBuffers () override;

		virtual void MakeContextCurrent () override;
		virtual void ReleaseContext () override;

		bool NewContext (Display*& display, GLXContext& newContext);
	};
}
