/**
 * @file Log.h
 * @author Kishalay Kundu <kishalay.kundu@gmail.com>
 * @section LICENSE
 * See LICENSE.txt included in this package
 *
 * @section DESCRIPTION
 * Lightweight logging facility.
 */
#pragma once

#include <cstring> // basename (...) is defined in cstring
#include <iostream>

#include "Config.h"

using std::cout;
using std::cerr;
using std::endl;

#ifdef SIM_LOG_ENABLED // output log messages

#	ifndef NDEBUG// print debug message, prefixed by the file and line number

#		define LOG(x) (cout << basename( __FILE__ ) << "[" << __LINE__ << "]: " << x << "." << endl)
#		define LOG_ERROR(x) (cerr << basename( __FILE__ ) << "[" << __LINE__ << "]: ERROR: " << x << "." << endl)
#		define LOG_WARNING(x) (cout << basename( __FILE__ ) << "[" << __LINE__ << "]: WARNING: " << x << "." << endl)

#	else// print debug message, without the file and line number

#		define LOG(x) (cout << x << "." << endl)
#		define LOG_ERROR(x) (cerr << "ERROR: " << x << "." << endl)
#		define LOG_WARNING(x) (cout << "WARNING: " << x << "." << endl)

#	endif

#else // no messages

#	define LOG(x)
#	define LOG_ERROR(x)
#	define LOG_WARNING(x)

#endif
