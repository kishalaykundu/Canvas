/**
 * @file Crc32.cpp
 * @author Kishalay Kundu <kishalay.kundu@gmail.com>
 * @section LICENSE
 * See LICENSE.txt included in this package
 *
 * @section DESCRIPTION
 * See Crc32.h.
 */

#include <cstdlib>
#include "Crc32.h"

namespace Sim {

	// constructor sets up the look up table
	Crc32::Crc32 ()
	{
		unsigned int result = 0, mask = 0;
    for (unsigned int i = 0; i < 256; ++i) {
       result = i;
       for (int j = 7; j >= 0; --j) {
          mask = -(result & 1);
          result = (result >> 1) ^ (0xEDB88320 & mask);
       }
       _table [i] = result;
    }
	}

	// returns the calculated hash
	unsigned int Crc32::operator () (const char* message)
	{
	   unsigned int i = 0, byte = 0, result = 0xFFFFFFFF;
	   while (message [i] != '\0') {
	  	 byte = message [i];
	  	 result = (result >> 8) ^ _table[(result ^ byte) & 0xFF];
	      i = i + 1;
	   }
	   return ~result;
	}
}
