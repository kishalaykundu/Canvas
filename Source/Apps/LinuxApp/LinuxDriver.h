/**
 * @file LinuxDriver.h
 * @author Kishalay Kundu <kishalay.kundu@gmail.com>
 * @section LICENSE
 * See LICENSE.txt included in this package
 *
 * @section DESCRIPTION
 * Linux specific driver derived from the Driver class.
 */

#include "Plugins/Plugin.h"
#include "Driver/Driver.h"

namespace Sim {

	class LinuxDriver : public Driver {

		friend class Driver;

	protected:
		LinuxDriver () = default;
		LinuxDriver (const LinuxDriver&) = delete;
		LinuxDriver& operator = (const LinuxDriver&) = delete;

	public:
		~LinuxDriver ();

		bool Initialize (const char* config) override;
		void Run () override;
		void Cleanup () override;
	};
}
