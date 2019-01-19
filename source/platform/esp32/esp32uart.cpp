/*
 * ESP32 UART.
 *
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <lwiot.h>

#include <lwiot/io/gpiopin.h>
#include <lwiot/io/gpiochip.h>
#include <lwiot/io/uart.h>
#include <lwiot/io/gpiopin.h>
#include <lwiot/esp32/esp32uart.h>

#define UART_BUFFER_SIZE 512
#define FLOW_CTL_THRESH 128

namespace lwiot { namespace esp32
{
	Uart::Uart(int num, long baud, uint32_t config) : lwiot::Uart(0, 0, baud, config),
		_uart_num((uart_port_t)num)
	{
		switch(num) {
		case 0:
			this->_tx = 1;
			this->_rx = 3;
			break;

		case 1:
			this->_tx = 10;
			this->_rx =  9;
			break;

		case 2:
			this->_tx = 17;
			this->_rx = 16;
			break;

		default:
			break;
		}

		switch(config) {
		case SERIAL_7N1:
			this->_setup.data_bits = UART_DATA_7_BITS;
			break;

		case SERIAL_8N1:
			this->_setup.data_bits = UART_DATA_8_BITS;
			break;

		default:
			break;
		}

		this->_setup.baud_rate = baud;
		this->_setup.stop_bits = UART_STOP_BITS_1;
		this->_setup.parity = UART_PARITY_DISABLE;
		this->_setup.flow_ctrl = UART_HW_FLOWCTRL_DISABLE;
		this->_setup.rx_flow_ctrl_thresh = UART_FIFO_LEN - 1;

		uart_param_config(_uart_num, &this->_setup);
		uart_set_pin(_uart_num, this->_tx.pin(), this->_rx.pin(), UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
		uart_driver_install(_uart_num, UART_BUFFER_SIZE * 2, 0, 0, NULL, 0);
	}

	Uart::~Uart()
	{
		uart_driver_delete(this->_uart_num);
	}

	size_t Uart::available() const
	{
		size_t retval;

		retval = 0;
		if(unlikely(uart_get_buffered_data_len(this->_uart_num, &retval)) != ESP_OK)
			return 0;

		return retval;
	}

	bool Uart::write(uint8_t byte)
	{
		return this->write(&byte, sizeof(byte)) == sizeof(uint8_t);
	}

	ssize_t Uart::write(const void *bytes, const size_t& length)
	{
		return uart_write_bytes(this->_uart_num, (const char*)bytes, length);
	}

	ssize_t Uart::read(void *buffer, const size_t& length)
	{
		int len;

		len = uart_read_bytes(this->_uart_num, static_cast<uint8_t*>(buffer), length, portMAX_DELAY);
		return static_cast<ssize_t>(len);
	}

	uint8_t Uart::read()
	{
		uint8_t retval;
		int len;

		retval = 0;
		len = uart_read_bytes(this->_uart_num, &retval, sizeof(retval), portMAX_DELAY);

		return len == 1 ? retval : 0;
	}
}
}
