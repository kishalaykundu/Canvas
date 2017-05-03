/**
 * @file CircularQueue.h
 * @author Kishalay Kundu <kishalay.kundu@gmail.com>
 * @section LICENSE
 * See LICENSE.txt included in this package
 *
 * @section DESCRIPTION
 * Template for thread-safe circular queue.
 */
#pragma once

#include <mutex>
#include <condition_variable>

namespace Sim {

	template <class T, unsigned int size> class CircularQueue {

		private:
			std::mutex _mutex;
			std::condition_variable _full, _empty;

			unsigned int _readIndex = 0;
			unsigned int _writeIndex = 0;
			T _queue [size];

		public:
			CircularQueue () = default;
			~CircularQueue () = default;
			void Push (const T& e)
			{
				std::unique_lock <std::mutex> loki (_mutex);
				while (Full ()) {
					_full.wait (loki);
				}

				_queue [Index (_writeIndex)] = e;
				_writeIndex = Index (_writeIndex + 1);

				_empty.notify_one ();
			}

			void Pop (T& result)
			{
				std::unique_lock <std::mutex> loki (_mutex);
				while (Empty ()) {
					_empty.wait (loki);
				}

				result = _queue [Index (_readIndex)];
				_readIndex = Index (_readIndex + 1);

				_full.notify_one ();
			}

		protected:
			bool Full ()
			{
				return (_writeIndex -_readIndex == size - 1) || (_writeIndex == _readIndex - 1);
			}
			bool Empty ()
			{
				return (_writeIndex == _readIndex);
			}
			unsigned int Index (const unsigned int index)
			{
				return index % size;
			}
	};
}
