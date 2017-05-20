/**
 * @file GLXWindow.cpp
 * @author Kishalay Kundu <kishalay.kundu@gmail.com>
 * @section LICENSE
 * See LICENSE.txt included in this package
 *
 * @section DESCRIPTION
 * See GLXWindow.h.
 */

#include "Log.h"
#include "Graphics/GL45/GLXWindow.h"

namespace Sim {

	GLXWindow::GLXWindow (unsigned int w, unsigned int h, unsigned int c)
	: _width (w), _height (h), _colorDepth (c) {}

	bool GLXWindow::Initialize (const char* config)
	{
		_display = XOpenDisplay (NULL);
		if (!_display){
			LOG_ERROR ("Could not open X Display system");
			return false;
		}

	  // check for minimum version of GLX
	  int major = 0, minor = 0;
	  if (!glXQueryVersion (_display, &major, &minor)){
	    LOG_ERROR("Could not get GLX version for display");
	    return false;
	  }
	  if (((major == SIM_GLX_MIN_MAJOR_VERSION) && (minor < SIM_GLX_MIN_MINOR_VERSION)) || (major < SIM_GLX_MIN_MAJOR_VERSION)){
	    LOG_ERROR("GLX version: " << major << "." << minor << ". Minimum required: 1.4");
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
	  	LOG_ERROR ("Could not retrieve GLX framebuffer config");
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
	  	LOG_ERROR ("Could not create display window");
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
			LOG_ERROR ("Could not create GLX context");
		  return false;
		}

		XSync (_display, False);

	  if (glXIsDirect (_display, _context) == True){
	  	LOG ("Direct GL rendering context available");
	  } else {
	  	LOG ("Indirect GL rendering context available");
	  }

	  MakeContextCurrent ();
	  if (gl3wInit ()){
	  	LOG_ERROR ("GL3W and OpenGL could not be initialized");
	  	return false;
	  }
	  if (!gl3wIsSupported(4, 5)){
	  	LOG_ERROR ("OpenGL 4.5 not supported");
	  	return false;
	  }

		return true;
	}

	void GLXWindow::Update ()
	{
	 	XNextEvent(_display, &_event);

	 	switch (_event.type) {

	 	case Expose:
	 	{
			XGetWindowAttributes(_display, _window, &_attributes);
			if (_width != static_cast <unsigned int> (_attributes.width) ||
					_height != static_cast <unsigned int> (_attributes.height)){
				Resize (_attributes.width, _attributes.height);
				//_renderer->UpdateProjection ();
			}
			break;
	 	}
	 	case ButtonPress:
	 	{
	 		//_renderer->Mouse (_event.xbutton.button, _event.xbutton.x, _event.xbutton.y);
	 		break;
	 	}
	 	case MotionNotify:
	 	{
	 		switch (_event.xmotion.state){
	 			case Button1Mask:
	 				//_renderer->LeftMouseMotion (_event.xmotion.x, _event.xmotion.y);
	 				break;
	 			case Button2Mask:
	 				//_renderer->RightMouseMotion (_event.xmotion.x, _event.xmotion.y);
	 				break;
	 			case Button3Mask:
	 				//_renderer->MiddleMouseMotion (_event.xmotion.x, _event.xmotion.y);
	 				break;
	 		}
	 		break;
	 	}
	 	case KeyPress :
	 	{
			char buff [20];
			unsigned int buffsize = 20;
			KeySym key;
			XComposeStatus compose;
			XLookupString (&_event.xkey, buff, buffsize, &key, &compose);

			LOG ("Input Key: " << buff);
			switch (key) {

			case XK_q: case XK_Q: case XK_Escape:
			{
		 		Driver::Instance ().Quit ();
				break;
			}

			default:
				break;
			}
	 		break;
	 	}
	 	default:
	 		LOG_ERROR ("Unrecognized XEvent");
	 		break;
	 	}
	}

	void GLXWindow::Cleanup ()
	{
		// this check allows cleanup to be called safely
		if (_contextAttributes){
			delete [] _contextAttributes;
			_contextAttributes = nullptr;
			_context = 0;
	    glXMakeContextCurrent (_display, None, None, NULL);
	 		glXDestroyContext (_display, _context);
	 		XDestroyWindow (_display, _window);
	 		XCloseDisplay (_display);
		}
	}

	void GLXWindow::Resize (unsigned int width, unsigned int height)
	{
		_width = width;
		_height = height;
		XResizeWindow (_display, _window, _width, _height);
	}

	void GLXWindow::SwapBuffers ()
	{
		glXSwapBuffers (_display, _window);
	}

	void GLXWindow::MakeContextCurrent ()
	{
		glXMakeContextCurrent (_display, _window, _window, _context);
	}

	void GLXWindow::ReleaseContext ()
	{
    glXMakeContextCurrent (_display, None, None, NULL);
	}

	bool GLXWindow::NewContext (Display*& display, GLXContext& context)
	{
		display = _display;
		context = glXCreateContextAttribsARB (display, _config, _context, true, _contextAttributes); LOG_GL_ERROR ();

		if (!context){
			LOG_ERROR ("Could not create new GLX context");
		  return false;
		}
		glXMakeContextCurrent (display, 0, 0, context); LOG_GL_ERROR ();
		XSync (display, False);
		return true;
	}
}
