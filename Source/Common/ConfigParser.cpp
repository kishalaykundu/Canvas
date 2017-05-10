/**
 * @file ConfigParser.cpp
 * @author Kishalay Kundu <kishalay.kundu@gmail.com>
 * @section LICENSE
 * See LICENSE.txt included in this package
 *
 * @section DESCRIPTION
 * See ConfigParser.h
 */
#include <cstring>
#include "tinyxml2.h"
#include "Log.h"
#include "ConfigParser.h"

using tinyxml2::XMLElement;
using tinyxml2::XMLError;
using tinyxml2::XML_SUCCESS;

namespace Sim {

	bool ConfigParser::Initialize (const char* config, const char* rootName)
	{
		XMLError result = _doc.LoadFile (config);
		if (result != XML_SUCCESS){
			LOG_ERROR ("Could not open " << config << " - either file does not exist or error while parsing file");
			return false;
		}

		_root = _doc.FirstChild ();
		if (_root == nullptr){
			LOG_ERROR ("Could not grab root element from " << config << ": " << _doc.ErrorName () << "...Exiting");
			return false;
		}
		if (strcmp (_root->Value (), rootName)){
			LOG_ERROR ("Root element in " << config << " is " << _root->Value () << " (not \'" << rootName << "\')...Exiting");
			return false;
		}
		return true;
	}

	const char* ConfigParser::DocName () const
	{
		return _doc.Value ();
	}

	XMLElement* ConfigParser::GetElement (const char* name, tinyxml2::XMLElement* root)
	{
		XMLElement* result = nullptr;

		if (root == nullptr){
			result = _root->FirstChildElement (name);
		}
		else {
			result = root->FirstChildElement (name);
		}

		if (result == nullptr){
			if (root == nullptr){
				LOG_ERROR ("Could not find " << name << " in " << _root->Value ());
			}
			else {
				LOG_ERROR ("Could not find " << name << " in " << root->Value ());
			}
		}
		return result;
	}
}
