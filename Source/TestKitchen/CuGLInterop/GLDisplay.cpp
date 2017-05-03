/*
 * GLXDisplay.cpp
 *
 *  Created on: 08-Mar-2017
 *      Author: kishalay
 */

#include <iostream>
#include <memory>

#include "Mesh.h"
#include "GLDisplay.h"

using std::cout;
using std::cerr;
using std::endl;
using std::shared_ptr;

namespace Sim {

	GLDisplay::GLDisplay ()
	: _runFlag (false), _top (0), _left (0), _width (256), _height (256), _colorDepth (16),
		_display (nullptr), _contextAttributes (nullptr)
	{ }

	GLDisplay::~GLDisplay () {}

	// initialization method
	bool GLDisplay::Initialize (shared_ptr <Sim::Mesh>& m)
	{
		if (!InitializeGLX ()){
			return false;
		}

		_runFlag = true;
		return true;
	}

	// run method
	void GLDisplay::Run ()
	{
	  // make created context current
		glXMakeContextCurrent (_display, _window, _window, _context);

		while (_runFlag){

		}
	}

	void GLDisplay::Cleanup ()
	{
		DestroyWindow ();
		delete [] _contextAttributes;
	}

	Display* GLDisplay::GetDisplay () const
	{
		return _display;
	}
	GLXFBConfig GLDisplay::GetConfig () const
	{
		return _config;
	}
	GLXContext GLDisplay::GetContext () const
	{
		return _context;
	}
	int* GLDisplay::GetContextAttributes () const
	{
		return _contextAttributes;
	}

	// method to initialize the GLX rendering system
	bool GLDisplay::InitializeGLX ()
	{
		_display = XOpenDisplay (NULL);
		if (!_display){
			cerr << "Could not open X Display system" << endl;
			return false;
		}

	  // check for minimum version of GLX
	  int major = 0, minor = 0;
	  if (!glXQueryVersion (_display, &major, &minor)){
	  	cerr << "Could not get GLX version for display" << endl;
	    return false;
	  }
	  if (((major == SIM_GLX_MIN_MAJOR_VERSION) && (minor < SIM_GLX_MIN_MINOR_VERSION)) || (major < SIM_GLX_MIN_MAJOR_VERSION)){
	  	cerr << "GLX version: " << major << "." << minor << ". Minimum required: 1.4" << endl;
	    return false;
	  }

	  // framebuffer configuration attributes
	  int attributes [] = {
	  		GLX_X_RENDERABLE, True,
	  		GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
	  		GLX_RENDER_TYPE, GLX_RGBA_BIT,
				GLX_X_VISUAL_TYPE, GLX_TRUE_COLOR,
	      GLX_RED_SIZE, 8,
				GLX_GREEN_SIZE, 8,
				GLX_BLUE_SIZE, 8,
				GLX_ALPHA_SIZE, 8,
	      GLX_DEPTH_SIZE, 24,
				GLX_STENCIL_SIZE, 8,
				GLX_DOUBLEBUFFER, True,
				None };

	  // initialize framebuffer config
	  int fbCount = 0;
	  GLXFBConfig* fbConfig = glXChooseFBConfig (_display, DefaultScreen(_display), attributes, &fbCount);
	  if (!fbConfig){
	  	cerr << "Could not retrieve GLX framebuffer config" << endl;
	  	return false;
	  }

	  // choose the framebuffer config with most samples per pixel
	  int bestFB = -1, worstFB = -1, bestNumSamples = -1, worstNumSamples = -1;
	  for (int i = 0; i < fbCount; ++i){
	  	XVisualInfo* info = glXGetVisualFromFBConfig (_display, fbConfig [i]);
	  	if (info){
	  		int buffers, samples;
	  		glXGetFBConfigAttrib (_display, fbConfig [i], GLX_SAMPLE_BUFFERS, &buffers);
	  		glXGetFBConfigAttrib (_display, fbConfig [i], GLX_SAMPLES, &samples);

	  		if (bestFB < 0 || (buffers && samples > bestNumSamples)){
	  			bestFB = i;
	  			bestNumSamples = samples;
	  		}
	  		if (worstFB < 0 || !buffers || samples < worstNumSamples){
	  			worstFB = i;
	  			worstNumSamples = samples;
	  		}
	  	}
	  	XFree (info);
	  }
	  _config = fbConfig [bestFB];
	  XFree (fbConfig);

	  // get an X Visual
	  XVisualInfo* info = glXGetVisualFromFBConfig (_display, _config);
	  XSetWindowAttributes swa;
	  Colormap cmap;
	  swa.colormap = cmap = XCreateColormap(_display, RootWindow (_display, info->screen), info->visual, AllocNone );
	  swa.background_pixmap = None;
	  swa.border_pixel = 0;
	  swa.event_mask = StructureNotifyMask;

	  // create display window
	  _colorDepth = info->depth;
	  _window = XCreateWindow (_display, RootWindow (_display, info->screen), _top, _left, _width, _height,
	  		0, _colorDepth, InputOutput, info->visual, CWBorderPixel | CWColormap | CWEventMask, &swa);
	  if (!_window){
	  	cerr << "Could not create display window" << endl;
	  	XFree (info);
	  	return false;
	  }
	  XFree (info);

	  // form window title
		XStoreName (_display, _window, "Chimera GLX Window");

		// map window to display
		XMapWindow (_display, _window);

		// create new context
#		ifndef NDEBUG
		_contextAttributes = new int [7];
		_contextAttributes [4] = GLX_CONTEXT_FLAGS_ARB;
		_contextAttributes [5] = GLX_CONTEXT_DEBUG_BIT_ARB;
		_contextAttributes [6] = None;
#		else
		_contextAttributes = new int [5];
		_contextAttributes [4] = None;
#		endif
		_contextAttributes [0] = GLX_CONTEXT_MAJOR_VERSION_ARB;
		_contextAttributes [1] = SIM_GL_MIN_MAJOR_VERSION;
		_contextAttributes [2] = GLX_CONTEXT_MINOR_VERSION_ARB;
		_contextAttributes [3] = SIM_GL_MIN_MINOR_VERSION;

		_context = glXCreateContextAttribsARB (_display, _config, 0, true, _contextAttributes);
		if (!_context){
			cerr << "Could not create GLX context" << endl;
		  return false;
		}
	  if (glXIsDirect (_display, _context) == True){
	  	cout << "Direct GL rendering context available" << endl;
	  } else {
	  	cout << "Indirect GL rendering context available" << endl;
	  }

		return true;
	}

	// method to destroy display window
	void GLDisplay::DestroyWindow ()
	{
    glXMakeContextCurrent (_display, None, None, NULL);
 		glXDestroyContext (_display, _context);
 		XDestroyWindow (_display, _window);
 		XCloseDisplay (_display);
	}
}
