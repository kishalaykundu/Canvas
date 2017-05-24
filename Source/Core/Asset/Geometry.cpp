/**
 * @file Geometry.cpp
 * @author Kishalay Kundu <kishalay.kundu@gmail.com>
 * @section LICENSE
 * See LICENSE.txt included in this package
 *
 * @section DESCRIPTION
 * See Geometry.h.
 */

#include <string>
#include <memory>

#include "tinyxml2.h"

#include "Preprocess.h"
#include "Types.h"
#include "Log.h"

#include "Vector.h"
#include "MeshUtils.h"
#include "Asset/Geometry.h"

using std::string;
using std::make_unique;

using tinyxml2::XMLElement;
using tinyxml2::XMLError;
using tinyxml2::XML_SUCCESS;

using namespace Sim::MeshUtils;
using Sim::Vector;

namespace Sim {
	namespace Assets {

		bool Geometry::Initialize (XMLElement& element, Asset* asset)
		{
			// get asset name prefix
			const char* name = element.Attribute ("Prefix");
			if (name == nullptr){
				LOG_ERROR ("No asset geometry \'Prefix\' specified (mandatory)");
				return false;
			}

			// get asset location
			const char* location = element.Attribute ("Location");
			if (location == nullptr){
				LOG_ERROR ("No asset geometry \'Location\' specified (mandatory)");
				return false;
			}

			unsigned int depth = 0;
			XMLError error;
			if ((error = element.QueryUnsignedAttribute("Depth", &depth)) != XML_SUCCESS){
				LOG_ERROR ("Could not read partition depth");
				return false;
			}
			for (unsigned int i = 0; i < depth; ++i){
				_numSubsets *= 8;
			}

			// make a prefix from location and name (to be used subsequently to read files later)
			string prefix (location);
			if (prefix [prefix.size () - 1] != '/'){
				prefix += "/";
			}
			prefix += std::to_string (depth);
			prefix += "/";
			prefix += name;

			string file (prefix);
			file += ".node";

			if (!ReadVertexFile (file.c_str ())){
				LOG_ERROR ("Failed to read vertex file for Geometry component of " << name);
				Cleanup ();
				return false;
			}

			if (!ReadIndexFiles (prefix.c_str ())){
				LOG_ERROR ("Failed to read index files for Geometry component of " << name);
				Cleanup ();
				return false;
			}
			UpdateSurfaceVertexCount ();
			_offsetSize = SIM_VECTOR_SIZE * sizeof (Vector) * _numVertices;

			return true;
		}

		void Geometry::Update ()
		{
			// toggle between two indices
			_offsetIndex = toggle (_offsetIndex);
		}

		void Geometry::Cleanup ()
		{
			_vertices.reset ();
			_faces.reset ();
			_subsets.reset ();
		}

		bool Geometry::ReadVertexFile (const char* file)
		{
			_numVertices = MeshUtils::MeshFileElementCount (file);

			// allocate vertex array and load it from file
			_vertices = make_unique <Vector []> (2*_numVertices);
			if (!_vertices){
				LOG_ERROR ("Could not allocate vertex array of size " << 2*_numVertices << " for " << file);
				return false;
			}
			Vector* vptr = _vertices.get ();

			if (!MeshUtils::VectorLoad <SIM_VECTOR_SIZE> (file, vptr)){
				LOG_ERROR ("Could not read vertex file " << file);
				Cleanup ();
				return false;
			}
			// populate 2nd buffer
			unsigned int offset = _numVertices;
			for (unsigned int i = 0; i < _numVertices; ++i){
				vptr [offset + i] = vptr [i];
			}

			// update axis-aligned bounding box
			Vector min (vptr [0]);
			Vector max (min);
			for (unsigned int i = 1; i < _numVertices; ++i){
				for (unsigned int j = 0; j < 3; ++j){
					if (min [j] > vptr [i][j]){
						min [j] = vptr [i][j];
					}
				}
				for (unsigned int j = 0; j < 3; ++j){
					if (max [j] < vptr [i][j]){
						max [j] = vptr [i][j];
					}
				}
			}
			_bounds.Update (min, max);
			return true;
		}

		bool Geometry::ReadIndexFiles (const char* prefix)
		{
			_subsets = make_unique <Geometry::SpatialSubset []> (_numSubsets);

			// get total number of faces
			for (unsigned int i = 0; i < _numSubsets; ++i){
				string file (prefix);
				file += ".";
				file += std::to_string (i);
				file += ".tri";

				SpatialSubset* s = &(_subsets.get () [i]);
				s->_isize = MeshUtils::MeshFileElementCount (file.c_str ());
				s->_ioffset = 3*_numFaces;
				_numFaces += s->_isize;
			}

			// initialize face index array
			_faces = make_unique <unsigned int []> (3*_numFaces);

			for (unsigned int i = 0; i < _numSubsets; ++i){
				string file (prefix);
				file += ".";
				file += std::to_string (i);
				file += ".tri";

				SpatialSubset* s = &(_subsets [i]);
				unsigned int* f = &(_faces [s->_ioffset]);
				if (!MeshUtils::IndexLoad <3> (file.c_str (), f)){
					LOG_ERROR ("Could not load index file " <<  file);
					return false;
				}

				// calculate the offset of vertex
				unsigned int min = f [0];
				for (unsigned int j = 1; j < 3*s->_isize; ++j){
					if (min > f [j]){
						min = f [j];
					}
				}
				s->_voffset = min;

				s->UpdateBound (CurrentVertexBuffer (), f);
			}
			return true;
		}

		void Geometry::UpdateSurfaceVertexCount ()
		{
			unsigned int* f = _faces.get ();
			for (unsigned int i = 0; i < 3*_numFaces; ++i){
				if (_numSurfaceVertices < f [i]){
					_numSurfaceVertices = f [i];
				}
			}
			// this is done because at this point _numSurfaceVertices
			// has the index of the last surface vertex
			++_numSurfaceVertices;
		}

	}
}
