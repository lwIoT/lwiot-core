/*
 * Analog digital converter chip.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

namespace lwiot
{
	class AdcChip;

	class AdcPin {
	public:
		AdcPin(int pin);
		explicit AdcPin(int pin, const AdcChip& chip);

		size_t read();

	private:
		AdcChip& _chip;
	};
}
