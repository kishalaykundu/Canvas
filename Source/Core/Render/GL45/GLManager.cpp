/**
 * @file GLManager.cpp
 * @author Kishalay Kundu <kishalay.kundu@gmail.com>
 * @section LICENSE
 * See LICENSE.txt included in this package
 *
 * @section DESCRIPTION
 * See GLManager.h.
 */

#include <memory>

#include "tinyxml2.h"

#include "Preprocess.h"
#include "Log.h"

#include "ConfigParser.h"
#include "GL/GLUtils.h"
#include "Render/Window.h"
#include "Render/GL45/GLXWindow.h"
#include "Render/GL45/GLManager.h"

using std::make_shared;
using tinyxml2::XMLElement;
using tinyxml2::XMLError;
using tinyxml2::XML_SUCCESS;

namespace Sim {

	bool GLManager::Initialize (const char* config)
	{
		// read configuration file
		ConfigParser parser;
		if (!parser.Initialize (config, "GLConfig")){
			LOG_ERROR ("Could not initialize parser for " << config);
			return false;
		}

		XMLElement* element = parser.GetElement ("Window");
		if (element == nullptr){
			LOG_ERROR ("No display window profile specified in " << config);
			return false;
		}

		if (!InitializeWindow (*element)){
			LOG_ERROR ("Could not initialize display window from " << config);
			return false;
		}
		element = nullptr;

		element = parser.GetElement ("Renderer");
		if (element == nullptr){
			LOG_ERROR ("No renderer profile specified in " << config);
			return false;
		}

		XMLElement* relem = element->FirstChildElement ("Background");
		if (relem != nullptr){
			SetWindowBackground (*relem);
		}
		relem = nullptr;

		relem = element->FirstChildElement ("Camera");
		if (relem != nullptr){
			SetCameraPosition (*relem);
		}
		relem = nullptr;

		relem = element->FirstChildElement ("DirectionalLights");
		if (relem != nullptr){
			if (!SetDirectionalLights (*relem)){
				LOG_ERROR ("Could not initialize directional lights");
				return false;
			}
		}

		EnableGLAttribs ();

		LOG ("OpenGL Render manager initialized");
		return true;
	}

	void GLManager::Update ()
	{
		// clear from previous frame
		glClearColor (_background [0], _background [1], _background [2], 0.);
		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void GLManager::Cleanup ()
	{
	}

	void GLManager::Mouse (unsigned int b, int x, int y)
	{
		_mouseButton = b;
		_mouseX = x;
		_mouseY = y;
	}

	bool GLManager::InitializeWindow (XMLElement& element)
	{
		XMLElement* elem = element.FirstChildElement ("Position");

		XMLError error = XML_SUCCESS;

		int top = 0, left = 0;
		if (elem != nullptr){
			if ((error = elem->QueryIntAttribute ("Top", &top)) != XML_SUCCESS){
				LOG_ERROR ("Could not read Top position placement of window from config file");
				return false;
			}
			if ((error = elem->QueryIntAttribute ("Left", &left)) != XML_SUCCESS){
				LOG_ERROR ("Could not read Left position placement of window from config file");
				return false;
			}
		}

		elem = element.FirstChildElement ("Dimensions");
		unsigned int width = SIM_DEFAULT_WINDOW_WIDTH, height = SIM_DEFAULT_WINDOW_HEIGHT;
		if (elem != nullptr){
			if ((error = elem->QueryUnsignedAttribute ("Width", &width)) != XML_SUCCESS){
				LOG_ERROR ("Could not read window width from config file");
				return false;
			}
			if ((error = elem->QueryUnsignedAttribute ("Height", &height)) != XML_SUCCESS){
				LOG_ERROR ("Could not read window height from config file");
				return false;
			}
		}
		elem = element.FirstChildElement ("PixelDepth");
		int colorDepth = SIM_DEFAULT_PIXEL_COLOR_DEPTH;
		if (elem != nullptr){
			if ((error = elem->QueryIntText (&colorDepth)) != XML_SUCCESS){
				LOG_ERROR ("Could not read Left position placement of window from config file");
				return false;
			}
		}

		_window = make_unique <GLXWindow> (GLXWindow (width, height, colorDepth));

		return _window->Initialize ();
	}

	void GLManager::SetWindowBackground (XMLElement& elem)
	{
		XMLElement* celem = elem.FirstChildElement ("Color");
		if (celem != nullptr){

			Real r = 0., g = 0., b = 0.;
#			ifdef SIM_DOUBLE_PRECISION
			celem->QueryDoubleAttribute ("R", &r);
			celem->QueryDoubleAttribute ("G", &g);
			celem->QueryDoubleAttribute ("B", &b);
#			else
			celem->QueryFloatAttribute ("R", &r);
			celem->QueryFloatAttribute ("G", &g);
			celem->QueryFloatAttribute ("B", &b);
#			endif

			_background [0] = static_cast <GLreal> (r);
			_background [1] = static_cast <GLreal> (g);
			_background [2] = static_cast <GLreal> (b);
		}
	}

	void GLManager::SetCameraPosition (XMLElement& elem)
	{
		XMLElement* celem = elem.FirstChildElement ("Position");
		if (celem != nullptr){

			Real x = 0., y = 0., z = 0.;
#			ifdef SIM_DOUBLE_PRECISION
			celem->QueryDoubleAttribute ("X", &x);
			celem->QueryDoubleAttribute ("Y", &y);
			celem->QueryDoubleAttribute ("Z", &z);
#			else
			celem->QueryFloatAttribute ("X", &x);
			celem->QueryFloatAttribute ("Y", &y);
			celem->QueryFloatAttribute ("Z", &z);
#			endif

			_cameraPosition [0] = static_cast <GLreal> (x);
			_cameraPosition [1] = static_cast <GLreal> (y);
			_cameraPosition [2] = static_cast <GLreal> (z);
		}
	}

	bool GLManager::SetDirectionalLights (XMLElement& elem)
	{
		unsigned int count = 0;
		elem.QueryUnsignedAttribute ("Count", &count);
		if (!count){
			LOG_ERROR ("Directional light count not specified");
			return false;
		}
		_numLights = count;

		XMLElement* lelem = elem.FirstChildElement ("Light");
		if (lelem == nullptr){
			LOG_ERROR ("Directional Lights specified but no \'Light\' element retrieved");
			return false;
		}
		_directionalLights = make_shared <DirectionalLight> (new DirectionalLight [count], DELETE_ARRAY <DirectionalLight> ());

		unsigned int i = 0;
		while (lelem != nullptr && i < count){

			DirectionalLight light;
			XMLElement* celem = lelem->FirstChildElement ("Direction");
			if (celem != nullptr){
				Real x = 0., y = 0., z = 0;
#				ifndef SIM_DOUBLE_PRECISION
				celem->QueryFloatAttribute ("X", &x);
				celem->QueryFloatAttribute ("Y", &y);
				celem->QueryFloatAttribute ("Z", &z);
#				else
				celem->QueryDoubleAttribute ("X", &x);
				celem->QueryDoubleAttribute ("Y", &y);
				celem->QueryDoubleAttribute ("Z", &z);
#				endif
				light._direction [0] = static_cast <GLreal> (x);
				light._direction [1] = static_cast <GLreal> (y);
				light._direction [2] = static_cast <GLreal> (z);
			}

			celem = lelem->FirstChildElement ("Ambient");
			if (celem != nullptr){
				Real r = 0., g = 0., b = 0;
#				ifndef SIM_DOUBLE_PRECISION
				celem->QueryFloatAttribute ("R", &r);
				celem->QueryFloatAttribute ("G", &g);
				celem->QueryFloatAttribute ("B", &b);
#				else
				celem->QueryDoubleAttribute ("R", &r);
				celem->QueryDoubleAttribute ("G", &g);
				celem->QueryDoubleAttribute ("B", &b);
#				endif
				light._ambient [0] = static_cast <GLreal> (r);
				light._ambient [1] = static_cast <GLreal> (g);
				light._ambient [2] = static_cast <GLreal> (b);
			}

			celem = lelem->FirstChildElement ("Diffuse");
			if (celem != nullptr){
				Real r = 0., g = 0., b = 0;
#				ifndef SIM_DOUBLE_PRECISION
				celem->QueryFloatAttribute ("R", &r);
				celem->QueryFloatAttribute ("G", &g);
				celem->QueryFloatAttribute ("B", &b);
#				else
				celem->QueryDoubleAttribute ("R", &r);
				celem->QueryDoubleAttribute ("G", &g);
				celem->QueryDoubleAttribute ("B", &b);
#				endif
				light._diffuse [0] = static_cast <GLreal> (r);
				light._diffuse [1] = static_cast <GLreal> (g);
				light._diffuse [2] = static_cast <GLreal> (b);
			}

			celem = lelem->FirstChildElement ("Specular");
			if (celem != nullptr){
#				ifndef SIM_DOUBLE_PRECISION
				celem->QueryFloatText (&light._specular);
#				else
				celem->QueryDoubleAttribute ("Value", &light._specular);
#				endif
			}

			celem = lelem->FirstChildElement ("Exponent");
			if (celem != nullptr){
#				ifndef SIM_DOUBLE_PRECISION
				celem->QueryFloatText (&light._exponent);
#				else
				celem->QueryDoubleAttribute ("Value", &light._exponent);
#				endif
			}

			_directionalLights.get () [i] = light;
			++i; lelem = lelem->NextSiblingElement ("Light");
		}
		if (i < count){
			_numLights = i;
			LOG_WARNING ("Directional Light \'Count\'" << count << " does not correspond with number of lights specified - " << i);
		}
		return true;
	}

	void GLManager::EnableGLAttribs ()
	{
		glEnable (GL_BLEND);

		glEnable (GL_CULL_FACE);
		glCullFace (GL_BACK);

		glEnable (GL_DEPTH_TEST);
		glDepthFunc (GL_LESS);

		glEnable (GL_LINE_SMOOTH);
		glEnable (GL_POLYGON_SMOOTH);

#		ifndef NDEBUG
		glEnable (GL_DEBUG_OUTPUT);
		glEnable (GL_DEBUG_OUTPUT_SYNCHRONOUS);
#		endif
	}
}
