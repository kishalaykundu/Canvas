/**
 * @file Preprocess.h
 * @author Kishalay Kundu <kishalay.kundu@gmail.com>
 * @section LICENSE
 * See LICENSE.txt included in this package
 *
 * @section DESCRIPTION
 * Commonly used definitions for the Canvas Framework.
 */
#pragma once

#include "Config.h"

namespace Sim {

#	define EXPORT __attribute__((visibility("default")))
#	define HIDDEN __attribute__((visibility("hidden")))

#	define SIM_ENDIAN_LITTLE 1
#	define SIM_ENDIAN_BIG 2

#	define SIM_ARCH_32 1
#	define SIM_ARCH_64 2

	// set precision
#	ifdef SIM_DOUBLE_PRECISION
		typedef double Real;
#		define EPSILON 1e-9
#	else
		typedef float Real;
#		define EPSILON 1e-6
#	endif

	// common lambda functions
	auto DELETE_ARRAY = [] (auto a) { delete [] a; };
	auto SAFE_DELETE = [] (auto x){ if (x != nullptr) {delete x; x = nullptr;} };
	auto ABSOLUTE = [] (auto f) { return f > 0. ? f : -f; };
	auto SIGN = [] (auto x) { return x > 0. ? 1 : -1; };
	auto MAX = [] (auto x, auto y) { return x > y ? x : y; };
}
