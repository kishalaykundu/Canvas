/**
 * @file GLManager.h
 * @author Kishalay Kundu <kishalay.kundu@gmail.com>
 * @section LICENSE
 * See LICENSE.txt included in this package
 *
 * @section DESCRIPTION
 * The OpenGL based Render Manager interface.
 */
#pragma once

#include <memory>

#include "tinyxml2.h"

#include "GL/GLUtils.h"
#include "Render/RenderManager.h"

namespace Sim {

	class GLProgram;

	class GLManager : public RenderManager {

		class DirectionalLight {

			GLreal _direction [3] = {0., 0., 0.};
			GLreal _ambient [3] = {0., 0., 0.};
			GLreal _diffuse [3] = {0., 0., 0.};
			GLreal _specular = 0.;
			GLreal _exponent = 0.;

			DirectionalLight () = default;
			DirectionalLight (const DirectionalLight& l)
	    : _specular (l._specular), _exponent (l._exponent)
	    {
	    	memcpy (_direction, l._direction, 3*sizeof (GLreal));
	    	memcpy (_ambient, l._ambient, 3*sizeof (GLreal));
	    	memcpy (_diffuse, l._diffuse, 3*sizeof (GLreal));
	    }
		};

	protected:
		AxisAlignedBox _bounds;
		GLreal _hither = 1., _yon = 1500., _fov = 45.;
		GLreal _background [3] = {0., 0., 0.}; // background color

		GLreal _projection [16] = {
				1., 0., 0., 0.,
				0., 1., 0., 0.,
				0., 0., 1., 0.,
				0., 0., 0., 1.
		};
		GLreal _modelview [16] = {
				1., 0., 0., 0.,
				0., 1., 0., 0.,
				0., 0., 1., 0.,
				0., 0., 0., 1.
		};

		// mouse-cursor attributes
		int _mouseX = 0, _mouseY = 0;
		unsigned int _mouseButton = 0;

		// camera attributes
		GLreal _cameraPosition [3] = {0., 0., 0.};
		GLreal _cameraScales [3] = {0., 0., 0.};

		std::map <std::string, std::shared_ptr <GLProgram> > _programs;

		unsigned int _numLights = 0;
		std::shared_ptr <DirectionalLight> _directionalLights;

	public:
		bool Initialize (const char* config) override;
		void Update () override;
		void Cleanup () override;

	protected:
		void Mouse (unsigned int, int, int);
		void LeftMouseMotion (int, int);
		void RightMouseMotion (int, int);
		void MiddleMouseMotion (int, int);

		void UpdateProjection ();
		void UpdateModelview ();

		bool InitializeWindow (tinyxml2::XMLElement&);
		void SetWindowBackground (tinyxml2::XMLElement&);
		void SetCameraPosition (tinyxml2::XMLElement&);
		bool SetDirectionalLights (tinyxml2::XMLElement&);
		void EnableGLAttribs ();

	};
}
