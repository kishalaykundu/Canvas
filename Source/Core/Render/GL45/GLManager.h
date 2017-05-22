/**
 * @file GLManager.h
 * @author Kishalay Kundu <kishalay.kundu@gmail.com>
 * @section LICENSE
 * See LICENSE.txt included in this package
 *
 * @section DESCRIPTION
 * OpenGL based Render Manager.
 */
#pragma once

#include <memory>

#include "tinyxml2.h"

#include "GL/GLUtils.h"
#include "AxisAlignedBox.h"
#include "Render/RenderManager.h"

namespace Sim {

	class GLProgram;

	class GLManager : public RenderManager {

		class DirectionalLight {

			friend class GLManager;

		protected:
			GLreal _direction [3] = {0., 0., 0.};
			GLreal _ambient [3] = {0., 0., 0.};
			GLreal _diffuse [3] = {0., 0., 0.};
			GLreal _specular = 0.;
			GLreal _exponent = 0.;

		public:
			DirectionalLight () = default;
			~DirectionalLight () = default;
			DirectionalLight (const DirectionalLight& l)
	    : _specular (l._specular), _exponent (l._exponent)
	    {
	    	memcpy (_direction, l._direction, 3*sizeof (GLreal));
	    	memcpy (_ambient, l._ambient, 3*sizeof (GLreal));
	    	memcpy (_diffuse, l._diffuse, 3*sizeof (GLreal));
	    }
			DirectionalLight& operator = (const DirectionalLight& l)
			{
	    	memcpy (_direction, l._direction, 3*sizeof (GLreal));
	    	memcpy (_ambient, l._ambient, 3*sizeof (GLreal));
	    	memcpy (_diffuse, l._diffuse, 3*sizeof (GLreal));
	    	_specular = l._specular;
	    	_exponent = l._exponent;
				return *this;
			}
		};

	protected:
		AxisAlignedBox _bounds;
		GLreal _hither = 1., _yon = 1500., _fov = 45.;
		GLreal _background [4] = {0., 0., 0., 0.}; // background color

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
		GLreal _cameraPosition [4] = {0., 0., 0., 1.};
		GLreal _cameraScales [3] = {0., 0., 0.};

		//std::map <std::string, std::shared_ptr <GLProgram> > _programs;

		unsigned int _numLights = 0;
		std::unique_ptr <DirectionalLight []> _directionalLights;

	public:
		GLManager () = default;
		~GLManager () {Cleanup ();}

		GLManager (const GLManager&) = delete;
		GLManager& operator = (const GLManager&) = delete;

		bool Initialize (const char* config) override;
		void Update () override;
		void Cleanup () override;

		bool MakeNewContext (Display*& display, GLXContext& newContext);

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
