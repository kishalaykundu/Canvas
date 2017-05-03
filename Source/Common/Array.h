/**
 * @file Array.h
 * @author Kishalay Kundu <kishalay.kundu@gmail.com>
 * @section LICENSE
 * See LICENSE.txt included in this package
 *
 * @section DESCRIPTION
 * A templetized wrapper around the C++11 array container.
 */
#pragma once

#include <array>

#include "Log.h"

using std::array;
using std::size_t;

namespace Sim {

	template <class T, std::size_t max> class Array : public array <T, max> {

		private:
			size_t _count = 0;

		public:
			Array () = default;
			~Array () = default;
			Array (const Array& arr) = default;
			Array& operator = (const Array& arr) = default;

			T& operator [] (size_t index)
			{
#				ifndef NDEBUG
				if (index >= _count){
					LOG_WARNING ("Returning bogus element. Please use push_back to add new elements");
					return array <T, max>::operator [] (0);
				}
#				endif
				return array <T, max>::operator [] (index);
			}

			constexpr size_t size () const {return _count;}

			inline void PushBack (const T& e)
			{
#				ifndef NDEBUG
				if (_count < max) {
#				endif
					array <T, max>::operator [] (_count) = e;
					++_count;
#				ifndef NDEBUG
				}
				else {
					LOG_ERROR ("Array is already full. Could not add more elements");
				}
#				endif
			}

			inline void Erase (size_t index)
			{
#				ifndef NDEBUG
				if (index >= _count){
					LOG_ERROR ("Out of bounds erase index specified for Array element");
					return;
				}
#				endif
				LOG_WARNING ("If Array element is of raw pointer type, please delete corresponding object before calling erase");
				for (auto i = index; i < _count - 1; ++i){
					array <T, max>::operator [] (i) = array <T, max>::operator [] (i + 1);
				}
				--_count;
			}
	};

}
