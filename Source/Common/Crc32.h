/**
 * @file Crc32.h
 * @author Kishalay Kundu <kishalay.kundu@gmail.com>
 * @section LICENSE
 * See LICENSE.txt included in this package
 *
 * @section DESCRIPTION
 * A simple crc32 hash code calculating program implemented in
 * C++ format.
 */
#pragma once

namespace Sim {

	class Crc32 {

	private:
		unsigned int _table [256];

	public:
		Crc32 ();
		~Crc32 () = default;

		// forbidden copy and assignment operator
		Crc32 (const Crc32&) = delete;
		Crc32& operator = (const Crc32&) = delete;

		unsigned int operator () (const char* message);
	};
}
