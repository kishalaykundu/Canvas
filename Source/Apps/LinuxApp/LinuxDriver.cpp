/**
 * @file LinuxDriver.cpp
 * @author Kishalay Kundu <kishalay.kundu@gmail.com>
 * @section LICENSE
 * See LICENSE.txt included in this package
 *
 * @section DESCRIPTION
 * See LinuxDriver.h.
 */

#include <memory>

#include "tinyxml2.h"

#include "Config.h"
#include "Log.h"
#include "ConfigParser.h"
#include "Driver/Driver.h"
#include "LinuxApp/LinuxDriver.h"

#ifdef SIM_GL_ENABLED
#	include "Render/GL45/GLManager.h"
#elif SIM_VK_ENABLED
#	include "Render/VK/VKManager.h"
#endif

#ifdef SIM_CUDA_ENABLED
#	include "Compute/Cuda/CudaManager.h"
#elif SIM_CL_ENABLED
#	include "Compute/CL/CLManager.h"
#else
#	include "Compute/ComputeManger.h"
#endif

#ifdef SIM_TBB_SCHEDULER_ENABLED
#	include "Tasks/TBB/TbbManager.h"
#elif SIM_THREAD_SCHEDULER_ENABLED
#	include "Tasks/Threads/ThreadManager.h"
#else
#	include "Tasks/TaskManager.h"
#endif

using std::make_unique;
using tinyxml2::XMLElement;

namespace Sim {

	Driver* Driver::_instance = new LinuxDriver ();

	LinuxDriver::~LinuxDriver () {Cleanup ();}

	bool LinuxDriver::Initialize (const char* config)
	{
		if (config == nullptr){
			LOG_ERROR ("No input configuration file specified");
			return false;
		}

		// read configuration file
		ConfigParser parser;
		if (!parser.Initialize (config, "AppConfig")){
			LOG_ERROR ("Could not initialize parser for " << config);
			return false;
		}

		// Initialize the event manager (always the first module to be initialized)
		XMLElement* element = parser.GetElement ("EventManager");
		if (element == nullptr){
			LOG_ERROR ("Event manager profile not found in " << config);
			return false;
		}
		if (!InitializeEventManager (element->Attribute ("Config"))){
			Cleanup ();
			return false;
		}
		element = nullptr;

		/**
		 * Initialize renderer and display window. The input configuration may contain
		 * multiple renderer profiles. We pick either the OpenGL or Vulkan type.
		 */
		element = parser.GetElement ("RenderManager");
#		ifdef SIM_GL_ENABLED
		while (element != nullptr && strcmp (element->Attribute ("Type"), "OpenGL")){
#		elif SIM_VK_ENABLED
		while (element != nullptr && strcmp (element->Attribute ("Type"), "Vulkan")){
#		endif
			element = element->NextSiblingElement ("RenderManager");
		}
		if (element == nullptr){
			LOG_ERROR ("Render manager profile not found in " << config);
			Cleanup ();
			return false;
		}
		if (!InitializeRenderManager (element->Attribute ("Config"))){
			Cleanup ();
			return false;
		}
		element = nullptr;

		/**
		 * Initialize HPC computing manager. The input configuration may contain
		 * multiple HPC profiles. We pick the one with type specified by install
		 */
		element = parser.GetElement ("ComputeManager");
#		ifdef SIM_CUDA_ENABLED
		while (element != nullptr && strcmp (element->Attribute ("Type"), "CUDA")){
#		elif SIM_CL_ENABLED
		while (element != nullptr && strcmp (element->Attribute ("Type"), "OpenCL")){
#		else
		while (element != nullptr && strcmp (element->Attribute ("Type"), "CPU")){
#		endif
			element = element->NextSiblingElement ("ComputeManager");
		}
		if (element == nullptr){
			LOG_ERROR ("High Performance computing manager profile not found in " << config);
			Cleanup ();
			return false;
		}
		if (!InitializeComputeManager (element->Attribute ("Config"))){
			Cleanup ();
			return false;
		}
		element = nullptr;

		// Initialize plugin manager (loads all dynamic plugin libraries)
		element = parser.GetElement ("PluginManager");
		if (element == nullptr){
			LOG_ERROR ("Plugin manager profile not found in " << config);
			Cleanup ();
			return false;
		}
		if (!InitializePluginManager (element->Attribute ("Config"))){
			Cleanup ();
			return false;
		}
		element = nullptr;

		// Initialize asset factory and load all assets
		element = parser.GetElement ("AssetManager");
		if (element == nullptr){
			LOG_ERROR ("Asset manager profile not found in " << config);
			Cleanup ();
			return false;
		}
		if (!InitializeAssetManager (element->Attribute ("Config"))){
			Cleanup ();
			return false;
		}
		element = nullptr;

		/**
		 * Initialize Intel TBB task manager. The input configuration may contain
		 * multiple task manager profiles. We pick the one with "IntelTBB" type.
		 */
		element = parser.GetElement ("TaskManager");
#		ifdef SIM_TBB_SCHEDULER_ENABLED
		while (element != nullptr && strcmp (element->Attribute ("Type"), "IntelTBB")){
#		elif SIM_THREAD_SCHEDULER_ENABLED
		while (element != nullptr && strcmp (element->Attribute ("Type"), "Thread")){
#		else
		while (element != nullptr && strcmp (element->Attribute ("Type"), "CPU")){
#		endif
			element = element->NextSiblingElement ("TaskManager");
		}
		if (element == nullptr){
			LOG_ERROR ("Task scheduler profile not found in " << config);
			Cleanup ();
			return false;
		}
		if (!InitializeTaskManager (element->Attribute ("Config"))){
			Cleanup ();
			return false;
		}
		element = nullptr;

		// Driver has been initialized and ready to run
		_runFlag = true;
		LOG ("Linux driver initialized");

		return true;
	}

	void LinuxDriver::Run ()
	{

	}

	void LinuxDriver::Cleanup ()
	{
		_assetManager.reset ();
		_pluginManager.reset ();
		_eventManager.reset ();
	}

	bool LinuxDriver::InitializeRenderManager (const char* config)
	{
#		ifdef SIM_GL_ENABLED
		_renderManager = make_unique <GLManager> ();
#		elif SIM_VK_ENABLED
		_renderManager = make_unique <VKManager> ();
#		endif
		if (!_renderManager->Initialize (config)){
			LOG_ERROR ("Render manager could not be initialize with " << config);
			return false;
		}
		return true;
	}

	bool LinuxDriver::InitializeComputeManager (const char* config)
	{
#		ifdef SIM_CUDA_ENABLED
		_computeManager = make_unique <CudaManager> ();
#		elif SIM_CL_ENABLED
		_computeManager = make_unique <CLManager> ();
#		else
		_computeManager = make_unique <ComputeManager> ();
#		endif
		if (!_computeManager->Initialize (config)){
			LOG_ERROR ("High Performance Computing manager could not be initialize with " << config);
			return false;
		}
		return true;
	}

	bool LinuxDriver::InitializeTaskManager (const char* config)
	{
#		ifdef SIM_TBB_SCHEDULER_ENABLED
		_taskManager = make_unique <TbbManager> ();
#		elif SIM_THREAD_SCHEDULER_ENABLED
		_taskManager = make_unique <ThreadManager> ();
#		else
		_taskManager = make_unique <TaskManager> ();
#		endif
		if (!_renderManager->Initialize (config)){
			LOG_ERROR ("Task manager could not be initialize with " << config);
			return false;
		}
		return true;
	}

}
