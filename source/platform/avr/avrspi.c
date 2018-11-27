/*
 * AVR SPI implementation.
 * 
 * @author Michel Megens
 * @email  dev@bietje.net
 */

#include <stdlib.h>
#include <stdio.h>
#include <lwiot.h>

#include <lwiot/types.h>
#include <lwiot/error.h>

#include <avr/io.h>

typedef enum spi_ctrl {
	SPI_MODE0, //!< CPOL:0 CPHA:0
	SPI_MODE1, //!< CPOL:0 CPHA:1
	SPI_MODE2, //!< CPOL:1 CPHA:0
	SPI_MODE3, //!< CPOL:1 CPHA:1
	SPI_SET_SPEED, //!< Set the SPI bitrate.
	SPI_2X, //!< Enable SPI double bitrate (if supported).
} spi_ctrl_t;

#define SPI_CPHA_FLAG      0 //!< Clock phase selector.
#define SPI_CPOL_FLAG      1 //!< Clock polarity selector.
#define SPI_CS_HIGH_FLAG   2 //!< CS active on HIGH.
#define SPI_2X_FLAG        3 //!< SPI double speed flag.

#define SPI_MODE0_MASK 0
#define SPI_MODE1_MASK (0 | (1 << SPI_CPHA_FLAG))
#define SPI_MODE2_MASK ((1 << SPI_CPOL_FLAG) | 0)
#define SPI_MODE3_MASK ((1<<SPI_CPOL_FLAG) | (1<<SPI_CPHA_FLAG))

static lwiot_mutex_t *master_xfer_mutex;
static unsigned long flags;

static unsigned char spi_clk_div[] = {
	4,
	16,
	64,
	128,
	2,
	8,
	32,
	64,
};

int atmega_spi_setspeed(uint32_t rate)
{
	unsigned char div = 0;
	unsigned char spcr;

	lwiot_mutex_lock(master_xfer_mutex, FOREVER);
	for(; div <= 7; div++) {
		if((F_CPU / spi_clk_div[div]) == rate)
			break;
	}

	if(div > 7) {
		lwiot_mutex_unlock(master_xfer_mutex);
		return -EINVALID;
	}

	if(div > 3) {
		/* enable spi2x */
		flags |= 1 << SPI_2X_FLAG;
		SPSR |= SPI2X;
	} else {
		SPSR &= 0;
		flags &= ~(1 << SPI_2X_FLAG);
	}
	
	spcr = SPCR;
	spcr &= ~0x3;
	spcr |= div | 0x3;
	SPCR = spcr;

	lwiot_mutex_unlock(master_xfer_mutex);
	return -EOK;
}

int atmega_spi_xfer(const uint8_t *tx, uint8_t *rx, size_t length)
{
	size_t len = length;

	lwiot_mutex_lock(master_xfer_mutex, FOREVER);
	SPDR = tx[0];
	while(--len > 0UL) {
		while(!(SPSR & _BV(SPIF)));
		*rx++ = SPDR;
		SPDR = *tx++;
	}

	while(!(SPSR & _BV(SPIF)));
	*rx = SPDR;

	lwiot_mutex_unlock(master_xfer_mutex);
	return length;
}

void atmega_spi_init(void)
{
	/* software initialisation */
	master_xfer_mutex = lwiot_mutex_create(0);
	SPCR |= _BV(SPR1);
	SPCR |= _BV(MSTR);
	SPCR |= _BV(SPE);
}

void atmega_spi_destroy(void)
{
	SPCR &= ~_BV(SPR1);
	SPCR &= ~_BV(MSTR);
	SPCR &= ~_BV(SPE);
	lwiot_mutex_destroy(master_xfer_mutex);
}
