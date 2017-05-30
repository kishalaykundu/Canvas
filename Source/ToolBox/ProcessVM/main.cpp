/**
 * @file main.cpp
 * @author Kishalay Kundu <kishalay.kundu@gmail.com>
 * @section LICENSE
 * See LICENSE.txt included in this package
 *
 * @section DESCRIPTION
 * This application converts the VoxelMan datasets into individual
 * texture files for segmented organs. The objectlists.txt in the
 * VoxelMan dataset is used to find labels and generate specific
 * texture files.
 */

#include <cstdlib>
#include <string>
#include "tinyxml2.h"

#include "Log.h"
#include "ConfigParser.h"

#include "VMProcessor.h"

using std::string;
using tinyxml2::XMLElement;
using tinyxml2::XMLError;
using tinyxml2::XML_SUCCESS;

int main (int argc, const char** argv)
{
	// sanity check
	if (argc == 2) {
		if (!strcmp (argv [1], "-h") || !strcmp (argv [1], "--help")){
			std::cout << "Usage: ./Bin/processvm <config file> (default: Assets/Config/VMConfig.xml)" << std::endl;
			exit (EXIT_SUCCESS);
		}
	}
	const char* input = nullptr;
	if (argc < 2){
		input = "Assets/Config/VMConfig.xml";
	} else {
		input = argv [1];
	}

	Sim::ConfigParser parser;
	if (!parser.Initialize (input, "VMConfig")){
		LOG ("FATAL ERROR: Could not read " << input);
		exit (EXIT_FAILURE);
	}

	XMLElement* element = nullptr;

	// Get data folder location
	if ((element = parser.GetElement ("Data")) == nullptr){
		LOG ("FATAL ERROR: No data element specified in " << input);
		exit (EXIT_FAILURE);
	}
	const char* dataFolder = element->Attribute ("Location");
	if (dataFolder == nullptr){
		LOG ("FATAL ERROR: No location specified for data folder in " << input);
		exit (EXIT_FAILURE);
	}

	// Get CT data folder and data prefix
	if ((element = parser.GetElement ("CT")) == nullptr){
		LOG ("FATAL ERROR: No CT folder specified in " << input);
		exit (EXIT_FAILURE);
	}
	const char* folder = element->Attribute ("Location");
	if (folder == nullptr){
		LOG ("FATAL ERROR: No location specified for CT folder in " << input);
		exit (EXIT_FAILURE);
	}
	const char* prefix = element->Attribute ("Prefix");
	if (prefix == nullptr){
		LOG ("FATAL ERROR: No data prefix specified for CT folder in " << input);
		exit (EXIT_FAILURE);
	}
	string ctFolder (dataFolder);
	if (ctFolder [ctFolder.length () - 1] != '/'){
		ctFolder.append ("/");
	}
	ctFolder += folder;
	if (ctFolder [ctFolder.length () - 1] != '/'){
		ctFolder.append ("/");
	}
	string ctPrefix (prefix);

	prefix = nullptr;
	folder = nullptr;
	element = nullptr;

	// Get RGB data folder and data prefix
	if ((element = parser.GetElement ("RGB")) == nullptr){
		LOG ("FATAL ERROR: No RGB folder specified in " << input);
		exit (EXIT_FAILURE);
	}
	folder = element->Attribute ("Location");
	if (folder == nullptr){
		LOG ("FATAL ERROR: No location specified for RGB folder in " << input);
		exit (EXIT_FAILURE);
	}
	prefix = element->Attribute ("Prefix");
	if (prefix == nullptr){
		LOG ("FATAL ERROR: No data prefix specified for RGB folder in " << input);
		exit (EXIT_FAILURE);
	}
	string rgbFolder (dataFolder);
	if (rgbFolder [rgbFolder.length () - 1] != '/'){
		rgbFolder.append ("/");
	}
	rgbFolder += folder;
	if (rgbFolder [rgbFolder.length () - 1] != '/'){
		rgbFolder.append ("/");
	}
	string rgbPrefix (prefix);

	prefix = nullptr;
	folder = nullptr;
	element = nullptr;

	// Get Mask data folder and data prefix
	if ((element = parser.GetElement ("Mask")) == nullptr){
		LOG ("FATAL ERROR: No Label folder specified in " << input);
		exit (EXIT_FAILURE);
	}
	folder = element->Attribute ("Location");
	if (folder == nullptr){
		LOG ("FATAL ERROR: No location specified for Label folder in " << input);
		exit (EXIT_FAILURE);
	}
	prefix = element->Attribute ("Prefix");
	if (prefix == nullptr){
		LOG ("FATAL ERROR: No data prefix specified for Label folder in " << input);
		exit (EXIT_FAILURE);
	}
	string maskFolder (dataFolder);
	if (maskFolder [maskFolder.length () - 1] != '/'){
		maskFolder.append ("/");
	}
	maskFolder += folder;
	if (maskFolder [maskFolder.length () - 1] != '/'){
		maskFolder.append ("/");
	}
	string maskPrefix (prefix);
	prefix = nullptr;
	folder = nullptr;
	element = nullptr;

	// Get height and width of the image files
	if ((element = parser.GetElement ("Images")) == nullptr){
		LOG ("FATAL ERROR: Image width and height are not specified in " << input);
		exit (EXIT_FAILURE);
	}
	XMLError error = XML_SUCCESS;

	unsigned int width = 0, height = 0;
	if ((error = element->QueryUnsignedAttribute ("Width", &width)) != XML_SUCCESS){
		LOG ("FATAL ERROR: Could not read image width in " << input);
		exit (EXIT_FAILURE);
	}
	if ((error = element->QueryUnsignedAttribute ("Height", &height)) != XML_SUCCESS){
		LOG ("FATAL ERROR: Could not read image width in " << input);
		exit (EXIT_FAILURE);
	}
	element = nullptr;

	// Get starting and ending suffixes for the files
	if ((element = parser.GetElement ("Indices")) == nullptr){
		LOG ("FATAL ERROR: Starting and ending indices not specified in " << input);
		exit (EXIT_FAILURE);
	}
	error = XML_SUCCESS;

	unsigned int start = 0, end = 0;
	if ((error = element->QueryUnsignedAttribute ("From", &start)) != XML_SUCCESS){
		LOG ("FATAL ERROR: Could not read start index in " << input);
		exit (EXIT_FAILURE);
	}
	if ((error = element->QueryUnsignedAttribute ("To", &end)) != XML_SUCCESS){
		LOG ("FATAL ERROR: Could not read end index in " << input);
		exit (EXIT_FAILURE);
	}
	if (start > end){
		LOG ("FATAL ERROR: Invalid start and end indices specified in " << input);
		exit (EXIT_FAILURE);
	}
	element = nullptr;

	/********************** DATA PROCESSING STARTS HERE **********************/
	if ((element = parser.GetElement ("Segments")) == nullptr){
		LOG ("FATAL ERROR: No segments specified for output in " << input);
		exit (EXIT_FAILURE);
	}
	unsigned int numSegments = 0;
	if ((error = element->QueryUnsignedAttribute ("Count", &numSegments)) != XML_SUCCESS){
		LOG ("FATAL ERROR: No Count specified for output segments in " << input);
		exit (EXIT_FAILURE);
	}

	// iterate through all the segments and output data
	XMLElement* segments = element->FirstChildElement ("Data");
	for (unsigned int i = 0; i < numSegments; ++i){

		// sanity check (this happens if count specified is more than existing segments)
		if (segments == nullptr){
			break;
		}
		const char* name = segments->Attribute ("Name");
		if (name == nullptr){
			LOG_ERROR ("No name specified for DataSet no. " << (i + 1) << " in " << input);
			break;
		}
		const char* outfolder = segments->Attribute ("Output");
		if (outfolder == nullptr){
			LOG_ERROR ("No output folder specified for " << name << " in " << input);
			break;
		}
		string prefix (name);
		string output (outfolder);
		if (output [output.size () - 1] != '/'){
			output += "/";
		}

		XMLElement* lelem = segments->FirstChildElement ("Labels");
		if (lelem == nullptr){
			LOG_ERROR ("No labels specified for DataSet no. " << (i + 1) << " in " << input);
			break;
		}
		unsigned int lcount = 0;
		if ((error = lelem->QueryUnsignedAttribute ("Count", &lcount)) != XML_SUCCESS){
			LOG_ERROR ("Could not read the number of labels for " << name << " in " << input);
			break;
		}

		unsigned int* labels = new unsigned int [lcount];
		unsigned int tmp = 0, ind = 0;
		XMLElement* items = lelem->FirstChildElement ("Item");
		while (items != nullptr){
			if ((error = items->QueryUnsignedText (&tmp)) != XML_SUCCESS){
				LOG_ERROR ("Invalid label listed for " << name << " in " << input);
				break;
			}
			labels [ind] = tmp;
			++ind;
			items = items->NextSiblingElement ("Item");
		}
		lcount = ind;

		Sim::VMProcessor vm (ctFolder, maskFolder, rgbFolder, ctPrefix, maskPrefix, rgbPrefix, output, prefix);
		vm.Width () = width;
		vm.Height () = height;
		vm.From () = start;
		vm.To () = end;

		const char* rgbout = segments->Attribute ("RGBOut");
		if (rgbout != nullptr){
			if (!strcmp ("True", rgbout)){
				vm.WriteRgb () = true;
			}
		}

		if (!vm.Process (lcount, labels)){
			LOG ("ERROR: Could not read data files for " << name << " in " << input);
			break;
		}
		delete [] labels;

		segments = segments->NextSiblingElement ("Data");
	}

	LOG ("Success: ProcessVM finished");

	exit (EXIT_SUCCESS);
}
