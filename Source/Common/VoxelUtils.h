/**
 * @file VoxelUtils.h
 * @author Kishalay Kundu <kishalay.kundu@gmail.com>
 * @section LICENSE
 * See LICENSE.txt included in this package
 *
 * @section DESCRIPTION
 * Utility functions for processing voxel-datasets.
 */
#pragma once

#include <fstream>

#include "Log.h"

using std::ios;
using std::ifstream;

namespace Sim {

	template <typename T> bool VoxelLoad (const char* filename, std::vector <T>& data)
	{
		if (data.empty ()){
			LOG_ERROR ("Input data array is not initialized");
			return false;
		}

		ifstream file (filename, ios::in | ios::binary);
		if (!file){
			LOG_ERROR ("Could not open " << filename);
			return false;
		}

		if (!file.read (reinterpret_cast <char*> (&(data [0])), sizeof (T)*data.size ())){
			LOG_ERROR ("Could not successfully read " << filename << ". Read " << file.gcount () << " bytes");
			return false;
		}
		return true;
	}

}
