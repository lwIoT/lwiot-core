/*
 * Analog digital converter chip.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <lwiot/types.h>

/// @file adcpin.h

namespace lwiot
{
	class AdcChip;

	/**
	 * @brief ADC pin abstraction.
	 * @ingroup io
	 */
	class AdcPin {
	public:
		/**
		 * @brief Construct an ADC pin.
		 * @param pin Pin number.
		 */
		explicit AdcPin(int pin);
		/**
		 * @brief Construct an ADC pin.
		 * @param pin Pin number.
		 * @param chip ADC chip.
		 */
		explicit AdcPin(int pin, AdcChip& chip);

		size_t read() const; //!< Read from the pin.
		const int& pin() const; //!< Get the pin number.

		bool operator ==(const AdcPin& pin); //!< Comparison operator.
		bool operator >(const AdcPin& pin); //!< Comparison operator.
		bool operator <(const AdcPin& pin); //!< Comparison operator.
		bool operator >=(const AdcPin& pin); //!< Comparison operator.
		bool operator <=(const AdcPin& pin); //!< Comparison operator.

	private:
		AdcChip& _chip;
		int _pin;
	};
}
