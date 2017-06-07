/**
 * @file VMProcessor.cpp
 * @author Kishalay Kundu <kishalay.kundu@gmail.com>
 * @section LICENSE
 * See LICENSE.txt included in this package
 *
 * @section DESCRIPTION
 * See VMProcessor.h.
 */
extern "C"{
#	include <sys/stat.h>
#	include <sys/types.h>
}

#include <string>
#include <vector>
#include <fstream>
#include <iostream>

#include "tinyxml2.h"
#include "lodepng.h"
#include "tinytiffreader.h"

#include "Log.h"
#include "PNGUtils.h"
#include "VMProcessor.h"

using std::ofstream;
using std::ios;
using std::vector;
using std::string;
using tinyxml2::XMLElement;

namespace Sim {

	bool VMProcessor::Process (unsigned int nlabels, unsigned int* labels)
	{
		if (!CheckSanity ()){
			LOG_ERROR ("All initialization parameters not set");
			return false;
		}

		// extract the smallest possible volume (padded by 2 on each side) from the larger dataset
		if (!ProcessMask (nlabels, labels)){
			LOG_ERROR ("Labels could not be successfully processed");
			Cleanup ();
			return false;
		}

		LOG (_prefix << ": Mask successfully processed");
		LOG ("Width - " << _vWidth << ". Bounds: " << _wBounds [0] << " - " << _wBounds [1]);
		LOG ("Height - " << _vHeight << ". Bounds: " << _hBounds [0] << " - " << _hBounds [1]);
		LOG ("Depth - " << _vDepth << ". Bounds: " << _dBounds [0] << " - " << _dBounds [1]);

		// take out disconnected components from segmented values
		if (!ConnectedComponents ()){
			LOG_ERROR ("Could not extract connected components");
			return false;
		}

		return true;

		if (!ProcessRGB (nlabels, labels)){
			LOG_ERROR ("RGB data could not be successfully processed");
			Cleanup ();
			return false;
		}

		if (_rgbOut){
			if (!WritePngStack ()){
				LOG_ERROR ("Could not write rgb data to PNG files");
			}
		}

		// write out texture file and deallocate data
		WriteTexFile (".rgba.tex3", _rgb);
		for (unsigned int i = 0; i < _rgb.size (); ++i){
			_rgb [i].clear ();
		}
		_rgb.clear ();

		if (!ProcessCT (nlabels, labels)){
			LOG_ERROR ("CT data could not be successfully processed");
			Cleanup ();
			return false;
		}

		WriteTexFile (".ct.tex3", _ct);
		for (unsigned int i = 0; i < _ct.size (); ++i){
			_ct [i].clear ();
		}
		_ct.clear ();

		// write out the mask volume
		WriteTexFile (".mask.tex3", _mask);
		for (unsigned int i = 0; i < _mask.size (); ++i){
			_mask [i].clear ();
		}
		_mask.clear ();

		// finally write out the size and offset info for the subset volume
		WriteInfoFile ();

		return true;
	}

	// checks whether input parameters make sense
	void VMProcessor::Cleanup ()
	{
		for (unsigned int i = 0; i < _ct.size (); ++i){
			_ct [i].clear ();
		}
		_ct.clear ();
		for (unsigned int i = 0; i < _rgb.size (); ++i){
			_rgb [i].clear ();
		}
		_rgb.clear ();
		for (unsigned int i = 0; i < _mask.size (); ++i){
			_mask [i].clear ();
		}
		_mask.clear ();
	}

	bool VMProcessor::CheckSanity ()
	{
		if (!_width || !_height){
			return false;
		}
		if (_from > _to){
			return false;
		}
		return !(_ctfolder.empty () || _maskfolder.empty () || _rgbfolder.empty () ||
				_ctprefix.empty () || _maskprefix.empty () || _rgbprefix.empty () || _outfolder.empty () || _prefix.empty ());
	}

	bool VMProcessor::ProcessMask (unsigned int nlabels, unsigned int* labels)
	{
		// process label files (in TIFF format)
		vector < vector <unsigned short> > mask (_to - _from + 1);
		for (auto i = _from; i <= _to; ++i){

			string file (_maskfolder + _maskprefix);
			file += std::to_string (i);
			file += ".tif";

			if (!ReadTiffFile (file.c_str (), mask [i - _from])){
				LOG_ERROR ("Could not read " << file);
				for (unsigned int j = 0; j < mask.size (); ++j){
					mask [j].clear ();
				}
				mask.clear ();
				return false;
			}
    }

		// find out the subset dimensions for all the labels
		_wBounds [0] = _width;
		_hBounds [0] = _height;
		_dBounds [0] = _to - _from + 1;

		for (unsigned int x = 0; x < nlabels; ++x){

			unsigned short label = static_cast <unsigned short> (labels [x]);

			for (unsigned int i = _from; i <= _to; ++i){
				bool sliceFlag = false;
				unsigned int ind = i - _from;
				unsigned short* image = &(mask [ind][0]);

				for (unsigned int j = 0; j < _height; ++j){
					for (unsigned int k = 0; k < _width; ++k){

						if (label == image [j*_width + k]){
							sliceFlag = true;
							_wBounds [0] = _wBounds [0] > k ? k : _wBounds [0];
							_wBounds [1] = _wBounds [1] < k ? k : _wBounds [1];
							_hBounds [0] = _hBounds [0] > j ? j : _hBounds [0];
							_hBounds [1] = _hBounds [1] < j ? j : _hBounds [1];
						}
					}
				}
				if (sliceFlag){
					_dBounds [0] = _dBounds [0] > ind ? ind : _dBounds [0];
					_dBounds [1] = _dBounds [1] < ind ? ind : _dBounds [1];
				}
			}
		}

		// form even-sized rectangular volume with at least 2 blank voxel padding
		// layers on each side (padding maybe 3 at one end in case of odd dimension)
		_vWidth = _wBounds [1] - _wBounds [0] + 1;
		_vHeight = _hBounds [1] - _hBounds [0] + 1;
		_vDepth = _dBounds [1] - _dBounds [0] + 1;

		auto isEven = [=] (unsigned int a) { return (a % 2) == 0; };
		_vWidth = isEven (_vWidth) ? _vWidth + 4 : _vWidth + 5;
		_vHeight = isEven (_vHeight) ? _vHeight + 4 : _vHeight + 5;
		_vDepth = isEven (_vDepth) ? _vDepth + 4 : _vDepth + 5;

		// initialize and populate the mask layer (used later
		_mask = vector < vector <unsigned char> > (_vDepth);
		for (unsigned int i = 0; i < _vDepth; ++i){
			_mask [i] = vector <unsigned char> (_vWidth * _vHeight, 0x00);
		}

		// form the label mask volume
		for (unsigned int x = 0; x < nlabels; ++x){

			unsigned short label = static_cast <unsigned short> (labels [x]);
			for (unsigned int i = _dBounds [0]; i <= _dBounds [1]; ++i){

				unsigned char* cimage = &(_mask [i - _dBounds [0]  + _offset][0]);
				unsigned short* image = &(mask [i][0]);

				for (unsigned int j = _hBounds [0]; j <= _hBounds [1]; ++j){
					for (unsigned int k = _wBounds [0]; k <= _wBounds [1]; ++k){

						if (label == image [j*_width + k]){
							cimage [(j - _hBounds [0] + _offset) * _vWidth + k - _wBounds [0] + _offset] = 0x0f;
						}
					}
				}
			}
		}

		// clear original mask volume - we don't need it anymore
		for (unsigned int i = 0; i < mask.size (); ++i){
			mask [i].clear ();
		}
		mask.clear ();

		return true;
	}

	bool VMProcessor::ProcessRGB (unsigned int nlabels, unsigned int* labels)
	{
		// read PNG rgb data (only subset is read from info gained from mask processing)
		vector < vector <unsigned char> > rgb (_vDepth);
		for (auto i = _dBounds [0] + _from; i <= _dBounds [1] + _from; ++i){

			string file (_rgbfolder + _rgbprefix);
			file += std::to_string (i);
			file += ".png";

			if (!ReadPngFile (file.c_str (), rgb [i - _dBounds [0] - _from + _offset])){
				LOG_ERROR ("Could not read " << file);
				for (unsigned int j = 0; j < rgb.size (); ++j){
					rgb [j].clear ();
				}
				rgb.clear ();
				return false;
			}
    }

		// initialize output rgb volume
		_rgb = vector < vector <unsigned char> > (_vDepth);
		for (unsigned int i = 0; i < _vDepth; ++i){
			_rgb [i] = vector <unsigned char> (4 * _vWidth * _vHeight, 0x00);
		}


		for (unsigned int i = _offset; i <= _dBounds [1] - _dBounds [0] + _offset; ++i){

			unsigned char* msk = &(_mask [i][0]);
			unsigned char* col = &(rgb [i][0]);
			unsigned char* cvol = &(_rgb [i][0]);
			unsigned ind = 0;

			for (unsigned int j = _hBounds [0]; j <= _hBounds [1]; ++j){
				for (unsigned int k = _wBounds [0]; k <= _wBounds [1]; ++k){

					ind = (j - _hBounds [0]) * _vWidth + k - _wBounds [0];

					if (msk [ind]){
						for (unsigned int a = 0; a < 3; ++a){
							cvol [4 * ind + a] = col [4 * (j * _width + k) + a];
						}
						cvol [4 * ind + 3] = 0xff;
					}
				}
			}
		}

		for (unsigned int i = 0; i < rgb.size (); ++i){
			rgb [i].clear ();
		}
		rgb.clear ();

		return true;
	}

	bool VMProcessor::ProcessCT (unsigned int nlabels, unsigned int* labels)
	{
		// read PNG CT data (only subset is read from info gained from mask processing)
		vector < vector <unsigned char> > ct (_vDepth);
		for (auto i = _dBounds [0] + _from; i <= _dBounds [1] + _from; ++i){

			string file (_ctfolder + _ctprefix);
			file += std::to_string (i);
			file += ".png";

			if (!ReadPngFile (file.c_str (), ct [i - _dBounds [0] - _from + _offset])){
				LOG_ERROR ("Could not read " << file);
				for (unsigned int j = 0; j < ct.size (); ++j){
					ct [j].clear ();
				}
				ct.clear ();
				return false;
			}
    }

		_ct = vector < vector <unsigned char> > (_vDepth);
		for (unsigned int i = 0; i < _vDepth; ++i){
			_ct [i] = vector <unsigned char> (4 * _vWidth * _vHeight, 0x00);
		}

		for (unsigned int i = _offset; i <= _dBounds [1] - _dBounds [0] + _offset; ++i){
			unsigned char* msk = &(_mask [i][0]);
			unsigned char* gray = &(ct [i][0]);
			unsigned char* gvol = &(_ct [i][0]);
			unsigned ind = 0;

			for (unsigned int j = _hBounds [0]; j <= _hBounds [1]; ++j){
				for (unsigned int k = _wBounds [0]; k <= _wBounds [1]; ++k){

					ind = (j - _hBounds [0]) * _vWidth + k - _wBounds [0];

					if (msk [ind]){
						for (unsigned int a = 0; a < 3; ++a){
							gvol [4 * ind + a] = gray [4 * (j * _width + k) + a];
						}
						gvol [4*ind + 3] = 0xff;
					}
				}
			}
		}

		for (unsigned int i = 0; i < ct.size (); ++i){
			ct [i].clear ();
		}
		ct.clear ();

		return true;
	}

	bool VMProcessor::ConnectedComponents ()
	{
		vector <vector <bool> > visited (_mask.size ());

		// make all 0-valued voxels as visited
		unsigned int count = 0;
		bool found = false;
		unsigned int first [3] = {0, 0, 0};

		for (unsigned int i = 0; i < _mask.size (); ++i){
			visited [i] = vector <bool> (_mask [i].size (), false);

			for (unsigned int j = 0; j < _vHeight; ++j){
				for (unsigned int k = 0; k < _vWidth; ++k){

					if (!_mask [i][j*_vWidth + k]){
						visited [i][j*_vWidth + k] = true;
					}
					else {
						if (!found){
							found = true;
							first [0] = k; first [1] = j; first [2] = i;
						}
						++count;
					}

				}
			}
		}
		LOG (count << " non-zero voxels found out of a total of " << _mask.size ()*_mask [0].size ());

		// do a breadth-first traversal to find connected components
		int indices [27];
		for (unsigned int i = 0; i < 27; ++i){
			indices [i] = -1;
		}


		return true;
	}

	bool VMProcessor::ReadPngFile (const char* file, vector <unsigned char>& data)
	{
		PNGDecoder dec (file, _width, _height);
		if (!dec (data)){
			return false;
		}
		return true;
	}

	bool VMProcessor::ReadTiffFile (const char* file, vector <unsigned short>& data)
	{
		if (!data.empty ()){
			data.clear ();
		}

		TinyTIFFReaderFile* reader = nullptr;
		reader = TinyTIFFReader_open (file);
		if (reader == nullptr) {
			LOG ("TIFF READER ERROR: Could not read " << file << " (not existent, not accessible or not a TIFF file)");
			return false;
		}
    if (TinyTIFFReader_wasError (reader)){
    	LOG ("TIFF READER ERROR:" << TinyTIFFReader_getLastError (reader));
      LOG ("TIFF File ImageDescription:" << TinyTIFFReader_getImageDescription (reader));
      return false;
    }

    // get image dimensions from file
		unsigned int width = TinyTIFFReader_getWidth (reader);
		unsigned int height = TinyTIFFReader_getHeight (reader);
		if (width != _width || height != _height){
			LOG ("TIFF READER ERROR: " << file << " dimensions - [" << width << "x" << height <<
					"]. Input dimensions - [" << _width << "x" << _height << "]");
			return false;
		}

		data = vector <unsigned short> (width * height);
		TinyTIFFReader_getSampleData (reader, &(data [0]), 0);
		TinyTIFFReader_close (reader);
		return true;
	}

	bool VMProcessor::WriteTexFile (const char* suffix, vector <vector <unsigned char> >& data)
	{
		string filename (_outfolder + _prefix);
		filename += suffix;

		ofstream file (filename, ios::out | ios::trunc);
		if (!file.is_open ()){
			LOG_ERROR ("Could not open " << filename);
			return false;
		}
		for (unsigned int i = 0; i < data.size (); ++i){
			file.write (reinterpret_cast <char*>(&(data [i])), data [i].size ());
		}

		file.close ();
		return true;
	}

	// write stack of PNG format RGB data
	bool VMProcessor::WritePngStack ()
	{
		string prefix (_outfolder);
		prefix += "PngRgb/";

		// check and create directory
		mkdir (prefix.c_str(), 0755);

		prefix += "rgb";
		for (unsigned int i = 0; i < _vDepth; ++i){
			string file (prefix);
			file += std::to_string (i);
			file += ".png";

			PNGEncoder enc (file.c_str (), _vWidth, _vHeight);
			if (!enc (_rgb [i])){
				LOG_ERROR ("Could not write to " << file);
				return false;
			}
		}

		return true;
	}

	// write out XML-style info file
	bool VMProcessor::WriteInfoFile ()
	{
		tinyxml2::XMLDocument doc;
		tinyxml2::XMLNode* root = doc.NewElement ("VMInfo");
		doc.InsertFirstChild (root);

		XMLElement* location = doc.NewElement ("Location");
		location->SetText (_outfolder.c_str ());

		root->InsertFirstChild (location);

		XMLElement *mask = doc.NewElement ("Mask");
		string maskfile (_prefix);
		maskfile += ".mask.tex3";
		mask->SetText (maskfile.c_str ());

		root->InsertEndChild (mask);

		XMLElement *rgb = doc.NewElement ("RGB");
		string rgbfile (_prefix);
		rgbfile += ".rgb.tex3";
		rgb->SetText (rgbfile.c_str ());

		root->InsertEndChild (rgb);

		XMLElement *ct = doc.NewElement ("CT");
		string ctfile (_prefix);
		ctfile += ".ct.tex3";
		ct->SetText (ctfile.c_str ());

		root->InsertEndChild (ct);

		XMLElement* dims = doc.NewElement ("Dimensions");
		dims->SetAttribute ("Width", _vWidth);
		dims->SetAttribute ("Height", _vHeight);
		dims->SetAttribute ("Depth", _vDepth);

		root->InsertEndChild (dims);

		XMLElement* offset = doc.NewElement ("Offset");
		int o = static_cast <int> (_offset);
		int x = static_cast <int> (_wBounds [0]) - o;
		int y = static_cast <int> (_hBounds [0]) - o;
		int z = static_cast <int> (_dBounds [0] + _from) - o;
		offset->SetAttribute ("X", x);
		offset->SetAttribute ("Y", y);
		offset->SetAttribute ("Z", z);
		root->InsertEndChild (offset);

		string file (_outfolder + _prefix);
		file += ".info";

		tinyxml2::XMLError error = doc.SaveFile (file.c_str ());
		if (error != tinyxml2::XML_SUCCESS){
			LOG_ERROR ("Could not write to " << file);
			return false;
		}

		return true;
	}
}
