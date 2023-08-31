/*
 * ble_driver.h
 *
 *  Author: MWF
 */ 


#ifndef BLE_DRIVER_H_
#define BLE_DRIVER_H_

/******************************************************************************
**      INCLUDES
******************************************************************************/
#include <atmel_start_pins.h>
#include <hal_gpio.h>
#include <hal_usart_async.h>
#include <driver_init.h>
#include <string.h>


int ble_init(void);
void ble_send(char* command);
void ble_send_and_receive(char* command, char* response);

#endif /* BLE_DRIVER_H_ */