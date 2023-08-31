#include "ble.h"

void ble_process(Ble *ble){
	strcat((*ble).rxBuffer_, "hola"); //rxBuffer debería leer lo del UART. 
	char buffer[MAX_MESSAGE_LENGTH]; 
	switch ((*ble).bleState_) //definir el blestate_ 
	{
		case BLE_BEGIN:
		{
			//definir buffer AT
			strcpy(buffer, "BLE_BEGIN");
            printf(buffer);
            ble_retry(BLE_RENEW, ble, buffer); 
			break;	
		}
		case BLE_RENEW:
		{
			//definir algo para ver el estado del state
			strcpy(buffer, "AT_RENEW");
            printf(buffer);
            ble_retry(BLE_BEGIN, ble, buffer);
			break; 
		}		
		default: 
		{
			break; 			
		}
	}
}


void ble_retry(bleStatus state, Ble *ble, char *buffer){
	(*ble).bleState_ = state; 
    //strcpy((*ble).state_, buffer);
    //command para enviar comando por uart
}