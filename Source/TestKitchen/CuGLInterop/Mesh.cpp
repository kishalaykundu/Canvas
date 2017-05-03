/**
 * @file Mesh.cpp
 * @author Kishalay Kundu <kishalay.kundu@gmail.com>
 * @section LICENSE
 * See LICENSE.txt included in this package
 *
 * @section DESCRIPTION
 * Mesh geometry.
 */

#include <memory>

#include "Preprocess.h"

#include "MeshLoader.h"
#include "Vector.h"
#include "Mesh.h"

using std::shared_ptr;

namespace Sim {

	Mesh::Mesh ()
	: _numVertices (0), _numFaces (0)
	{ }

	Mesh::~Mesh ()
	{
		Cleanup ();
	}

	bool Mesh::Initialize (const char* vf, const char* ff)
	{
		_numVertices = MeshLoader::GetElementCount (vf);
		_current = shared_ptr <Vector> (new Vector [_numVertices], DeleteArray <Vector> ());
		if (!MeshLoader::LoadVertices <SIM_VECTOR_SIZE> (vf, _current. get())){
			Cleanup ();
			return false;
		}
		_numFaces = MeshLoader::GetElementCount (ff);
		_faces = shared_ptr <unsigned int> (new unsigned int [3*_numFaces], DeleteArray <unsigned int> ());
		if (!MeshLoader::LoadIndices <3> (ff, _faces.get ())){
			Cleanup ();
			return false;
		}
		return true;
	}

	void Mesh::Cleanup ()
	{
		_current.reset ();
		_faces.reset ();
	}
}
