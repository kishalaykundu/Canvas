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

#include "Log.h"
#include "Driver/Driver.h"
#include "LinuxApp/LinuxDriver.h"

using Sim::Driver;

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
