/**
 * @file main.cpp
 * @author Kishalay Kundu <kishalay.kundu@gmail.com>
 * @section LICENSE
 * See LICENSE.txt included in this package
 *
 * @section DESCRIPTION
 * The entry point for the Canvas simulation framework. This is where
 * everything starts. Here, the driver, the simulation core logic and
 * user interfaces are initialized and synchronized first. The program
 * then goes on to rest in the simulation-loop and at the end, finally
 * exits cleanly.
 * This particular app is OpenGL based. There are other platform spec-
 * ific modules that can be added based on input configuration. This
 * includes CUDA or OpenCL HPC managers, TBB or Thread based schedulers
 * etc.
 */
#include <cstdlib>
#include <cstring>
#include <memory>

#include "tinyxml2.h"

#include "Preprocess.h"
#include "Log.h"
#include "ConfigParser.h"
#include "Driver/Driver.h"

#include "Driver/Manager.h"
#include "Events/EventManager.h"
#include "Graphics/GL45/GLManager.h"
#include "HPC/Cuda/CudaManager.h"
#include "Plugins/PluginFactory.h"
#include "Assets/AssetFactory.h"
#include "Tasks/Threads/ThreadScheduler.h"

using std::unique_ptr;
using std::make_unique;

using tinyxml2::XMLElement;
using Sim::Driver;
using Sim::Manager;

int main (int argc, const char** argv)
{
	// sanity check
	if (argc == 2) {
		if (!strcmp (argv [1], "-h") || !strcmp (argv [1], "--help")){
			LOG ("Usage: ./Bin/simulate <config file> (default: Assets/Config/AppConfig.xml)");
			exit (EXIT_SUCCESS);
		}
	}

	// READ INPUT FILE AND INITIALIZE
	{
		const char* input = nullptr;
		if (argc < 2){
			input = "Assets/Config/AppConfig.xml";
		} else {
			input = argv [1];
		}

		// INSTANTIATE ALL MANAGERS. (MANAGERS ARE NOT INITIALIZED HERE)
		LOG ("Reading " << input << "...");
		{
			Sim::ConfigParser parser;
			if (!parser.Initialize (input, "AppConfig")){
				LOG ("FATAL ERROR: Input configuration file " << input << " could not be initialized. ABORTING");
				Driver::Instance ().Cleanup ();
				exit (EXIT_FAILURE);
			}

			XMLElement* element = nullptr;

			// Initialize the Event manager
			if ((element = parser.GetElement ("EventManager")) != nullptr){
				unique_ptr <Manager> em = make_unique <Sim::EventManager> ();
				if (!Driver::Instance ().Add ("EventManager", em)){
					LOG ("FATAL ERROR: Could not add EventManager to the application");
					Driver::Instance ().Cleanup ();
					exit (EXIT_FAILURE);
				}
			}
			else {
				LOG ("FATAL ERROR: No event manager specified in " << input << ". ABORTING");
				Driver::Instance ().Cleanup ();
				exit (EXIT_FAILURE);
			}
			element = nullptr;

			// Initialize the Display system manager
			if ((element = parser.GetElement ("GpuManager")) != nullptr){
				const char* type = element->Attribute ("Type");
				const char* name = element->Attribute ("Name");
				if (type == nullptr || name == nullptr){
					LOG ("FATAL ERROR: Must specify type (OpenGL/Vulkan) and name for Display system. ABORTING");
					Driver::Instance ().Cleanup ();
					exit (EXIT_FAILURE);
				}
				unique_ptr <Manager> gm;
				if (!strcmp ("OpenGL", type)){
					gm = make_unique <Sim::GLManager> ();
				}
				else if (!strcmp ("Vulkan", type)){
					gm = make_unique <Sim::VkManager> ();
				}
				if (!Driver::Instance ().Add (name, gm)){
					LOG ("FATAL ERROR: Could not add " << name << " to the application");
					Driver::Instance ().Cleanup ();
					exit (EXIT_FAILURE);
				}
			}
			else {
				LOG ("FATAL ERROR: No display system specified in " << input << ". ABORTING");
				Driver::Instance ().Cleanup ();
				exit (EXIT_FAILURE);
			}
			element = nullptr;

			// Initialize High Performance Computing Managers (can be more than 1)
			element = nullptr;

			// Initialize Plugin library factory
			if ((element = parser.GetElement ("PluginFactory")) != nullptr){
				unique_ptr <Manager> pf = make_unique <Sim::PluginFactory> ();
				if (!Driver::Instance ().Add ("PluginFactory", pf)){
					LOG ("FATAL ERROR: Could not add PluginFactory to the application");
					Driver::Instance ().Cleanup ();
					exit (EXIT_FAILURE);
				}
			}
			else {
				LOG ("FATAL ERROR: No plugin factory specified in " << input << ". ABORTING");
				Driver::Instance ().Cleanup ();
				exit (EXIT_FAILURE);
			}
			element = nullptr;

			// Initialize Asset factory
			if ((element = parser.GetElement ("AssetFactory")) != nullptr){
				unique_ptr <Manager> af = make_unique <Sim::AssetFactory> ();
				if (!Driver::Instance ().Add ("AssetFactory", af)){
					LOG ("FATAL ERROR: Could not add AssetFactory to the application");
					Driver::Instance ().Cleanup ();
					exit (EXIT_FAILURE);
				}
			}
			else {
				LOG ("FATAL ERROR: No asset factory specified in " << input << ". ABORTING");
				Driver::Instance ().Cleanup ();
				exit (EXIT_FAILURE);
			}
			element = nullptr;

			// Initialize Task manager
			element = nullptr;
		}

		// INITIALIZE ALL MANAGERS AND THEIR RESPECTIVE DATA
		if (!Driver::Instance ().Initialize (input)){
			LOG ("FATAL ERROR: Application failed to start. ABORTING");

			Driver::Instance ().Cleanup ();
			exit (EXIT_FAILURE);
		}
	}

	// THE MAIN LOOP
	Driver::Instance ().Run ();

	// PREPARE FOR EXITING: CLEAN ALL APP RESOURCES
	Driver::Instance ().Cleanup ();

	exit (EXIT_SUCCESS);
}
