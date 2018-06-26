/*
 * ESP8266 system interface class definition.
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
	class Esp8266System : public System {
	public:
		explicit Esp8266System(SleepState state = SLEEP_DEEP);
		virtual ~Esp8266System() = default;

		virtual void sleep(const uint32_t& ms) const override;
		virtual void restart() const override;
	};
}
