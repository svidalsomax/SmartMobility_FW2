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

/******************************************************************************
**      VARIABLES SOMAX - BLE
******************************************************************************/

typedef enum {
	BLE_BEGIN,
	BLE_RENEW,
	BLE_ROLE,
	BLE_IMME,
	BLE_RESET,
	BLE_PROCESS,
}bleStatus;

typedef struct{
	char txBuffer_[64];
	char rxBuffer_[64];
	bleStatus bleState_;
	unsigned long timer_;
	unsigned long time_;
	int tryCounter_;
	char state_[64];
	bool role_ ;
	int scanTime_;
}Ble;



/******************************************************************************
**      FUNCIONES LANEK
******************************************************************************/


int ble_init(void);
void ble_send(char* command);
void ble_send_and_receive(char* command, char* response);

/******************************************************************************
**    FUNCIONES SOMAX
******************************************************************************/
void ble_process(Ble *ble);
void ble_retry(bleStatus state, Ble *ble, char *buffer);
void ble_set_timer(unsigned long t, Ble *ble);

#endif /* BLE_DRIVER_H_ */