/**
 * @file Driver.h
 * @author Kishalay Kundu <kishalay.kundu@gmail.com>
 * @section LICENSE
 * See LICENSE.txt included in this package
 *
 * @section DESCRIPTION
 * The abstract factory responsible for adding and maintaining
 * the different manager (factory) modules. This is designed so
 * that the concrete apps (clients) can, depending on the input
 * config file spawn and run different modules.
 */
#pragma once

#include <string>
#include <map>
#include <memory>

namespace Sim {

	class Manager;
	class Plugin;
	class Asset;

	class Driver {

	protected:
		bool _runFlag = false;
		Driver* _instance = nullptr;
		std::map <unsigned int, std::unique_ptr <Manager> > _managers;

		Driver () = default;
		Driver (const Driver&) = delete;
		Driver& operator = (const Driver&) = delete;

	public:
		~Driver () = default;

		static Driver& Instance () {return *_instance;}

		bool AddManager (unsigned int id, std::unique_ptr <Manager> manager);
		bool AddPlugin (unsigned int id, std::unique_ptr <Plugin> plugin);
		bool AddAsset (unsigned int id, std::shared_ptr <Asset> asset);

		Manager* GetManager (unsigned int id);
		Plugin* GetPlugin (unsigned int id);
		Asset* GetAsset (unsigned int id);

		bool Initialize (const char* configfile);
		void Run ();
		void Cleanup ();
		void Quit ();
	};
}
