/**
 * @file VMProcessor.h
 * @author Kishalay Kundu <kishalay.kundu@gmail.com>
 * @section LICENSE
 * See LICENSE.txt included in this package
 *
 * @section DESCRIPTION
 * The voxel processor class for VoxelMan. This reads all the voxel data
 * and converts them to datasets for individual organs.
 */
#pragma once

#include <vector>
#include <string>

namespace Sim {

	class VMProcessor {

	protected:
		bool _rgbOut = false;
		unsigned int _width = 0, _height = 0, _from = 0, _to = 0;
		unsigned int _wBounds [2] = {0, 0};
		unsigned int _hBounds [2] = {0, 0};
		unsigned int _dBounds [2] = {0, 0};

		const unsigned int _offset = 2;
		unsigned int _vWidth = 0, _vHeight = 0, _vDepth = 0;
		std::vector < std::vector <unsigned char> > _mask, _rgb, _ct;

		// input related strings
		std::string _ctfolder, _maskfolder, _rgbfolder;
		std::string _ctprefix, _maskprefix, _rgbprefix;

		// output related strings
		std::string _outfolder;
		std::string _prefix;

	public:
		VMProcessor () = delete;
		VMProcessor (const VMProcessor&) = delete;
		VMProcessor& operator = (const VMProcessor&) = delete;

		VMProcessor (std::string& ctf, std::string& maskf, std::string& rgbf, std::string& ct, std::string& mask, std::string& rgb,
				std::string& outloc, std::string& prefix)
		: _ctfolder (ctf), _maskfolder (maskf), _rgbfolder (rgbf), _ctprefix (ct), _maskprefix (mask), _rgbprefix (rgb),
			_outfolder (outloc), _prefix (prefix) {}
		~VMProcessor () = default;

		unsigned int& Width () {return _width;}
		unsigned int& Height () {return _height;}
		unsigned int& From () {return _from;}
		unsigned int& To () {return _to;}
		bool& WriteRgb () {return _rgbOut;}

		bool Process (unsigned int nlabels, unsigned int* labels);

	private:
		void Cleanup ();
		bool CheckSanity ();

		bool ProcessMask (unsigned int nlabels, unsigned int* labels);
		bool ProcessRGB (unsigned int nlabels, unsigned int* labels);
		bool ProcessCT (unsigned int nlabels, unsigned int* labels);

		bool ConnectedComponents ();

		bool ReadPngFile (const char* file, std::vector <unsigned char>& data);
		bool ReadTiffFile (const char* file, std::vector <unsigned short>& data);
		bool WriteTexFile (const char* suffix, std::vector <std::vector <unsigned char> >& data);

		bool WritePngStack ();

		bool WriteInfoFile ();
	};
}
