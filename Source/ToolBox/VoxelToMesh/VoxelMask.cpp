/**
 * @file VoxelMask.cpp
 * @author Kishalay Kundu <kishalay.kundu@gmail.com>
 * @section LICENSE
 * See LICENSE.txt included in this package
 *
 * @section DESCRIPTION
 * See VoxelMask.h.
 */

#include "Log.h"
#include "VoxelUtils.h"
#include "VoxelMask.h"

namespace Sim {

	bool VoxelMask::LoadFile (const char* filename)
	{
		if (VoxelLoad <unsigned char> (filename, _data)){
			LOG_ERROR ("Could not load " << filename);
			_data.clear ();
			return false;
		}
		return true;
	}

	unsigned int VoxelMask::ConnectedComponents ()
	{
		unsigned int result = 0;

		return result;
	}

	void VoxelMask::Neighbor (unsigned int a, unsigned int b, unsigned int c, unsigned char*& d)
	{
		int x = static_cast <int> (a);
		int y = static_cast <int> (b);
		int z = static_cast <int> (c);
		for (auto i = 0; i < 27; ++i){
			d [i] = 0x00;
		}

		int index = 0;

		for (int i = -1; i <= 1; ++i){
			for (int j = -1; j <= 1; ++j){
				for (int k = -1; k <= 1; ++k){
					if ((x + k) >= 0 && (x + k) < _width &&
							(y + j) >= 0 && (y + j) < _height &&
							(z + i) >= 0 && (z + i) < _depth){
						d [(i + 1)*9 + (j + 1)*3 + k + 1] = _data [((z + i)*_height + y + j)*_width + x + k];
					}
				}
			}
		}
	}
}
