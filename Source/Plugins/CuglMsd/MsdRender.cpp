/**
 * @file MsdRender.cpp
 * @author Kishalay Kundu <kishalay.kundu@gmail.com>
 * @section LICENSE
 * See LICENSE.txt included in this package
 *
 * @section DESCRIPTION
 * See MsdRender.h.
 */

#include <string>
#include "tinyxml2.h"

#include "Driver/Driver.h"

#include "Render/GL45/GLManager.h"
#include "Render/GL45/GLProgram.h"

#include "MsdRender.h"

using tinyxml2::XMLElement;

namespace Sim {
	namespace Assets {

		bool MsdRender::Initialize (XMLElement& element, Asset* asset)
		{
			// load the programs to be used for rendering purposes
			XMLElement* programs = element.FirstChildElement ("Programs");
			if (programs == nullptr){
				LOG_ERROR ("No shading programs specified in " << element.GetText ());
				return false;
			}
			if (!LoadPrograms (*programs)){
				LOG_ERROR ("Could not load all shader programs in " << element.GetText ());
				return false;
			}
			programs = nullptr;

			return true;
		}

		void MsdRender::Cleanup () {}

		bool MsdRender::LoadPrograms (XMLElement& element)
		{
			XMLElement* plist = element.FirstChildElement ("Program");
			if (plist == nullptr){
				LOG_ERROR ("No programs specified in config");
				return false;
			}
			while (plist != nullptr){

				const char* type = plist->Attribute ("Type");
				if (type == nullptr){
					LOG_ERROR ("No type specified for program");
					return false;
				}

				if (!strcmp ("Normal", type)){
					_program [0] = (static_cast <GLManager*> (Driver::Instance ().GetRenderManager()))->AddProgram (*plist);
					if (!_program [0]){
						LOG_ERROR ("Could not add Normal program");
						return false;
					}
				}
				else if (!strcmp ("MSD", type)){
					_program [1] = (static_cast <GLManager*> (Driver::Instance ().GetRenderManager()))->AddProgram (*plist);
					if (!_program [1]){
						LOG_ERROR ("Could not add Normal program");
						return false;
					}
				}
				else {
					LOG_ERROR ("Non recognizable program type: " << type);
					return false;
				}

				plist = plist->NextSiblingElement ("Program");
			}

			// check that both programs have been loaded
			if (!_program [0] || !_program [1]){
				LOG_ERROR ("All needed programs not loaded");
				return false;
			}

			return true;
		}
	}
}
