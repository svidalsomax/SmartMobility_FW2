/*
 * simcom_driver.h
 *
 *  Author: MWF
 */ 


#ifndef SIMCOM_DRIVER_H_
#define SIMCOM_DRIVER_H_

/******************************************************************************
**      INCLUDES
******************************************************************************/
#include <atmel_start_pins.h>
#include <hal_gpio.h>
#include <hal_usart_async.h>
#include <driver_init.h>
#include <string.h>


int simcom_init(void);
void simcom_send(char* command);
void simcom_send_and_receive(char* command, char* response);
void simcom_receive(char* response);

#endif /* SIMCOM_DRIVER_H_ */