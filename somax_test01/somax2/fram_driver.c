/*
 * fram_driver.c
 *
 *  Author: MWF
 */ 


#include "fram_driver.h"

#define FRAM_address 0x50

//void FRAM_writeRegister (uint8_t reg_add, uint8_t reg_val);


/**
 * \brief Sync version of i2c write command for bytes
 */
int32_t i2c_m_sync_byte_write(struct i2c_m_sync_desc *i2c, uint8_t *buffer, uint8_t length)
{
	struct _i2c_m_msg msg;
	int32_t           ret;

	msg.addr   = i2c->slave_addr;
	msg.len    = length;
	msg.flags  = I2C_M_STOP;
	msg.buffer = buffer;

	ret = _i2c_m_sync_transfer(&i2c->device, &msg);

	if (ret != 0) {
		/* error occurred */
		return ret;
	}

	return ERR_NONE;
}

/**
 * \brief Sync version of i2c write command for bytes
 */
int32_t i2c_m_sync_byte_write_nostop(struct i2c_m_sync_desc *i2c, uint8_t *buffer, uint8_t length)
{
	struct _i2c_m_msg msg;
	int32_t           ret;

	msg.addr   = i2c->slave_addr;
	msg.len    = length;
	msg.flags  = 0;
	msg.buffer = buffer;

	ret = _i2c_m_sync_transfer(&i2c->device, &msg);

	if (ret != 0) {
		/* error occurred */
		return ret;
	}

	return ERR_NONE;
}

/**
 * \brief Sync version of i2c read command for bytes
 */
int32_t i2c_m_sync_byte_read(struct i2c_m_sync_desc *i2c, uint8_t *buffer, uint8_t length)
{
	struct _i2c_m_msg msg;
	int32_t           ret;

	msg.addr   = i2c->slave_addr;
	msg.len    = length;
	msg.flags  = I2C_M_STOP | I2C_M_RD;
	msg.buffer = buffer;

	ret = _i2c_m_sync_transfer(&i2c->device, &msg);

	if (ret != 0) {
		/* error occurred */
		return ret;
	}

	return ERR_NONE;
}


int FRAM_init(void)
{
	//i2c_m_sync_get_io_descriptor(&I2C_0, &fram_io);
	i2c_m_sync_enable(&I2C_0);
	i2c_m_sync_set_slaveaddr(&I2C_0, FRAM_address, I2C_M_SEVEN);
	
	return;
}


/*---------------------------------------------------------------------------*/
void FRAM_bytewrite(uint16_t addr, uint8_t* data)
{
	i2c_m_sync_set_slaveaddr(&I2C_0, FRAM_address, I2C_M_SEVEN);
	//uint8_t buff[2] = { reg_add , reg_val};
	//io_write( i2c_io , buff, 2 );
	uint8_t add_low = ((uint16_t)addr >> 0) & 0xFF;
	uint8_t add_high = ((uint16_t)addr >> 8) & 0xFF;
	uint8_t databuf = *data;
	uint8_t buff[3] = {add_high, add_low, databuf};

	i2c_m_sync_byte_write(&I2C_0, buff, 3);
	
	return;
}

void FRAM_byteread(uint16_t addr, uint8_t* data)
{
	i2c_m_sync_set_slaveaddr(&I2C_0, FRAM_address, I2C_M_SEVEN);
	//uint8_t buff[2] = { reg_add , reg_val};
	//io_write( i2c_io , buff, 2 );
	//i2c_m_sync_cmd_write(&I2C_0, reg_add, buff, 1);
	uint8_t add_low = ((uint16_t)addr >> 0) & 0xFF;
	uint8_t add_high = ((uint16_t)addr >> 8) & 0xFF;
	uint8_t buff[2] = {add_high, add_low};
	
	//i2c_m_sync_write(fram_io, buff, 3);
	//i2c_m_sync_read(fram_io, data, 1);
	i2c_m_sync_byte_write_nostop(&I2C_0, buff, 2);
	i2c_m_sync_byte_read(&I2C_0, data, 1);
	
	return;
}

void FRAM_currentbyteread(uint16_t addr, uint8_t* data)
{
	i2c_m_sync_set_slaveaddr(&I2C_0, FRAM_address, I2C_M_SEVEN);
	//uint8_t buff[2] = { reg_add , reg_val};
	//io_write( i2c_io , buff, 2 );
	//i2c_m_sync_cmd_write(&I2C_0, reg_add, buff, 1);

	i2c_m_sync_byte_read(&I2C_0, data, 1);
	
	return;
}

/*---------------------------------------------------------------------------*/
uint8_t FRAM_readRegister(uint8_t reg_add)
{
	uint8_t buff;
	i2c_m_sync_set_slaveaddr(&I2C_0, FRAM_address, I2C_M_SEVEN);
	i2c_m_sync_cmd_read(&I2C_0, reg_add, &buff, 1);
	
	return buff;
}

/*---------------------------------------------------------------------------*/
bool FRAM_secureWrite(uint8_t reg_add, uint8_t reg_val)
{
	for (uint8_t attempts = 0; attempts < 3; attempts--)
	{
		FRAM_writeRegister(reg_add, reg_val);
		uint8_t status = FRAM_readRegister(reg_add);
		if (status == reg_val)
		{
			return true;
		}
		delay_us(1 + 100 * attempts * attempts);
	}

	return false;
}

/*
void IMU_readAcc(uint8_t* buff)
{
	i2c_m_sync_set_slaveaddr(&I2C_0, ACC_address, I2C_M_SEVEN);
	i2c_m_sync_cmd_read(&I2C_0, 0x28, buff, 6);
	delay_us(5);
	
	return;
}

void IMU_readStatus(uint8_t* buff)
{
	i2c_m_sync_set_slaveaddr(&I2C_0, ACC_address, I2C_M_SEVEN);
	i2c_m_sync_cmd_read(&I2C_0, 0x1e, buff, 1);
	delay_us(5);
	
	return;
}
*/