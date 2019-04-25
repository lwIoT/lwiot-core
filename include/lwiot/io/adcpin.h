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
		const int& pin() const;

		bool operator ==(const AdcPin& pin);
		bool operator >(const AdcPin& pin);
		bool operator <(const AdcPin& pin);
		bool operator >=(const AdcPin& pin);
		bool operator <=(const AdcPin& pin);

	private:
		AdcChip& _chip;
		int _pin;
	};
}
