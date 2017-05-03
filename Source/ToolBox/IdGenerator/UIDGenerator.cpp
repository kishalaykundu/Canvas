/**
 * @file UIDGenerator.cpp
 * @author Kishalay Kundu <kishalay.kundu@gmail.com>
 * @section LICENSE
 * See LICENSE.txt included in this package
 *
 * @section DESCRIPTION
 * See UIDGenerator.h.
 */

#include <climits>
#include <mutex>

#include "UIDGenerator.h"

using std::mutex;

namespace Sim {

	UIDGenerator* UIDGenerator::_instance = nullptr;

	static std::random_device _dev;
	static std::mt19937 _mers (_dev ());
	static std::uniform_int_distribution <int> _dist (1, USHRT_MAX);

	unsigned int UIDGenerator::GetUniqueId ()
	{
		_mutex.lock ();
		return _dist (_mers);
		_mutex.unlock ();
	}
}
