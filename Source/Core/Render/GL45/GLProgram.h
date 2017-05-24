/**
 * @file GLProgram.h
 * @author Kishalay Kundu <kishalay.kundu@gmail.com>
 * @section LICENSE
 * See LICENSE.txt included in this package
 *
 * @section DESCRIPTION
 * GLSL Program handler.
 */
#pragma once

#include <string>
#include "tinyxml2.h"

#include "Preprocess.h"
#include "GL/GLUtils.h"

namespace Sim {

	class GLProgram {

	protected:
		GLuint _id = 0;
		bool _spirv = false;
		// folder where program is kept
		std::string _location;
		// names of different shaders
		std::string _vert, _ctrl, _eval, _geom, _frag, _compute;

	public:
		GLProgram () = default;
		~GLProgram () {Cleanup ();}

		GLProgram (const GLProgram&) = default;
		GLProgram& operator = (const GLProgram&) = default;

		GLuint Id () const {return _id;}

		bool Initialize (tinyxml2::XMLElement&);
		bool Load ();
		void Cleanup ();

	protected:
		bool Reset ();
		bool LoadGraphicsProgram ();
		bool LoadComputeProgram ();
		GLuint LoadShader (const GLenum shaderType, const std::string& filename);
	};
}
