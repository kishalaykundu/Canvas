/**
 * @file MeshUtils.h
 * @author Kishalay Kundu <kishalay.kundu@gmail.com>
 * @section LICENSE
 * See LICENSE.txt included in this package
 *
 * @section DESCRIPTION
 * Utility functions for reading meshes.
 */
#pragma once

#include <fstream>

#include "Preprocess.h"
#include "Log.h"
#include "Vector.h"

using std::ios;
using std::ifstream;

namespace Sim {
	namespace MeshUtils {

		auto MeshFileElementCount = [=] (const char* filename)
		{
			ifstream file (filename, ios::in);
			if (!file){
				LOG_ERROR ("Could not open " << filename);
				return 0;
			}
			int result = 0;
			file >> result;
			if (file.fail ()){
				LOG_ERROR ("Non-integer element count in " << filename);
				result = 0;
			}
			file.close ();
			return result;
		};

		// vertex buffer loading function (pre-allocated array)
		template <int size> bool VectorLoad (const char* filename, SimVector<size>* vertices)
		{
#			ifndef NDEBUG
			static_assert (size < 2 || size > 4, "Vector of minimum size 2 and maximum size 4 allowed");
			if (vertices == nullptr){
				LOG_ERROR ("Non-initialized vector array passed to VertexLoad for " << filename);
				return false;
			}
#			endif

			ifstream file (filename, ios::in);
			if (!file){
				LOG_ERROR ("Could not open " << filename);
				return false;
			}

			int count = 0;
			file >> count;
#			ifndef NDEBUG
			if (file.fail ()){
				LOG_ERROR ("Non-integer element count in " << filename);
				file.close ();
				return false;
			}
			if (count <= 0){
				LOG_ERROR ("Invalid element count " << count << " in " << filename);
				file.close ();
				return false;
			}
#			endif

			for (auto i = 0; i < count; ++i){
				file >> vertices [i][0] >> vertices [i][1];

				switch (size){
				case 3: case 4:
					file >> vertices [i][2]; break;
				}

#				ifndef NDEBUG
				if (file.bad ()){
					LOG_ERROR ("IO error occured when reading vector no. " << i << " in " << filename);
					file.close ();
					return false;
				}
				else if (file.eof()){
					LOG_ERROR ("End of file reached before " << count << " vectors could be read from " << filename);
					file.close ();
					return false;
				}
				else if (file.fail()){
					LOG_ERROR ("Non-real data read for vectors no. " << i << " in " << filename);
					file.close ();
					return false;
				}
#				endif

			}
			file.close ();

			LOG ("Loaded " << filename);
			return true;
		}

		template <int size> bool IndexLoad (const char* filename, unsigned int* indices)
		{
#			ifndef NDEBUG
			static_assert (size < 2 || size > 4, "Index of minimum size 2 and maximum size 4 allowed");
			if (vertices == nullptr){
				LOG_ERROR ("Non-initialized index array passed to IndexLoad for " << filename);
				return false;
			}
#			endif

			ifstream file (filename, ios::in);
			if (!file){
				LOG_ERROR ("Could not open " << filename);
				return false;
			}

			int count = 0;
			file >> count;
#			ifndef NDEBUG
			if (file.fail ()){
				LOG_ERROR ("Non-integer element count in " << filename);
				file.close ();
				return false;
			}
			if (count <= 0){
				LOG_ERROR ("Invalid element count " << count << " in " << filename);
				file.close ();
				return false;
			}
#			endif

			for (auto i = 0; i < count; ++i){
				unsigned int ind = size*i;
				file >> indices [ind] >> indices [ind + 1];

				switch (size){
				case 3:
					file >> indices [ind + 2];
					break;
				case 4:
					file >> indices [ind + 2] >> indices [ind + 3];
					break;
				}

#				ifndef NDEBUG
				if (file.bad ()){
					LOG_ERROR ("IO error occured when reading element no. " << i << " in " << filename);
					file.close ();
					return false;
				}
				else if (file.eof()){
					LOG_ERROR ("End of file reached before " << count << " elements could be read from " << filename);
					file.close ();
					return false;
				}
				else if (file.fail()){
					LOG_ERROR ("Non-integer data read for element no. " << i << " in " << filename);
					file.close ();
					return false;
				}
#				endif

			}
			file.close ();

			LOG ("Loaded " << filename);
			return true;
		}
	}
}
