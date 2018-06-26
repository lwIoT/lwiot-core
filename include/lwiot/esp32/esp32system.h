/*
 * Esp32 system interface class definition.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <lwiot.h>

#include <lwiot/types.h>
#include <lwiot/system.h>


namespace lwiot
{	
	class Esp32System : public System {
	public:
		explicit Esp32System(SleepState state = SLEEP_DEEP);
		virtual ~Esp32System() = default;

		virtual void sleep(const uint32_t& ms) const override;
		virtual void restart() const override;
	};
}
