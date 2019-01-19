/*
 * GPIO strategy for the ESP8266.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <esp8266.h>
#include <lwiot.h>

#include <esp/rtc_regs.h>
#include <esp/gpio.h>
#include <esp/gpio_regs.h>

#include <espressif/esp8266/eagle_soc.h>
#include <espressif/esp8266/ets_sys.h>
#include <espressif/esp_common.h>

#include <lwiot/types.h>
#include <lwiot/io/gpiochip.h>
#include <lwiot/esp8266/esp8266gpiochip.h>
#include <lwiot/log.h>


#define PIN16 16

static irq_handler_t __handlers[PINS] = {0,};


namespace lwiot { namespace esp8266
{
	static void IRAM gpio_irq_handler(uint8_t num)
	{
		printf("In IRQ");
		if(num > PINS)
			return;

		auto handler = __handlers[num];
		if(handler == nullptr)
			return;

		handler();
	}
	GpioChip::GpioChip() : lwiot::GpioChip(PINS)
	{ }

	void GpioChip::mode(int pin, const PinMode& mode)
	{
		if(pin == PIN16) {
			this->mode16(mode);
			return;
		}

		uint8_t _pin = pin;

		switch(mode) {
		case INPUT_PULLUP:
			GPIO.ENABLE_OUT_CLEAR = BIT(_pin);
			iomux_set_gpio_function(_pin, false);
			this->setPullup(_pin, true);
			break;

		case INPUT_NOPULLUP:
			GPIO.ENABLE_OUT_CLEAR = BIT(_pin);
			iomux_set_gpio_function(_pin, false);
			this->setPullup(_pin, false);
			break;

		case INPUT:
			GPIO.ENABLE_OUT_CLEAR = BIT(_pin);
			iomux_set_gpio_function(_pin, false);
			break;

		case OUTPUT:
			GPIO.CONF[_pin] &= ~GPIO_CONF_OPEN_DRAIN;
			GPIO.ENABLE_OUT_SET = BIT(_pin);
			iomux_set_gpio_function(_pin, true);
			break;

		case OUTPUT_OPEN_DRAIN:
			GPIO.CONF[_pin] |= GPIO_CONF_OPEN_DRAIN;
			GPIO.ENABLE_OUT_SET = BIT(_pin);
			iomux_set_gpio_function(_pin, true);
			break;

		default:
			print_dbg("Attempting to set invalid GPIO mode!\n");
			break;
		}
	}

	void IRAM GpioChip::write(int pin, bool value)
	{
		uint8_t _pin = pin;

		if(_pin == 16) {
			RTC.GPIO_OUT = (RTC.GPIO_OUT & 0xfffffffe) | (value ? 1 : 0);
		} else if(value) {
			GPIO.OUT_SET = BIT(_pin) & GPIO_OUT_PIN_MASK;
		} else {
			GPIO.OUT_CLEAR = BIT(_pin) & GPIO_OUT_PIN_MASK;
		}
	}

	bool GpioChip::read(int pin) const
	{
		auto rv = 0;

		if(pin == PIN16)
			rv = RTC.GPIO_IN & 1U;
		else
			rv = GPIO.IN & BIT(pin);

		return rv != 0;
	}

	void GpioChip::setOpenDrain(int pin)
	{
		this->mode(pin, OUTPUT_OPEN_DRAIN);
		this->write(pin, true);
	}

	void GpioChip::odWrite(int pin, bool value)
	{
		this->write(pin, value);
	}

	void GpioChip::setPullup(int pin, bool enable)
	{
		uint32_t flags = 0;

		if(enable)
			flags |= IOMUX_PIN_PULLUP;

		flags |= IOMUX_PIN_PULLUP_SLEEP;
		iomux_set_pullup_flags(gpio_to_iomux(pin), flags);
	}

	void GpioChip::mode16(const PinMode& mode)
	{
		RTC.GPIO_CFG[3] = (RTC.GPIO_CFG[3] & 0xffffffbc) | 1;
		RTC.GPIO_CONF = (RTC.GPIO_CONF & 0xfffffffe) | 0;

		switch(mode) {
		case INPUT_NOPULLUP:
			RTC.GPIO_ENABLE = (RTC.GPIO_OUT & 0xfffffffe);
			this->setPullup(16, false);
			break;

		case INPUT_PULLUP:
			RTC.GPIO_ENABLE = (RTC.GPIO_OUT & 0xfffffffe);
			this->setPullup(16, true);
			break;

		case INPUT:
			RTC.GPIO_ENABLE = (RTC.GPIO_OUT & 0xfffffffe);
			break;

		case OUTPUT:
		case OUTPUT_OPEN_DRAIN:
			RTC.GPIO_ENABLE = (RTC.GPIO_OUT & 0xfffffffe) | 1;
			break;

		default:
			print_dbg("Attempting to set invalid GPIO mode!\n");
			break;
		}
	}

	void GpioChip::attachIrqHandler(int pin, irq_handler_t handler, IrqEdge edge)
	{
		auto _edge = this->mapIrqEdge(edge);

		if(_edge == GPIO_INTTYPE_NONE || pin >= PINS)
			return;

		this->mode(pin, INPUT);
		__handlers[pin] = handler;
		gpio_set_interrupt(pin, _edge, gpio_irq_handler);
	}

	gpio_inttype_t GpioChip::mapIrqEdge(const IrqEdge& edge) const
	{
		switch(edge) {
		case IrqRising:
			return GPIO_INTTYPE_EDGE_POS;

		case IrqFalling:
			return GPIO_INTTYPE_EDGE_NEG;

		case IrqRisingFalling:
			return GPIO_INTTYPE_EDGE_ANY;

		default:
			return GPIO_INTTYPE_NONE;
		}
	}
}
}

static lwiot::esp8266::GpioChip esp_gpio;
lwiot::GpioChip& gpio = esp_gpio;
