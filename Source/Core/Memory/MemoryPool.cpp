/**
 * @file MemoryPool.cpp
 * @author Kishalay Kundu <kishalay.kundu@gmail.com>
 * @section LICENSE
 * See LICENSE.txt included in this package
 *
 * @section DESCRIPTION
 * See MemoryPool.h.
 */
#include <cstdlib>

#include "Preprocess.h"
#include "Memory/MemoryPool.h"

namespace Sim {
	const static size_t SIM_MEMORY_PAGE_HEADER_SIZE = (sizeof (unsigned char*));

	MemoryPool::~MemoryPool ()
	{
		Cleanup ();
	}

	// function to initialize the memory pool with specified page size and number of pages
	bool MemoryPool::Initialize (unsigned int pageSize, unsigned int numPages)
	{
		if (_rawArray != nullptr){
			LOG_WARNING ("Current memory pool is not empty. All allocated memory will be destroyed");
			Cleanup ();
		}
		_pageSize = pageSize;
		_numPages = numPages;

		return GrowArray ();
	}

	// completely destroy the memory pool
	void MemoryPool::Cleanup ()
	{
		for (unsigned int i = 0; i < _numArrays; ++i){
			free (_rawArray [i]);
		}
		free (_rawArray);
		_rawArray = nullptr;
	}

	// returns a pointer to a new page of memory
	void* MemoryPool::Allocate ()
	{
		// if we are out of memory, grow pool or return null if reallocation not allowed
		if (_head == nullptr){
			if (!_allowResize){
				return nullptr;
			}
			if (!GrowArray ()){
				return nullptr;
			}
		}

		unsigned char* current = _head;
		_head = GetNext (_head);
		return current + SIM_MEMORY_PAGE_HEADER_SIZE; // returns pointer to data section of page
	}

	// returns page to the pool
	void MemoryPool::Free (void* memoryPtr)
	{
		if (memoryPtr != nullptr){
			unsigned char* pagePtr = (unsigned char*)memoryPtr - SIM_MEMORY_PAGE_HEADER_SIZE;
			SetNext (pagePtr, _head);
			_head = pagePtr;
		}
	}

	// function to allocate a new memory array and add it to the pool (expensive operation)
	bool MemoryPool::GrowArray ()
	{
		// allocate and check if new allocation is successful
		unsigned char** newArray = (unsigned char**) malloc ((_numArrays + 1) * SIM_MEMORY_PAGE_HEADER_SIZE);
		if (newArray == nullptr){
			return false;
		}

		// copy existent memory pointers to newly allocated array
		for (unsigned int i = 0; i < _numArrays; ++i){
			newArray [i] = _rawArray [i];
		}
		// allocate new memory chunk
		newArray [_numArrays] = AllocateNewBlock ();

		// attach block to end of current memory list
		if (_head != nullptr){
			unsigned char* current = _head;
			unsigned char* next = GetNext (_head);
			while (next)
			{
				current = next;
				next = GetNext (next);
			}
			SetNext (current, newArray [_numArrays]);
		}
		else {
			_head = newArray [_numArrays];
		}

		// destroy any old allocated array
		if (_rawArray != nullptr){
			free (_rawArray);
		}

		// point internal array to newly allocated array
		_rawArray = newArray;
		++_numArrays;
		return true;
	}

	// allocates a new memory block
	unsigned char* MemoryPool::AllocateNewBlock ()
	{
		size_t blockSize = _pageSize + SIM_MEMORY_PAGE_HEADER_SIZE;
		size_t arraySize = _numPages * blockSize;
		unsigned char* newBlock = (unsigned char*) malloc (arraySize);
		if (newBlock == nullptr){
			return nullptr;
		}

		// turn raw memory block into singly linked list
		unsigned char* current = newBlock;
		unsigned char* end = current + arraySize;
		while (current < end){

			unsigned char* next = current + blockSize;
			unsigned char** pageHead = (unsigned char**) current;
			pageHead [0] = next < end ? next: nullptr;

			current += blockSize;
		}

		return newBlock;
	}

	unsigned char* MemoryPool::GetNext (unsigned char* block)
	{
		unsigned char** head = (unsigned char**)block;
		return head [0];
	}

	void MemoryPool::SetNext(unsigned char* blockToChange, unsigned char* newNext)
	{
		unsigned char** head = (unsigned char**)blockToChange;
		head [0] = newNext;
	}
}
