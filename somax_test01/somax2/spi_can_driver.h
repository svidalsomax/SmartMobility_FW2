/*
 * spi_can_driver.h
 *
 *  Author: MWF
 */ 


#ifndef SPI_CAN_DRIVER_H_
#define SPI_CAN_DRIVER_H_

/******************************************************************************
**      INCLUDES
******************************************************************************/
#include <atmel_start_pins.h>
#include <hal_gpio.h>
#include <driver_init.h>

void spi_init(void);
void spi_write_reg(uint8_t reg, uint8_t value);
void spi_read_reg(uint8_t reg, uint8_t* value);

#endif /* SPI_CAN_DRIVER_H_ */