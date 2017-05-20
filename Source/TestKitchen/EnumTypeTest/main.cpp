/**
 * @file main.cpp
 * @author Kishalay Kundu <kishalay.kundu@gmail.com>
 * @section LICENSE
 * See LICENSE.txt included in this package
 *
 * @section DESCRIPTION
 * Test module to incorporate various functionalities for Enum types.
 */

#include <cstdlib>
#include <iostream>
#include <string>

#include "Log.h"

using std::ostream;
using std::string;

enum class MyEnum {ONE, TWO, THREE, UNKNOWN};

inline ostream& operator << (ostream& output, const MyEnum& e) {
	switch (e){
	case MyEnum::ONE: output << "ONE"; break;
	case MyEnum::TWO: output << "TWO"; break;
	case MyEnum::THREE: output << "THREE"; break;
	case MyEnum::UNKNOWN: output << "UNKNOWN"; break;
	}
	return output;
};

auto MyEnumFromString = [=] (const char* str){
	string estr (str);
	if (!estr.compare ("ONE")){
		return MyEnum::ONE;
	}
	else if (!estr.compare ("TWO")){
		return MyEnum::TWO;
	}
	else if (!estr.compare ("THREE")){
		return MyEnum::THREE;
	}
	else {
		return MyEnum::UNKNOWN;
	}
};

int main ()
{
	MyEnum me = MyEnum::TWO;

	LOG ("Enum type with lambda: " << me);
	LOG ("Enum from string (ONE): " << MyEnumFromString ("ONE"));
	LOG ("Enum from string (TWO): " << MyEnumFromString ("TWO"));
	LOG ("Enum from string (THREE): " << MyEnumFromString ("THREE"));
	exit (EXIT_SUCCESS);
}
