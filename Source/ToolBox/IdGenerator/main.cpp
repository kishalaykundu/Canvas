/***
 * Program to read xml-style input file and generate unique id's
 * for each of the inputs provided. The output is also written out
 * to file "Assets/Config/MapId.xml
 */

#include <iostream>

#include "tinyxml2.h"
#include "ConfigParser.h"
#include "UIDGenerator.h"

using std::cout;
using std::cerr;
using std::endl;
using tinyxml2::XMLElement;
using Sim::ConfigParser;
using Sim::UIDGenerator;

int main (int argc, char** argv)
{
	if (argc < 2){
		cerr << "Usage: ./Bin/generateIds <input file with path name>" << endl;
	}

	ConfigParser ip;
	if (!ip.Initialize (argv [1], "SimMapInput")){
		exit (EXIT_FAILURE);
	}

	tinyxml2::XMLDocument doc;
	tinyxml2::XMLNode* root = doc.NewElement ("NameMap");
	doc.InsertFirstChild (root);

	const XMLElement* ielem = ip.GetElement ("Input");
	while (ielem != nullptr){

		XMLElement* elem = doc.NewElement ("Map");
		elem->SetAttribute ("Type", ielem->Attribute ("Type"));
		elem->SetAttribute ("Name", ielem->Attribute ("Name"));
		elem->SetAttribute ("Key", UIDGenerator::Instance ().GetUniqueId ());

		root->InsertEndChild (elem);

		ielem = ielem->NextSiblingElement ();
	}

	tinyxml2::XMLError error = doc.SaveFile ("Assets/Config/MapId.xml");

	exit (EXIT_SUCCESS);
}
