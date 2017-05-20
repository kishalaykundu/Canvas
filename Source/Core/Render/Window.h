/**
 * @file Window.h
 * @author Kishalay Kundu <kishalay.kundu@gmail.com>
 * @section LICENSE
 * See LICENSE.txt included in this package
 *
 * @section DESCRIPTION
 * The generic Window interface.
 */
#pragma once

namespace Sim {

	const unsigned int SIM_DEFAULT_WINDOW_WIDTH = 256;
	const unsigned int SIM_DEFAULT_WINDOW_HEIGHT = 256;
	const unsigned int SIM_DEFAULT_PIXEL_COLOR_DEPTH = 16;

	class Window {

	protected:
		int _top = 0;
		int _left = 0;

		unsigned int _width = SIM_DEFAULT_WINDOW_WIDTH;
		unsigned int _height = SIM_DEFAULT_WINDOW_HEIGHT;
		int _colorDepth = SIM_DEFAULT_PIXEL_COLOR_DEPTH;

	public:
		virtual ~Window ();

		virtual bool Initialize () = 0;
		virtual void Update () = 0;
		virtual void Cleanup () = 0;

		virtual void Resize (unsigned int width, unsigned int height) = 0;
		virtual void SwapBuffers () = 0;

		virtual void MakeContextCurrent () = 0;
		virtual void ReleaseContext () = 0;

		unsigned int Width () const {return _width;}
		unsigned int Height () const {return _height;}
		unsigned int ColorDepth () const {return _colorDepth;}
	};
}
