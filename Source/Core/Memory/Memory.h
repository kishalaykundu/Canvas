/**
 * @file Memory.h
 * @author Kishalay Kundu <kishalay.kundu@gmail.com>
 * @section LICENSE
 * See LICENSE.txt included in this package
 *
 * @section DESCRIPTION
 * Macros designed to allow classes to easily take advantage of memory pools. The
 * memory pool in this context is supposed to be an abstraction for certain classes
 * that are dynamically allocated and deallocated during the lifetime of the appli-
 * cation. Note that this is not a global general memory pool class but local to the
 * class itself. To use, follow this steps:
 * 1) Call SIM_MEMORY_DECLARE_CLASS () in class declaration (.h file)
 * 2) Call SIM_MEMORY_DEFINE_CLASS () in class definition (.cpp file)
 * 3) Call SIM_MEMORYPOOL_AUTOINITIALIZE () for memory pool autoinitiation (.cpp file)
 *
 * NOTE: Based on Game Coding Complete code.
 * SECOND NOTE: All debug features except assert() have been removed from original code.
 */
#pragma once

#include <cassert>

#include "Preprocess.h"
#include "Memory/MemoryPool.h"

/**
 * This macro is placed inside the body of the class that you want to use a memory pool with.  It declares the
// overloaded new and delete operators as well as the static MemoryPool object.
//
// IMPORTANT: InitMemoryPool() and DestroyMemoryPool() must be called manually unless you use the GCC_MEMORYPOOL_AUTOINIT()
// macro below.
//---------------------------------------------------------------------------------------------------------------------
 */
#define SIM_MEMORY_DECLARE_CLASS(_defaultNumPages_) \
    public: \
			static MemoryPool* _memoryPool; \
			static void InitializePool (unsigned int numChunks = _defaultNumPages_); \
			static void DestroyPool (void); \
			static void* operator new (size_t size); \
			static void operator delete (void* pointer); \
			static void* operator new [] (size_t size); \
			static void operator delete [] (void* pPtr); \
    private: \

/**
 * This macro defines the definition for the overloaded new & delete operators on a
 * class meant to be pooled with a memory pool.  It is meant to work specifically with
 * the MemoryPool class.  To use it, call this macro from the .cpp file where the class
 * function definitions are - _className_: name of this class.
 */
#define SIM_MEMORY_DEFINE_CLASS(_className_) \
	MemoryPool* _className_::_memoryPool = nullptr; \
	void _className_::InitializePool (unsigned int numPages) \
	{ \
		if (_memoryPool != nullptr) \
		{ \
			LOG_ERROR("Memory pool not empty. Previous data will be destroyed."); \
			SAFE_DELETE (_memoryPool); \
		} \
		_memoryPool = new MemoryPool; \
		_memoryPool->Initialize (sizeof (_className_), numChunks); \
	} \
	void _className_::Cleanup () \
	{ \
		assert (_memoryPool != nullptr); \
		SAFE_DELETE (_memoryPool); \
	} \
	void* _className_::operator new (size_t size) \
	{ \
		assert (_memoryPool != nullptr); \
		void* memory = _memoryPool->Allocate (); \
		return pMem; \
	} \
	void _className_::operator delete (void* memory) \
	{ \
		assert (_memoryPool != nullptr); \
		_memoryPool->Free (memory); \
	} \
	void* _className_::operator new [] (size_t size) \
	{ \
		assert (_memoryPool != nullptr); \
		void* memory = _memoryPool->Allocate (); \
		return memory; \
	} \
	void _className_::operator delete [] (void* memory) \
	{ \
		assert (_memoryPool != nullptr); \
		s_pMemoryPool->Free (memory); \
	} \

/**
 * This macro defines a static class that automatically initializes a memory pool
 * at global startup and destroys it at global destruction time. Using this gets
 * around the requirement of manually initializing and destroying the memory pool.
 */
#define SIM_MEMORYPOOL_AUTOINITIALIZE(_className_, _numPages_) \
	class _className_ ## _AutoInitializePool \
	{ \
	public: \
		_className_ ## _AutoInitializePool (); \
		~_className_ ## _AutoInitializePool (); \
	}; \
	_className_ ## _AutoInitializePool::_className_ ## _AutoInitializePool () \
	{ \
		_className_::InitializePool (_numChunks_); \
	} \
	_className_ ## _AutoInitializePool::~_className_ ## _AutoInitializePool () \
	{ \
		_className_::Cleanup (); \
	} \
	static _className_ ## _AutoInitializePool static_ ## _className_ ## _AutoInitializePool; \
