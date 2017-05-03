/**
 * @file main.cpp
 * @author Kishalay Kundu <kishalay.kundu@gmail.com>
 * @section LICENSE
 * See LICENSE.txt included in this package
 *
 * @section DESCRIPTION
 * Test module to check whether current platform supports CUDA-OpenGL
 * interop with multithreading.
 */

#include <cstring>
#include <cstdlib>

#include <iostream>
#include <memory>
#include <thread>

#include "Callback.h"
#include "Mesh.h"
#include "GLDisplay.h"
#include "CuPhysics.h"

using std::cerr;
using std::endl;
using std::thread;
using std::shared_ptr;
using std::make_shared;

using util::Callback;

using Sim::Mesh;
using Sim::GLDisplay;
using Sim::CuPhysics;

void Invoke (Callback<void (void)> callback)
{

}

int main (int argc, const char** argv)
{
	if (argc < 3 || !strcmp ("-h", argv [1]) || !strcmp ("--help", argv [1])){
		cerr << "Usage: Bin/cuglTest <vertex file> <index file>" << endl;
		exit (EXIT_SUCCESS);
	}

	shared_ptr <Mesh> mesh = make_shared <Mesh> ();
	if (!mesh->Initialize (argv [1], argv [2])){
		cerr << "Could not load mesh from " << argv [1] << " and " << argv [2] << "....Aborting" << endl;
		exit (EXIT_FAILURE);
	}

	shared_ptr <GLDisplay> display = make_shared <GLDisplay> ();
	if (!display->Initialize (mesh)){
		cerr << "Could not load GL Display....Aborting" << endl;
		mesh.reset ();
		exit (EXIT_FAILURE);
	}
	shared_ptr <CuPhysics> physics= make_shared <CuPhysics> ();
	if (!physics->Initialize (display)){
		cerr << "Could not load CUDA physics....Aborting" << endl;
		display.reset ();
		mesh.reset ();
		exit (EXIT_FAILURE);
	}

	thread t1 (Invoke, BIND_MEM_CB (&GLDisplay::Run, display.get ()));
	thread t2 (Invoke, BIND_MEM_CB (&CuPhysics::Run, physics.get ()));

	physics->Cleanup ();
	display->Cleanup ();
	mesh.reset ();

	exit (EXIT_SUCCESS);
}
