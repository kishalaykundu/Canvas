/**
 * @file GLProgram.h
 * @author Kishalay Kundu <kishalay.kundu@gmail.com>
 * @section LICENSE
 * See LICENSE.txt included in this package
 *
 * @section DESCRIPTION
 * See GLProgram.h.
 */
#include <cstring>
#include <string>
#include <iostream>
#include <fstream>
#include "tinyxml2.h"

#include "Log.h"
#include "GLProgram.h"

using std::string;
using std::ifstream;
using std::ios;
using tinyxml2::XMLElement;

namespace Sim {

	bool GLProgram::Initialize (XMLElement& element)
	{
		const char* location = element.Attribute ("Location");
		if (location == nullptr){
			LOG_ERROR ("No location specified for GL program");
			return false;
		}
		_location = string (location);
		if (_location [_location.size () - 1] != '/'){
			_location += "/";
		}

		const char* format = element.Attribute ("Format");
		if (format != nullptr){
			if (!strcmp ("SPIR-V", format)){
				_spirv = true;
			}
		}

		/**
		 * Load the filenames for all shader types. This is used if the
		 * shaders have to be reloaded in the future.
		 */
		XMLElement* shaderlist = element.FirstChildElement ("Shader");
		if (shaderlist == nullptr){
			LOG_ERROR ("No shaders specified for GL program");
			return false;
		}
		while (shaderlist != nullptr){
			const char* type = shaderlist->Attribute ("Type");
			if (type == nullptr){
				LOG_ERROR ("No type specified for GL shader");
				return false;
			}
			const char* file = shaderlist->Attribute ("Filename");
			if (type == nullptr){
				LOG_ERROR ("No filename specified for GL shader");
				return false;
			}
			if (!strcmp ("Vertex", type)){
				_vert = string (file);
			}
			else if (!strcmp ("TessCtrl", type)){
				_ctrl = string (file);
			}
			else if (!strcmp ("TessEval", type)){
				_eval = string (file);
			}
			else if (!strcmp ("Geometry", type)){
				_geom = string (file);
			}
			else if (!strcmp ("Fragment", type)){
				_frag = string (file);
			}
			else if (!strcmp ("Compute", type)){
				_compute = string (file);
			}
			else {
				LOG_ERROR ("Unidentified shader type \'" << type << "\' specified for " << file);
				return false;
			}

			shaderlist = shaderlist->NextSiblingElement ("Shader");
		}

		// check to see that compute shaders and the rest are mutually exclusive
		if (!_compute.empty()){
			if (!_vert.empty () || !_ctrl.empty () || !_eval.empty () || !_geom.empty () || !_frag.empty ()){
				LOG_ERROR ("Compute shader " << _compute << " specified along with other shaders (forbidden)");
				return false;
			}
		}
		// check if at least vertex and fragment shader stages exist
		else if (_vert.empty () || _frag.empty ()){
			LOG_ERROR ("Incomplete shader specification: Vertex and Fragment shaders mandatory");
			return false;
		}
		// check that for tessellation, both evaluation and control shaders exist
		else if (!_ctrl.empty () || !_eval.empty ()){
			if (_ctrl.empty () || _eval.empty ()){
				LOG_ERROR ("Tessellation shader problem: Control and evaluation shaders must exist together");
				return false;
			}
		}

		if (!Load ()){
			LOG_ERROR ("Could not load GL program");
			return false;
		}
		return true;
	}

	void GLProgram::Cleanup ()
	{
		if (_id){
			glDeleteProgram (_id); LOG_GL_ERROR ();
		}
	}

	bool GLProgram::Load ()
	{
		if (_compute.empty ()){
			return LoadGraphicsProgram ();
		}

		return LoadComputeProgram ();
	}

	// function to reset - detach all shaders and mark them for deletion
	bool GLProgram::Reset ()
	{
		if (!_id){
			LOG_ERROR ("Program with id 0 cannot be reset");
			return false;
		}
		GLint count = 0;
		glGetProgramiv (_id, GL_ATTACHED_SHADERS, &count);

		if (!count){
			LOG_ERROR ("No attached shaders found for " << _id);
			return false;
		}

		GLuint* shaders = new GLuint [count];
		glGetAttachedShaders (_id, count, NULL, shaders); LOG_GL_ERROR ();
		for (auto i = 0; i < count; ++i){
			glDetachShader (_id, shaders [i]); LOG_GL_ERROR ();
			glDeleteShader (shaders [i]); LOG_GL_ERROR ();
		}

		delete [] shaders;

		return true;
	}

	// method to load graphics program
	bool GLProgram::LoadGraphicsProgram ()
	{
		// load vertex shader
		GLuint vertId = 0;
		if (!(vertId = LoadShader (GL_VERTEX_SHADER, _vert))){
			LOG_ERROR ("Could not load " << _vert);
			return false;
		}

		// optionally load tessellation control and evaluation shaders
		GLuint ctrlId = 0, evalId = 0;
		if (!_ctrl.empty ()){
			if (!(ctrlId = LoadShader (GL_TESS_CONTROL_SHADER, _ctrl))){
				LOG_ERROR ("Could not load " << _ctrl);
				return false;
			}
			if (!(evalId = LoadShader (GL_TESS_EVALUATION_SHADER, _eval))){
				LOG_ERROR ("Could not load " << _eval);
				return false;
			}
		}

		// optionally load geometry shader
		GLuint geomId = 0;
		if (!_geom.empty ()){
			if (!(geomId = LoadShader (GL_GEOMETRY_SHADER, _geom))){
				LOG_ERROR ("Could not load " << _geom);
				return false;
			}
		}

		// load fragment shader
		GLuint fragId = 0;
		if (!(fragId = LoadShader (GL_FRAGMENT_SHADER, _frag))){
			LOG_ERROR ("Could not load " << _frag);
			return false;
		}

		// detach and delete all shaders for already existing program
		if (_id){
			Reset ();
		}
		// create new program
		else {
			_id = glCreateProgram (); LOG_GL_ERROR ();
			if (!_id){
				LOG_ERROR ("Could not create new GL program");
				return false;
			}
		}

		// attach all the shaders (vertex and fragment shaders mandatory)
		glAttachShader (_id, vertId); LOG_GL_ERROR ();
		if (ctrlId){
			glAttachShader (_id, ctrlId); LOG_GL_ERROR ();
			glAttachShader (_id, evalId); LOG_GL_ERROR ();
		}
		if (geomId){
			glAttachShader (_id, geomId); LOG_GL_ERROR ();
		}
		glAttachShader (_id, fragId); LOG_GL_ERROR ();

		glLinkProgram (_id); LOG_GL_ERROR ();

		GLint status = GL_FALSE;
		glGetProgramiv (_id, GL_LINK_STATUS, &status);

		if (status != GL_TRUE){
			GLint length = 0;

			glGetProgramiv (_id, GL_INFO_LOG_LENGTH, &length);
			++length;

			char* error = new char [length];
			glGetProgramInfoLog (_id, length, NULL, error);

			LOG_ERROR ("GLSL Program Link Error:");
			LOG_ERROR (error);

			delete [] error;
			return false;
		}

		// delete all the shaders that have been used
		glDeleteShader (vertId); LOG_GL_ERROR ();
		if (ctrlId){
			glDeleteShader (ctrlId); LOG_GL_ERROR ();
			glDeleteShader (evalId); LOG_GL_ERROR ();
		}
		if (geomId){
			glDeleteShader (geomId); LOG_GL_ERROR ();
		}
		glDeleteShader (fragId); LOG_GL_ERROR ();

		return true;
	}

	// method to load compute program
	bool GLProgram::LoadComputeProgram ()
	{
		// load compute shader
		GLuint computeId = 0;
		if (!(computeId = LoadShader (GL_COMPUTE_SHADER, _compute))){
			LOG_ERROR ("Could not load " << _compute);
			return false;
		}

		// detach and delete all shaders for already existing program
		if (_id){
			Reset ();
		}
		// create new program
		else {
			_id = glCreateProgram (); LOG_GL_ERROR ();
			if (!_id){
				LOG_ERROR ("Could not create new GL program");
				return false;
			}
		}
		glAttachShader (_id, computeId); LOG_GL_ERROR ();

		glLinkProgram (_id); LOG_GL_ERROR ();

		GLint status = GL_FALSE;
		glGetProgramiv (_id, GL_LINK_STATUS, &status);

		if (status != GL_TRUE){
			GLint length = 0;

			glGetProgramiv (_id, GL_INFO_LOG_LENGTH, &length);
			++length;

			char* error = new char [length];
			glGetProgramInfoLog (_id, length, NULL, error);

			LOG_ERROR ("GLSL Program Link Error:");
			LOG_ERROR (error);

			delete [] error;
			return false;
		}
		glDeleteShader (computeId); LOG_GL_ERROR ();

		return true;
	}

	GLuint GLProgram::LoadShader (const GLenum type, const string& filename)
	{
		GLuint shaderId = glCreateShader (type); LOG_GL_ERROR ();
		if (!shaderId){
			LOG_ERROR ("Could not create GL shader for " << filename);
			return 0;
		}

    ifstream is;
    string file (_location + filename);
    is.open (file.c_str (), ios::binary);
    if (!is){
    	LOG_ERROR ("Could not open " << filename);
    }
    is.seekg (0, ios::end);
    size_t length = is.tellg ();
    is.seekg (0, ios::beg);
    char *src = new char [length + 1];
    is.read (src, length);
    is.close ();
    src [length] = '\0';

    string code (src);
    delete [] src;

    const char *source = code.c_str ();
    glShaderSource (shaderId, 1, &source, NULL); LOG_GL_ERROR ();

    glCompileShader (shaderId); LOG_GL_ERROR ();

    GLint status = GL_FALSE;
    glGetShaderiv (shaderId, GL_COMPILE_STATUS, &status);

    if (status != GL_TRUE){
    	GLint length = 0;

    	glGetShaderiv (shaderId, GL_INFO_LOG_LENGTH, &length);
    	++length;

    	char* error = new char [length];
    	glGetShaderInfoLog (shaderId, length, NULL, error);

    	LOG_ERROR ("GLSL shader error [" << filename << "]:");
    	LOG_ERROR (error);

    	delete [] error;
    	return 0;
    }

		return shaderId;
	}
}
