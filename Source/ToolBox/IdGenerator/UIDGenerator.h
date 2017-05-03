/**
 * @file UIDGenerator.h
 * @author Kishalay Kundu <kishalay.kundu@gmail.com>
 * @section LICENSE
 * @section LICENSE
 * See LICENSE.txt included in this package
 *
 * @section DESCRIPTION
 * A global unique identifier generator system.
 */
#pragma once

#include <random>
#include <mutex>

namespace Sim {

	class UIDGenerator {

		private:
			std::mutex _mutex;
			static UIDGenerator* _instance;

			UIDGenerator () // private constructor
			{

			}
			UIDGenerator (const UIDGenerator& g) {} // forbidden copy constructor
			UIDGenerator& operator = (const UIDGenerator& g) // forbidden assignment operator
			{
				return *this;
			}

		public:
			~UIDGenerator () {}
			static UIDGenerator& Instance ()
			{
				if (_instance) {
					return *_instance;
				} else {
					_instance = new UIDGenerator ();
				}
				return *_instance;
			}

			unsigned int GetUniqueId ();
	};
}
