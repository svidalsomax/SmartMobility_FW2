/*
 * simcom_driver.c
 *
 *  Author: MWF
 */ 

#include "simcom_driver.h"

#include <ctype.h>
#include <stdlib.h>

#define MAX_MESSAGE_LENGTH 64 // Cambiar tambien USART_1_BUFFER_SIZE en driver_init.c
#define TIME_TO_DELAY 20

static struct io_descriptor *simcom_io;


/******************************************************************************
**     FUNCIONES LANEK
******************************************************************************/
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
	
	return 1;
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

/******************************************************************************
**     VARIABLES SOMAX 
******************************************************************************/
const char empty[] = "";
const char blank[] = "\r\n";
const char ok[] = "OK\r\n";
const char error[] = "ERROR\r\n";
const char cmeError[] = "+CME ERROR: ";
const char cmsError[] = "+CMS ERROR: ";
const char start[] = "START\r\n";
const char pbDone[] = "PB DONE\r\n";
const char gpsinfo[] = "+CGPSINFO:";
const char simei[] = "+SIMEI: ";
const char netopen[] = "+NETOPEN: ";
const char ipaddr[] = "+IPADDR: ";
const char cipopen[] = "+CIPOPEN: ";
const char cipsend[] = "+CIPSEND: ";
const char cipstat[] = "+CIPSTAT: ";
const char recvFrom[] = "RECV FROM:";
const char ciperror[] = "+CIPERROR: ";
const char ipclose[] = "+IPCLOSE: ";
const char cipclose[] = "+CIPCLOSE: ";
const char netclose[] = "+NETCLOSE: ";

// Funci�n para extraer n�meros enteros separados por comas
void getIntegers(const char *buffer, int *integers, int *count) {
	*count = 0;
	bool save = true;

	for (int index = 0; buffer[index] != '\0'; index++) {
		if (save) {
			integers[*count] = atoi(buffer + index);
			(*count)++;
			save = false;
		}

		char c = buffer[index];

		if (c == ',') {
			save = true;
			} else if (!isdigit(c)) {
			*count = 0; // Reiniciar el conteo si se encuentra un car�cter no v�lido
			return;
		}
	}
}

void Simcom_init(Simcom * simcom ){
	simcom->state_ = simcom_state_cancel;
	simcom->otaMode_ = false;
	simcom->attempt_ = 0;
	simcom->connection_ = 0;
	simcom->startDelay_ = 30000;
	simcom->lastRequest_ = 0;
	simcom->lastError_ = 0;
	simcom->lastReset_ = 0;
	simcom->lastOnline_ = 0;
	simcom->lastTransmission_ = 0;
	simcom->remoteServerDisconnectionCount_ = 1;
	simcom->networkServerDisconnectionCount_ = 0;
	simcom->timer_ = 0;
	simcom->netTimeout_ = 60000;
	simcom->openTimeout_ = 60000;
	simcom->sendTimeout_ = 90000;
	simcom->serverPort_ = 0;
	simcom->ipaddr_ = "0.0.0.0";
	simcom->positionDelay_ = 10000;
	simcom->tcpTxBlock_ = 0;
	simcom->tDataRxSize_ = 0;
	simcom->tDataTxSize_ = 0;
}

void Simcom_setStartDelay(Simcom * simcom, unsigned long startDelay){
	simcom->startDelay_ = startDelay;
}

void Simcom_setApn(Simcom * simcom, char * apn, char * user, char * password){
	simcom->apn_ = apn;
	simcom->user_ = user;
	simcom->password_ = password;
}

void Simcom_setTimeouts(Simcom * simcom, unsigned long netTimeout, unsigned long openTimeout, unsigned long sendTimeout){
	simcom->netTimeout_  = netTimeout;
	simcom->openTimeout_ = openTimeout;
	simcom->sendTimeout_ = sendTimeout;
}

void Simcom_setServer(Simcom * simcom, char * serverIp, unsigned serverPort){
	simcom->serverIp_   = serverIp;
	simcom->serverPort_ = serverPort;
}

void Simcom_setPositionDelay(Simcom * simcom, unsigned positionDelay){
	simcom->positionDelay_ = positionDelay;
}


Simcom_State Simcom_state(Simcom * simcom){
	return simcom->state_;
}

unsigned Simcom_connection(Simcom * simcom){
	return simcom->connection_;
}

unsigned long Simcom_lastReset(Simcom * simcom){
	return simcom->lastReset_;
}

unsigned long Simcom_lastOnline(Simcom * simcom){
	return simcom->lastOnline_;
}


unsigned long Simcom_lastTransmission(Simcom * simcom){
	return simcom->lastTransmission_;
}

Imei Simcom_imei(Simcom * simcom){
	return simcom->imei_;
}

PositionRecord Simcom_positionRecord(Simcom * simcom){
	return simcom->positionRecord_;
}

unsigned long Simcom_tDataRxSize(Simcom * simcom){
	return simcom->tDataRxSize_;
}

unsigned long Simcom_tDataTxSize(Simcom * simcom){
	return simcom->tDataTxSize_;
}

void Simcom_closeConnection(Simcom * simcom){
	simcom->disconectionFlag_ = true;
}

void Simcom_setConnection(Simcom * simcom){
	simcom->disconectionFlag_ = false;
}


char * Simcom_IPaddress(Simcom * simcom){
	return simcom->ipaddr_;
}

void Simcom_setOtaMode(Simcom * simcom){
	simcom->otaMode_ = true;
}

void Simcom_clearOtaMode(Simcom * simcom){
	simcom->otaMode_ = false;
}

void Simcom_process(Simcom * simcom){
    //printf("entro al process \n");
    char * buffer; 
	simcom_receive(buffer);
	strcat(simcom->rxBuffer_ , buffer);
	
    //printf("se asigno buffer del process: %s \n", buffer);
    //
    //printf("despues de strcat, rxBuffer_ : %s \n", simcom->rxBuffer_);

    #ifdef DEBUG_SIM
        usb_serialPrint(buffer); 
    #endif

    switch (simcom->state_)
    {
    case simcom_state_cancel:
        
        // FALTAN FUNCIONES A�N PARA QUE FUNCINE EL PROCESS DE SIMCOM
        /*
        request("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAT\r", 30, 200, 0);
        setTimer(startDelay_);
        processResponse(State::creset);*/
        break;
    
    default:
        break;
    }

}


void Simcom_reset(Simcom * simcom){
	//se hace un clear de tcpTxBuffer y se pasa a state cancel
	simcom->tcpTxBuffer_[0] = '\0';
	Simcom_nextState(simcom,simcom_state_cancel);
}


void Simcom_netclose(Simcom * simcom){
	//txBuffer_.clear();
	Simcom_setTimer(simcom, 90000);
	Simcom_nextState(simcom, simcom_state_cipstat);
}


void Simcom_tcpSend(Simcom * simcom, char * buffer){
	strcat(simcom->tcpRxBuffer_,buffer);
}

char * Simcom_tcpReceive(Simcom * simcom){
	char * buffer = simcom->tcpRxBuffer_;
	simcom->tcpRxBuffer_[0] = '\0';
	return buffer;
}

char * Simcom_tcpTxBuffer(Simcom * simcom){
	return simcom->tcpTxBuffer_;
}

char * Simcom_tcpRxBuffer(Simcom * simcom){
	return simcom->tcpRxBuffer_;
}

void Simcom_request(Simcom * simcom, char * command, unsigned maxAttempt, unsigned long wait, unsigned long errorWait){
	// unsigned long t = millis(); cambiar millis por calendar de microchip
	unsigned long t = 123456789;

	if(!simcom->attempt_ || (simcom->lastRequest_ && (t - simcom->lastRequest_ > wait)) || (simcom->lastError_ && (t - simcom->lastError_ > errorWait))){
		if(simcom->attempt_ < maxAttempt){
			simcom->attempt_++;
			simcom->lastError_ = 0;
			strcpy(simcom->txBuffer_,command);
		}
		else {
			//throw exception(command)
		}
	}

}


void Simcom_nextState(Simcom * simcom, Simcom_State state){
	simcom->state_ = state;
	simcom->lastRequest_ = 0;
	simcom->lastError_ = 0;
	simcom->attempt_ = 0;
}



//-------------se van rellenando funciones por ac�?? 



void Simcom_setTimer(Simcom * simcom, unsigned long wait){
	// simcom->timer_ = millis() + wait; cambiar el millis por la funci�n de microchip
}

bool Simcom_timer(Simcom * simcom){
	// return millis() > simcom->timer_; cambiar millis por la funci�n de microchip
	return false;
}
