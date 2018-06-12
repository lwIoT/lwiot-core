/*
 * UART interface class.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <stdint.h>
#include <lwiot.h>

#include <lwiot/gpiopin.h>
#include <lwiot/gpiochip.h>
#include <lwiot/uart.h>

namespace lwiot
{
	Uart::Uart(int tx, int rx, long baud, uint32_t config) : Uart(GpioPin(tx), GpioPin(rx), baud, config)
	{ }
	
	Uart::Uart(const GpioPin& tx, const GpioPin& rx, long baud, uint32_t config) :
		Stream(),
		_tx(tx), _rx(rx), _baud(baud), _config(config)
	{ }
}
