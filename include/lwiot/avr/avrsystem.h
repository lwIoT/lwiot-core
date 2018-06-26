/*
 * AVR system interface class definition.
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
	class AvrSystem : public System {
	public:
		explicit AvrSystem(SleepState state = SLEEP_DEEP);
		virtual ~AvrSystem();

		virtual void sleep(const uint32_t& ms) const override;
		virtual void restart() const override;

	private:
		static bool _restart;

		/* Methods */
		void setupWdt() const;
		friend void wdt_isr_handler();
	};
}
