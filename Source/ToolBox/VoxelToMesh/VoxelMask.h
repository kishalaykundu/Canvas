/**
 * @file VoxelMask.h
 * @author Kishalay Kundu <kishalay.kundu@gmail.com>
 * @section LICENSE
 * See LICENSE.txt included in this package
 *
 * @section DESCRIPTION
 * The voxelized mask class that uploads a 3D 1-byte mask dataset
 * and provides various utilities for it.
 */
#pragma once

#include <vector>

namespace Sim {

	class VoxelMask {

	protected:
		unsigned int _width = 0, _height = 0, _depth = 0;
		std::vector <unsigned char> _data;

	public:
		VoxelMask () = delete;
		~VoxelMask ();

		VoxelMask (const VoxelMask&) = delete;
		VoxelMask& operator = (const VoxelMask&) = delete;

		VoxelMask (unsigned int width, unsigned int height, unsigned int depth)
		: _width (width), _height (height), _depth (depth), _data (width*height*depth, 0x00) {}

		bool LoadFile (const char* filename);
		unsigned int ConnectedComponents ();

	protected:
		void Neighbor (unsigned int x, unsigned int y, unsigned int z, unsigned char*& result);
	};
}
