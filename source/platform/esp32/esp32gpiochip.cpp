/*
 * GPIO implementation for the ESP32.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <lwiot.h>
#include <esp_intr.h>
#include <esp_attr.h>

#include <lwiot/io/gpiochip.h>
#include <lwiot/log.h>
#include <lwiot/esp32/esp32gpiochip.h>

#include <driver/gpio.h>
#include <rom/ets_sys.h>
#include <sys/reent.h>

#include "esp32gpio.h"

#ifdef PINS
#undef PINS
#endif

#define PINS 40

#ifndef HIGH
#define HIGH 1
#define LOW  0
#endif

/* Interrupt modes */
#define DISABLED  0x00
#define RISING    0x01
#define FALLING   0x02
#define CHANGE 	  0x03

static irq_handler_t gpio_isr_handlers[PINS] = {0};
static intr_handle_t gpio_intr_handle = nullptr;

static void IRAM_ATTR gpio_external_isr(void *arg)
{
	uint32_t gpio_intr_status_l = 0;
    uint32_t gpio_intr_status_h = 0;
    uint8_t pin = 0;

    gpio_intr_status_l = GPIO.status;
    gpio_intr_status_h = GPIO.status1.val;
    GPIO.status_w1tc = gpio_intr_status_l;//Clear intr for gpio0-gpio31
    GPIO.status1_w1tc.val = gpio_intr_status_h;//Clear intr for gpio32-39

    if(gpio_intr_status_l) {
        do {
            if(gpio_intr_status_l & ((uint32_t)1 << pin)) {
                if(gpio_isr_handlers[pin]) {
                    gpio_isr_handlers[pin]();
                }
            }
        } while(++pin<32);
    }

    if(gpio_intr_status_h) {
        pin=32;
        do {
            if(gpio_intr_status_h & ((uint32_t)1 << (pin - 32))) {
                if(gpio_isr_handlers[pin]) {
                    gpio_isr_handlers[pin]();
                }
            }
        } while(++pin<GPIO_PIN_COUNT);
	}
}

namespace lwiot { namespace esp32
{
	GpioChip::GpioChip() : lwiot::GpioChip(PINS)
	{ }

	void GpioChip::mode(int pin, const PinMode& mode)
	{
		gpio_num_t _pin = (gpio_num_t)pin;

		switch(mode) {
		case INPUT_PULLUP:
			gpio_set_direction(_pin, GPIO_MODE_INPUT);
			gpio_pullup_en(_pin);
			break;

		case INPUT_NOPULLUP:
			gpio_set_direction(_pin, GPIO_MODE_INPUT);
			gpio_pullup_dis(_pin);
			break;

		case INPUT:
			gpio_set_direction(_pin, GPIO_MODE_INPUT);
			gpio_set_pull_mode(_pin, GPIO_FLOATING);
			break;

		case OUTPUT:
			gpio_set_direction(_pin, GPIO_MODE_OUTPUT);
			gpio_pullup_dis(_pin);
			gpio_pulldown_dis(_pin);
			break;

		case OUTPUT_OPEN_DRAIN:
			gpio_set_direction(_pin, GPIO_MODE_INPUT_OUTPUT_OD);
			gpio_set_pull_mode(_pin, GPIO_FLOATING);
			break;

		default:
			print_dbg("Attempting to set invalid GPIO mode!\n");
			break;
		}
	}

	void IRAM_ATTR GpioChip::write(int pin, bool value)
	{
		auto level = value ? HIGH : LOW;
		gpio_set_level((gpio_num_t)pin, level);
	}

	bool IRAM_ATTR GpioChip::read(int pin) const
	{
		return gpio_get_level((gpio_num_t)pin) == 1U;
	}

	void GpioChip::setOpenDrain(int pin)
	{
		this->mode(pin, OUTPUT_OPEN_DRAIN);
		this->write(pin, true);
	}

	void IRAM_ATTR GpioChip::odWrite(int pin, bool value)
	{
		auto level = value ? HIGH : LOW;
		gpio_set_level((gpio_num_t)pin, level);
	}

	void GpioChip::attachIrqHandler(int pin, irq_handler_t handler, IrqEdge edge)
	{
		static bool interrupt_initialized = false;
    
		if(!interrupt_initialized) {
			interrupt_initialized = true;
			esp_intr_alloc(ETS_GPIO_INTR_SOURCE, (int)ESP_INTR_FLAG_IRAM,
				gpio_external_isr, NULL, &gpio_intr_handle);
		}

		gpio_isr_handlers[pin] = handler;
		esp_intr_disable(gpio_intr_handle);
		
		if(esp_intr_get_cpu(gpio_intr_handle)) {
			GPIO.pin[pin].int_ena = 1;
		} else { //PRO_CPU
			GPIO.pin[pin].int_ena = 4;
		}

		auto type = this->mapIrqType(edge);
		if(type == -1) {
			esp_intr_enable(gpio_intr_handle);
			return;
		}

		GPIO.pin[pin].int_type = type;
		esp_intr_enable(gpio_intr_handle);
	}

	int GpioChip::mapIrqType(const IrqEdge& edge) const
	{
		switch(edge) {
		case IrqRising:
			return RISING;

		case IrqFalling:
			return FALLING;

		case IrqRisingFalling:
			return CHANGE;

		case IrqNone:
			return DISABLED;

		default:
			return -1;
		}
	}
}

extern "C" void pinMode(int pin, int mode)
{
	switch(mode) {
	case RAW_INPUT:
		gpio.mode(pin, lwiot::PinMode::INPUT);
		break;

	case RAW_OUTPUT:
		gpio.mode(pin, lwiot::PinMode::OUTPUT);
		break;

	case RAW_OUTPUT_OPENDRAIN:
		gpio.mode(pin, lwiot::PinMode::OUTPUT_OPEN_DRAIN);
		break;
	}
}
}

static lwiot::esp32::GpioChip esp_gpio;
lwiot::GpioChip& gpio = esp_gpio;
