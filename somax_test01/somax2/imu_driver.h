/*
 * imu_driver.h
 *
 *  Author: MWF
 */ 

#ifndef IMU_H_
#define IMU_H_
/******************************************************************************
**      INCLUDES
******************************************************************************/
#include <atmel_start_pins.h>
#include <hal_gpio.h>
#include <hal_i2c_m_sync.h>
#include <driver_init.h>


static struct io_descriptor *i2c_io;

void IMU_writeRegister (uint8_t reg_add, uint8_t reg_val);
int IMU_init(void);
uint8_t IMU_readRegister(uint8_t reg_add);
bool IMU_secureWrite(uint8_t reg_add, uint8_t reg_val);

#endif