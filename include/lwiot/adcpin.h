/*
 * Analog digital converter chip.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <lwiot/types.h>

namespace lwiot
{
	class AdcChip;

	class AdcPin {
	public:
		AdcPin(int pin);
		explicit AdcPin(int pin, AdcChip& chip);

		size_t read() const;

	private:
		AdcChip& _chip;
		int _pin;
	};
}
