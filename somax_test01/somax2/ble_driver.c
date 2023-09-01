/*
 * ble_driver.c
 *
 *  Author: MWF
 */ 

#include "ble_driver.h"

#define MAX_MESSAGE_LENGTH 64	// Cambiar tambien USART_0_BUFFER_SIZE en driver_init.c
#define TIME_TO_DELAY 20

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
	io_read(ble_io, response, MAX_MESSAGE_LENGTH);
	delay_ms(TIME_TO_DELAY);
}


/******************************************************************************
**      FUNCIONES SOMAX
******************************************************************************/

void ble_process(Ble *ble){
	//strcat((*ble).rxBuffer_, "hola"); //rxBuffer debería leer lo del UART.
	char buffer[MAX_MESSAGE_LENGTH];
	switch ((*ble).bleState_) //definir el blestate_
	{
		case BLE_BEGIN:
		{
			//definir buffer AT
			strcpy(buffer, "AT");
			//printf(buffer);
			ble_retry(BLE_RENEW, ble, buffer);
			break;
		}
		case BLE_RENEW:
		{
			//definir algo para ver el estado del state
			strcpy(buffer, "AT");
			//printf(buffer);
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
	ble_send_and_receive(buffer,);
	//command para enviar comando por uart
}

void ble_set_timer(unsigned long t, Ble *ble){
	(*ble).timer_ = t;
	//(*ble).time =  ;
}