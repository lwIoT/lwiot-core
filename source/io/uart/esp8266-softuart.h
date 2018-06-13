/*
 * Private header file for the ESP8266 software UART.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#pragma once

#include <lwiot.h>

CDECL
extern bool esp_softuart_open(uint8_t uart_no, uint32_t baudrate, uint8_t rx_pin, uint8_t tx_pin);
extern bool esp_softuart_close(uint8_t uart_no);
extern bool esp_softuart_put(uint8_t uart_no, char c);
extern bool esp_softuart_puts(uint8_t uart_no, const char *s);
extern bool esp_softuart_available(uint8_t uart_no);
extern uint8_t esp_softuart_read(uint8_t uart_no);
CDECL_END
