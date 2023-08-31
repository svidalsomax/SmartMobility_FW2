/*
 * simcom_driver.c
 *
 *  Author: MWF
 */ 

#include "simcom_driver.h"

#define MAX_MESSAGE_LENGTH 64 // Cambiar tambien USART_1_BUFFER_SIZE en driver_init.c
#define TIME_TO_DELAY 20

static struct io_descriptor *simcom_io;


static void tx_cb_USART_1(const struct usart_async_descriptor *const io_descr)
{
	/* Transfer completed */
	usb_serial_write("Tx completed\n", strlen("Tx completed\n"));
}

static void rx_cb_USART_1(const struct usart_async_descriptor *const io_descr)
{
	/* Transfer completed */
	usb_serial_write("Rx completed\n", strlen("Rx completed\n"));
}

int simcom_init(void)
{
	usart_async_register_callback(&USART_1, USART_ASYNC_TXC_CB, tx_cb_USART_1);
	usart_async_register_callback(&USART_1, USART_ASYNC_RXC_CB, rx_cb_USART_1);
	//usart_async_register_callback(&USART_1, USART_ASYNC_ERROR_CB, err_cb);
	usart_async_get_io_descriptor(&USART_1, &simcom_io);
	usart_async_enable(&USART_1);
	delay_ms(TIME_TO_DELAY);
	
	return;
}

void simcom_send(char* command)
{
	char at_cmd[MAX_MESSAGE_LENGTH] = {0};
	strcpy(at_cmd, "");
	strcat(at_cmd, command);
	io_write(simcom_io, (uint8_t *)at_cmd, strlen(at_cmd));
}

void simcom_send_and_receive(char* command, char* response)
{
	char at_cmd[MAX_MESSAGE_LENGTH] = {0};
	strcpy(at_cmd, "");
	strcat(at_cmd, command);
	io_write(simcom_io, (uint8_t *)at_cmd, strlen(at_cmd));
	delay_ms(TIME_TO_DELAY);
	io_read(simcom_io, response, MAX_MESSAGE_LENGTH);
	delay_ms(TIME_TO_DELAY);
}

void simcom_receive(char* response)
{
	io_read(simcom_io, response, MAX_MESSAGE_LENGTH);
	delay_ms(TIME_TO_DELAY);
}