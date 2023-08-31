/*
 * imu_driver.c
 *
 *  Author: MWF
 */ 

#include "imu_driver.h"

#define ACC_address 0x18

void IMU_writeRegister (uint8_t reg_add, uint8_t reg_val);


int IMU_init(void)
{
	i2c_m_sync_get_io_descriptor(&I2C_0, &i2c_io);
	i2c_m_sync_enable(&I2C_0);
	i2c_m_sync_set_slaveaddr(&I2C_0, ACC_address, I2C_M_SEVEN);
	IMU_writeRegister(0x24, 0x80);	// reboot memory
	delay_ms(10);
	//IMU_writeRegister(0x20, 0x2F);	// 10 Hz - Low power mode - X Y Z enabled
	IMU_writeRegister(0x20, 0x5F);	// 100 Hz - Low power mode - X Y Z enabled
	IMU_writeRegister(0x21, 0x09);	// High pass filter on INT1 activity
	IMU_writeRegister(0x22, 0x40);	// IA1 interrupt on pin INT1
	IMU_writeRegister(0x23, 0x00);	// FS = +-2g
	IMU_writeRegister(0x24, 0x08);	// INT1 latched
	IMU_writeRegister(0x32, 0x10);	// INT1 Threshold: 1b = 16mg, 0x64 = 1.6 g
	IMU_writeRegister(0x33, 0x00);	// INT1 Duration: 1b = 1/ODR, 0x01 = 0.1 s (irrelevant bc latch)
	IMU_readRegister(0x26);			// read to set Reference
	IMU_writeRegister(0x30, 0x2a);	// INT1 X Y Z high enable
	delay_ms(10);
	
	return;
}

/*---------------------------------------------------------------------------*/
void IMU_writeRegister(uint8_t reg_add, uint8_t reg_val)
{
	i2c_m_sync_set_slaveaddr(&I2C_0, ACC_address, I2C_M_SEVEN);
	uint8_t buff[2] = { reg_add , reg_val};
	io_write( i2c_io , buff, 2 );
	
	//uint8_t buff[1] = {reg_val};
	//i2c_m_sync_cmd_write(&I2C_0, reg_add, buff, 1);
	
	return;
}

/*---------------------------------------------------------------------------*/
uint8_t IMU_readRegister(uint8_t reg_add)
{
	uint8_t buff;
	i2c_m_sync_set_slaveaddr(&I2C_0, ACC_address, I2C_M_SEVEN);
	i2c_m_sync_cmd_read(&I2C_0, reg_add, &buff, 1);
	
	return buff;
}

/*---------------------------------------------------------------------------*/
bool IMU_secureWrite(uint8_t reg_add, uint8_t reg_val)
{
	for (uint8_t attempts = 0; attempts < 3; attempts--)
	{
		IMU_writeRegister(reg_add, reg_val);
		uint8_t status = IMU_readRegister(reg_add);
		if (status == reg_val)
		{
			return true;
		}
		delay_us(1 + 100 * attempts * attempts);
	}

	return false;
}

