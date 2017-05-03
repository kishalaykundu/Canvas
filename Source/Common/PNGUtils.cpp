/**
 * @file PNGUtils.cpp
 * @author Kishalay Kundu <kishalay.kundu@gmail.com>
 * @section LICENSE
 * See LICENSE.txt included in this package
 *
 * @section DESCRIPTION
 * See PNGUtils.h.
 */
#include <vector>

#include "Log.h"
#include "PNGUtils.h"

using std::vector;

namespace Sim {

	bool PNGEncoder::operator () (vector <unsigned char>& data)
	{
		// safety check
		if (data.empty ()){
			LOG_ERROR ("Input data to write to " << _file << " is empty");
			return false;
		}

		// encode
		auto error = lodepng::encode (_file, data, _width, _height);
		if (error){
			LOG_ERROR ("Could not write to " << _file << ". Error no. " << error << ": " << lodepng_error_text (error));
			data.clear ();
			return false;
		}
		data.clear ();

		return true;
	}

	bool PNGDecoder::operator () (vector <unsigned char>& data)
	{
		// purge any existing data
		if (!data.empty ()){
			data.clear ();
		}

		// decode
		auto error = lodepng::decode (data, _width, _height, _file);
		if (error){
			LOG_ERROR ("Could not load " << _file << ". Error no. " << error << ": " << lodepng_error_text (error));
			data.clear ();
			return false;
		}

		return true;
	}
}
