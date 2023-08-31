/*
 * fram_driver.h
 *
 *  Author: MWF
 */ 


#ifndef FRAM_DRIVER_H_
#define FRAM_DRIVER_H_
/******************************************************************************
**      INCLUDES
******************************************************************************/
#include <atmel_start_pins.h>
#include <hal_gpio.h>
#include <hal_i2c_m_sync.h>
#include <driver_init.h>


static struct io_descriptor *fram_io;

//void FRAM_writeRegister (uint8_t reg_add, uint8_t reg_val);
int FRAM_init(void);
void FRAM_bytewrite(uint16_t addr, uint8_t* data);
void FRAM_byteread(uint16_t addr, uint8_t* data);
void FRAM_currentbyteread(uint16_t addr, uint8_t* data);
uint8_t FRAM_readRegister(uint8_t reg_add);
bool FRAM_secureWrite(uint8_t reg_add, uint8_t reg_val);
//void IMU_readAcc(uint8_t* buff);
//void IMU_readStatus(uint8_t* buff);


#endif /* FRAM_DRIVER_H_ */