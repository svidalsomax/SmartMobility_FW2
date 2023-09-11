/*
 * ble_driver.c
 *
 *  Author: MWF
 */ 

#include "ble_driver.h"


static struct io_descriptor *ble_io;


static void tx_cb_USART_0(const struct usart_async_descriptor *const io_descr)
{
	/* Transfer completed */
	//usb_serial_write("Tx completed\n", strlen("Tx completed\n"));
}

static void rx_cb_USART_0(const struct usart_async_descriptor *const io_descr)
{
	/* Transfer completed */
	//usb_serial_write("Rx completed\n", strlen("Rx completed\n"));
}

int ble_init(void)
{
	usart_async_register_callback(&USART_0, USART_ASYNC_TXC_CB, tx_cb_USART_0);
	usart_async_register_callback(&USART_0, USART_ASYNC_RXC_CB, rx_cb_USART_0);
	//usart_async_register_callback(&USART_0, USART_ASYNC_ERROR_CB, err_cb);
	usart_async_get_io_descriptor(&USART_0, &ble_io);
	usart_async_enable(&USART_0);
	delay_ms(TIME_TO_DELAY);
	
	return 0;
}

void ble_send(char* command)
{
	char at_cmd[MAX_MESSAGE_LENGTH] = {0};
	strcpy(at_cmd, "");
	strcat(at_cmd, command);
	io_write(ble_io, (uint8_t *)at_cmd, strlen(at_cmd));
}

void ble_send_and_receive(char* command, char* response)
{
	char at_cmd[MAX_MESSAGE_LENGTH] = {0};
	strcpy(at_cmd, "");
	strcat(at_cmd, command);
	io_write(ble_io, (uint8_t *)at_cmd, strlen(at_cmd));
	delay_ms(TIME_TO_DELAY);
	io_read(ble_io, (uint8_t *)response, MAX_MESSAGE_LENGTH);
	delay_ms(TIME_TO_DELAY);
}


/******************************************************************************
**      FUNCIONES SOMAX
******************************************************************************/

uint32_t ble_millis_ = 0; 

void ble_process(Ble *ble){

	io_read(ble_io, (uint8_t *)ble->rxBuffer_, MAX_MESSAGE_LENGTH);
	char buffer[MAX_MESSAGE_LENGTH];
	
	switch ((*ble).bleState_) //definir el blestate_
	{
		case BLE_BEGIN:
		{
			strcpy(buffer, "AT");
			
			if (ble_timer(ble)){
				ble_retry(BLE_RENEW, ble, buffer);	
			}
			
			break;
		}
		case BLE_RENEW:
		{
			strcpy(buffer, "AT+RENEW");
			
			if (ble_timer(ble)){
				ble_retry(BLE_ROLE, ble, buffer);
			}
			
			break;
		}
		case BLE_ROLE:
		{
			strcpy(buffer, "AT+ROLE1");
			
			if (ble_timer(ble)){
				ble_retry(BLE_IMME, ble, buffer);
			}
			break;
			
		}
		case BLE_IMME:
		{
			strcpy(buffer, "AT+IMME1");
			
			if (ble_timer(ble)){
				ble_retry(BLE_RESET, ble, buffer);	
			}
			break;
			
		}
		case BLE_RESET:
		{
			strcpy(buffer, "AT+RESET");
			// if timer
			
			if (ble_timer(ble)){
				ble->tryCounter_ = (ble->tryCounter_ == 0) ? ble->tryCounter_+1:ble->tryCounter_ ;
				ble_retry(BLE_PROCESS, ble, buffer);				
			}
			break;

		}
		case BLE_PROCESS:
		{
			if(!ble->state_){
				strcpy(ble->txBuffer_,"AT+DISI?");
				ble->state_=1;
			}
			else{
				size_t size_rxBuffer_ = strlen(ble->rxBuffer_); 

				if(size_rxBuffer_ >= 8 && strcmp(ble->rxBuffer_ + (size_rxBuffer_ - 8), "OK+DISCE") == 0){
					//funcion parse de ble->rxBuffer_ --- patearla un poco
					ble->rxBuffer_[0] ='\0';
					ble->state_ = 0; 
				}

				if (_calendar_get_counter(&CALENDAR_0.device) - ble->scanTime_ > 3500){
					ble->state_ = 0;
				}	
			}
			break;
		}
		default: 
		{
			break; 			
		}
	}
	
	//si el txBuffer_ no está vacío, envíar el buffer
	if(!(ble->txBuffer_[0]=='\0')){  
		//ble_send_and_receive(ble->txBuffer_, ble->response_);
		io_write(ble_io, (uint8_t *)ble->txBuffer_, strlen(ble->txBuffer_));
		ble->scanTime_ =0;
	}

	//si el txBuffer_ está vacío y scanTime existe, actualizar scanTime_ del ble.
	if(ble->txBuffer_[0] == '\0' && !ble->scanTime_){
		//ble->scanTime_ = millis();
	}
}

ble_Scan ble_getScan(Ble *ble) {
	ble_process(ble);
	ble_Scan beaconScan = (*ble).scan_;
	clean_ble_Scan(&(*ble).scan_);
	return beaconScan;
}

void clean_ble_Scan(ble_Scan* scan){
	scan->scanTime_ = 0;
	for (int i = 0; i < sizeof(scan->beaconVector) / sizeof(scan->beaconVector[0]); i++) {
		scan->beaconVector[i].mac_ = 0;
		scan->beaconVector[i].rssi_ = 0;
	}
}

//esta función maldita está hxc -- incompleta
void parse(Ble *ble, char* block){
	/*ble->scan_.scanTime_ = ble->scanTime_;

	size_t size = strlen(block);
	if (size>16){
		size_t payload = size - 16;

		if(!(payload % 78)){
			//resizeBeaconVector(&(*ble).scan_, payload);
		}
	}*/
}

void ble_setName(char *name){
	char buffer[MAX_MESSAGE_LENGTH];
	strcpy(buffer, "AT+NAME");
	strcat(buffer, name);
	//uart_.write(&buffer); cambiar por io_write??
	//delay(TIME_TO_DELAY); esta linea sirve solo para MC Studio
}

bool ble_setBaud(unsigned long baud, Ble *ble){
	char buffer[MAX_MESSAGE_LENGTH];
	char answer[MAX_MESSAGE_LENGTH];
	switch (baud)
	{
	case 9600:
		strcpy(buffer, "AT+BAUD0");
		strcpy(answer, "Ok+Set:0");
		break;

	case 19200:
		strcpy(buffer, "AT+BAUD1");
		strcpy(answer, "Ok+Set:1");
		break;

	case 38400:
		strcpy(buffer, "AT+BAUD2");
		strcpy(answer, "Ok+Set:2");
		break;

	case 57600:
		strcpy(buffer, "AT+BAUD3");
		strcpy(answer, "Ok+Set:3");
		break;

	case 115200:
		strcpy(buffer, "AT+BAUD4");
		strcpy(answer, "Ok+Set:4");
		break;

	case 4800:
		strcpy(buffer, "AT+BAUD5");
		strcpy(answer, "Ok+Set:5");
		break;

	case 2400:
		strcpy(buffer, "AT+BAUD6");
		strcpy(answer, "Ok+Set:6");
		break;

	case 1200:
		strcpy(buffer, "AT+BAUD7");
		strcpy(answer, "Ok+Set:7");
		break;

	case 230400:
		strcpy(buffer, "AT+BAUD8");
		strcpy(answer, "Ok+Set:8");
		break;

	default:
		strcpy(buffer, "AT+BAUD0");
		strcpy(answer, "Ok+Set:0");
		break;
	}

	/*  TODO LO SIGUIENTE DEBERÍA PODER ENCAPSULARSE EN LA FUNCIÓN BLE_SEND_RESP DE LANEK      
	io_write buffer;
	delay(TIME_TO_DELAY);
	char rx_Buffer[MAX_MESSAGE_LENGTH] = io_read; */
	ble_send_and_receive(buffer, ble->response_);
	
	if (strcmp((*ble).response_, answer) == 0)
	{
		return true;
	} else {
		return false; 
	} 
}

bool ble_sleepMode(Ble *ble){ 
	char buffer[MAX_MESSAGE_LENGTH];
	strcpy(buffer, "AT+SLEEP");
	ble_send_and_receive(buffer, ble->response_);
	delay_ms(TIME_TO_DELAY);
	ble_send_and_receive(buffer, ble->response_);
	delay_ms(TIME_TO_DELAY);

	if (strcmp((*ble).response_, "OK+SLEEP") == 0)
	{
		return true;
	} else {
		return false;
	}
}

bool ble_wakeUp(Ble *ble){
	char buffer[MAX_MESSAGE_LENGTH];
	strcpy(buffer, "AAAAAAAAAAAAAAAAAAAAAAAAT");
	ble_send_and_receive(buffer, ble->response_);
	delay_ms(TIME_TO_DELAY);
	ble_send_and_receive(buffer, ble->response_);
	delay_ms(TIME_TO_DELAY);

	if (strcmp((*ble).response_, "OK+WAKE") == 0)
	{
		return true;
	} else {
		return false;
	}
}

bool ble_timer(Ble *ble)
{
	return ((*ble).time_ + (*ble).timer_ <  _calendar_get_counter(&CALENDAR_0.device));// some like millis()
}

void ble_setTimer(unsigned long t, Ble *ble){
	(*ble).time_ = _calendar_get_counter(&CALENDAR_0.device);// some like millis()
	(*ble).timer_ = t;
}

void ble_retry(bleStatus state, Ble *ble, char *buffer){

	if (ble->tryCounter_ < 2){
		io_write(ble_io, (uint8_t *)buffer, strlen(buffer));
		ble->tryCounter_ ++; 
		ble_setTimer(TIME_TO_DELAY, ble);
	} else {
		ble->bleState_ = state; 
		ble->tryCounter_ = 0; 
	}
}

