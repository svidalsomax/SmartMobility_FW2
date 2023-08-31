/*
 * spi_can_driver.c
 *
 *  Author: MWF
 */ 

#include "spi_can_driver.h"

static struct io_descriptor *spi_io;

void spi_init(void)
{
	spi_m_sync_get_io_descriptor(&SPI_0, &spi_io);
	spi_m_sync_enable(&SPI_0);
};

void spi_write_reg(uint8_t reg, uint8_t value)
{
	uint8_t buffer[3] = {0x02, reg, value};
	gpio_set_pin_level(SPI_CS0, false);
	io_write(spi_io, buffer, 3);
	gpio_set_pin_level(SPI_CS0, true);
}

void spi_read_reg(uint8_t reg, uint8_t* value)
{
	uint8_t buffer[2] = {0x03, reg};
	gpio_set_pin_level(SPI_CS0, false);
	io_write(spi_io, buffer, 2);
	io_read(spi_io, value, 1);
	gpio_set_pin_level(SPI_CS0, true);
}

