/**
 * @file Mesh.h
 * @author Kishalay Kundu <kishalay.kundu@gmail.com>
 * @section LICENSE
 * See LICENSE.txt included in this package
 *
 * @section DESCRIPTION
 * Mesh geometry.
 */
#pragma once

#include <memory>
#include <mutex>
#include "Vector.h"

namespace Sim {

	class Mesh {

	protected:
		std::mutex _mutex;

		unsigned int _numVertices;
		std::shared_ptr <Vector> _current;

		unsigned int _numFaces;
		std::shared_ptr <unsigned int> _faces;

	public:
		Mesh ();
		~Mesh ();
		bool Initialize (const char* vertexFile, const char* indexFile);
		void Cleanup ();
	};
}
