/**
 * @file MemoryPool.h
 * @author Kishalay Kundu <kishalay.kundu@gmail.com>
 * @section LICENSE
 * See LICENSE.txt included in this package
 *
 * @section DESCRIPTION
 * The raw memory pool class for the Chimera system. A memory pool is
 * a pool of memory that's split into pages of equal size, each with a
 * 4-byte header. The header is treated as a pointer that points to the
 * next page, making the pool a singly-linked list of memory pages.
 * When the pool is first initialized via the Initialize () function, it
 * must be passed the page-size and the number of pages to be created.
 * These two values are immutable until the pool is destroyed and reini-
 * tialized. The page-size is the size of each page, minus the header,
 * in bytes.  The memory pool allocates the appropriate amount of memory
 * and sets up the data structure during initialization. Total memory usage
 * is be (NP*(PS + 4)+Ov), where NP is the number of pages, PS is the size
 * of each page, and Ov is the overhead for the class (currently 18 + (number
 * of reallocations * 4).
 * Note: Adapted from Game Coding Complete code.
 */
#pragma once

namespace Sim {

	class MemoryPool {

		private:
			unsigned char** _rawArray;  // array of memory blocks, each split up into pages and connected
			unsigned char* _head;  // the front of the memory chunk linked list
			unsigned int _pageSize; // size of single page in bytes
			unsigned int _numPages;  // number of pages in array
			unsigned int _numArrays;  // number elements in the memory array
			bool _allowResize;  // true if we resize the memory pool when it fills up (expensive operation)

		public:
			MemoryPool ()
			: _rawArray (nullptr), _head (nullptr), _pageSize (0),
				_numPages (0), _numArrays (0), _allowResize (true) {}
			~MemoryPool ();

		private: // non-allowed copy constructor and assignment operator
			MemoryPool (const MemoryPool& m)
			: _rawArray (m._rawArray), _head (m._head), _pageSize (m._pageSize),
				_numPages (m._numPages), _numArrays (m._numArrays), _allowResize (m._allowResize)
			{}
			MemoryPool& operator = (const MemoryPool& m) {return *this;}

		public:
			bool Initialize (unsigned int pageSize, unsigned int numPages);
			void Cleanup ();

			/**
			 * Function to retrieve a page from the memory pool. This removes the head of
			 * the linked list, sets the new head to the next page, and returns a pointer
			 * to the data section of the old head.  If there are no more pages left and
			 * resize is allowed, then another block of N pages is allocated, where N is
			 * the number of pages passed into Initialize (). While this is typically a
			 * very fast function, reallocation is costly, so initial sizes should be chosen
			 * carefully.
			 */
			void* Allocate ();
			/**
			 * Function to release a page of memory back into the memory pool for reuse. This
			 * causes the page to be inserted to the front of the list, ready for the next bit.
			 */
			void Free (void *memoryPointer);

			unsigned int PageSize () const {return _pageSize;}
			void AllowResize (bool flag) {_allowResize = flag;}

		private:
			void Reset (); // resets internal variables

			// internal memory allocation helpers
			bool GrowArray ();
			unsigned char* AllocateNewBlock ();

			// internal linked list management
			unsigned char* GetNext (unsigned char* block);
			void SetNext (unsigned char* blockToChange, unsigned char* newNext);
	};
}
