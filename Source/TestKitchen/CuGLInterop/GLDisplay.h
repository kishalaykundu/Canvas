/**
 * @file GLDisplay.h
 * @author Kishalay Kundu <kishalay.kundu@gmail.com>
 * @section LICENSE
 * See LICENSE.txt included in this package
 *
 * @section DESCRIPTION
 * OpenGL based display thread.
 */
#pragma once

#include <memory>

#include "GL/GLUtils.h"
#include "X11/X11.h"

namespace Sim {

	class Mesh;

	class GLDisplay {

	protected:
		bool _runFlag;

		int _top;
		int _left;

		unsigned int _width;
		unsigned int _height;
		int _colorDepth;

		::Window _window;
		::Display *_display;
		::GLXFBConfig _config;
		::GLXContext _context;
		int* _contextAttributes;
		::XEvent _event;

	public:
		GLDisplay ();
		~GLDisplay ();
		bool Initialize (std::shared_ptr <Mesh>&);
		void Run ();
		void Cleanup ();

		Display* GetDisplay () const;
		GLXFBConfig GetConfig () const;
		GLXContext GetContext () const;
		int* GetContextAttributes () const;

		void DestroyWindow ();

	protected:
		bool InitializeGLX ();
	};
}
