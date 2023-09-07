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


#define MAX_MESSAGE_LENGTH 64	// Cambiar tambien USART_0_BUFFER_SIZE en driver_init.c
#define TIME_TO_DELAY 20
#define TIME_TO_DELAY_BLE 750

typedef enum {
	BLE_BEGIN,
	BLE_RENEW,
	BLE_ROLE,
	BLE_IMME,
	BLE_RESET,
	BLE_PROCESS,
}bleStatus;

//Beacon - used to create Beacon Vector initialized and used throght ble_Scan struct.
typedef struct {
	unsigned long long mac_;
	int rssi_;
} Beacon;

//ble_Scan - used for ...
typedef struct{
	unsigned long scanTime_;
	Beacon *beaconVector;
}ble_Scan;


typedef struct{
	char txBuffer_[MAX_MESSAGE_LENGTH];
	char rxBuffer_[MAX_MESSAGE_LENGTH];
	bleStatus bleState_;
	unsigned long timer_;
	unsigned long time_;
	int tryCounter_;
	char state_;
	bool role_ ;
	int scanTime_;
	ble_Scan scan_;
	char response_[MAX_MESSAGE_LENGTH];
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
/*
// ---- Función que echa a andar la máquina de estados. 
*/
void ble_process(Ble *ble);

/*
// ---- Función que añade los elementos escaneados al beaconvector. Falta terminanrla.
*/
void parse(Ble *ble, char* block);

/*
// ---- Se añaden los escaneos del Ble al vector beaconScan y se borra la info de scan del Ble
*/
ble_Scan ble_getSecan(Ble *ble);

/*
// ---- Limpia el vector de beacons y el escaneo
*/
void clean_ble_Scan(ble_Scan* scan);

/*
// ---- ble_setName: Setea nombre del Ble
*/
void ble_setName(char *name);

/*
// ---- Setea baudios de comuniación del Ble
*/
bool ble_setBaud(unsigned long baud, Ble *ble);

/*
// ---- Manda el ble a dormir por comando AT
*/
bool ble_sleepMode(Ble *ble);

/*
// ---- Despierta al ble por comando AT
*/
bool ble_wakeUp(Ble *ble);

/*
// ---- Indica si está listo el timer
*/
bool ble_timer(Ble *ble);

/*
// ---- Envía comando AT del ble definido antes
*/
void ble_retry(bleStatus state, Ble *ble, char *buffer);

/*
// ---- Setea tiempo a comparar para triggerear timer
*/
void ble_set_timer(unsigned long t, Ble *ble);

#endif /* BLE_DRIVER_H_ */