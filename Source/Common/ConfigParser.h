/**
 * @file ConfigParser.h
 * @author Kishalay Kundu <kishalay.kundu@gmail.com>
 * @section LICENSE
 * See LICENSE.txt included in this package
 *
 * @section DESCRIPTION
 * The XML-styled configuration file parser for the Chimera framework.
 */
#pragma once

#include "tinyxml2.h"

namespace Sim {

	class ConfigParser {
		private:
			tinyxml2::XMLDocument _doc;
			tinyxml2::XMLNode* _root = nullptr;

		public:
			ConfigParser () = default;
			~ConfigParser () = default;

			// forbidden copy and assignment operators
			ConfigParser (const ConfigParser&) = delete;
			ConfigParser& operator = (const ConfigParser&) = delete;

			bool Initialize (const char* inputfile, const char* rootName);
			const char* DocName () const;
			tinyxml2::XMLElement* GetElement (const char*, tinyxml2::XMLElement* root = nullptr);

	};
}
