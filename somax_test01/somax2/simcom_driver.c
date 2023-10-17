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
	
	//usb_serial_write("Tx completed\n", strlen("Tx completed\n"));
}

static void rx_cb_USART_1(const struct usart_async_descriptor *const io_descr)
{
	/* Transfer completed */
	//usb_serial_write("Rx completed\n", strlen("Rx completed\n"));
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

// Función para extraer números enteros separados por comas
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
			*count = 0; // Reiniciar el conteo si se encuentra un carácter no válido
			return;
		}
	}
}

void Simcom_struct_init(Simcom * simcom ){
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
    char buffer[64] = {0}; 
	simcom_receive(buffer);
	strcat(simcom->rxBuffer_ , buffer);

    #ifdef DEBUG_SIM
        //usb_serialPrint(buffer); 
		usb_serialPrint(buffer);
    #endif

    switch (simcom->state_)
    {
    case simcom_state_cancel:
        
        // FALTAN FUNCIONES AÚN PARA QUE FUNCINE EL PROCESS DE SIMCOM
		usb_serialPrint("SIMCOM_STATE: CANCEL \n");
        Simcom_request(simcom,"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAT\r", 30, 200, 0);
        Simcom_setTimer(simcom, simcom->startDelay_);
        Simcom_processResponse(simcom, simcom_state_creset);
        break;
    default:
        break;
    }
	
	//si el txBuffer_ no está vacío, envíar el buffer
	if(!(simcom->txBuffer_[0]=='\0')){
		simcom_send(simcom->txBuffer_);
		simcom->lastRequest_=0;
	}

	//si el txBuffer_ está vacío y lastRequest existe, actualizar scanTime_ del ble.
	if(simcom->txBuffer_[0] == '\0' && !simcom->lastRequest_){
		simcom->lastRequest_=_calendar_get_counter(&CALENDAR_0.device);
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
	unsigned long t = _calendar_get_counter(&CALENDAR_0.device);// some like millis()
	//unsigned long t = 123456789;
	
	usb_serialPrint("SIMCOM REQUEST");

	if(!simcom->attempt_ || (simcom->lastRequest_ && (t - simcom->lastRequest_ > wait)) || (simcom->lastError_ && (t - simcom->lastError_ > errorWait))){
		if(simcom->attempt_ < maxAttempt){
			simcom->attempt_++;
			simcom->lastError_ = 0;
			strcpy(simcom->txBuffer_,command);
		}
		else {
			//throw exception(command)
			usb_serialPrint("ERROR EN SIMCOM_request");
		}
	}

}


//acá va el processResponse que veré después. Primero vamos con el lexer. 
void Simcom_processResponse(Simcom * simcom, Simcom_State state){	 
	Token_Name success = Token_Name_ok;
	Token_Name error = Token_Name_error; 
	
	while(1){
		usb_serialPrint("\nWhile 1 - processResponse que nunca se acaba \n");
		Token token = Simcom_lexer(simcom);
		usb_serialPrint("salgo del simcom lexer");
		if ((token.name_ == Token_Name_empty) || (token.name_ == Token_Name_notReady)){
			break; 
		}else if(token.name_ == success){
			Simcom_nextState(simcom, state);
		}else if(token.name_ == error){
			Simcom_retry(simcom); 
		} else {
			// ACÁ PENDIENTE SEGUIR CON EL SIMCOM_ASYNC, NO ES TAN LARGO PERO ESTÁ LATERO
			// Simcom_async(token); 
		}
	}
}


Token Simcom_lexer(Simcom * simcom){
	usb_serialPrint("simcom_lexer | entro al Simcom_lexer \n");
	Token token = {0}; 
    unsigned nInteger = 0;
    unsigned offset = 0;
	usb_serialPrint("simcom_lexer | RxBuffer: ");
	usb_serialPrint(simcom->rxBuffer_);
	usb_serialPrint("\n");
	if ((simcom->rxBuffer_[0] != '\0') && (simcom->rxBuffer_[0] == '>')){
		token.name_ = Token_Name_sendPrompt;
		strcpy(token.value_ ,">");
		token.value_[2]='\0';
	}
	else
	{
		char *substr = strstr(simcom->rxBuffer_, "\r\n"); //puntero al final del rxbuffer
		usb_serialPrint("simcom_lexer | substr"); 
		usb_serialPrint(substr);
		usb_serialPrint("\n");
		if (substr == NULL)	{
			usb_serialPrint("simcom_lexer | substr else == NULL");
			usb_serialPrint("\n");		
			strcpy(token.value_, simcom->rxBuffer_);
			if (token.value_[0] == '\0'){
				token.name_ = Token_Name_empty;
			}
			else{
				token.name_ = Token_Name_notReady;
			}
		}
		else
		{
			usb_serialPrint("simcom_lexer | substr else != NULL");
			usb_serialPrint("\n");
			size_t index = substr - simcom->rxBuffer_; //calcular el indice de donde esta \r\n restando posiciones de memoria
			// viene la linea 744 de simcom.cpp 
			strncpy(token.value_,simcom->rxBuffer_,index + 2); //index + 2 es el largo de la subcadena del  rxbuffer que se copará a token.value_. Por alguna razón en el fw antiguo se hace una copia de la cadena rxBuffer al token.value_
			token.value_[index+2] = '\0';
			
			if(token.value_ == blank){
				token.name_ = Token_Name_blank;
			}else if(token.value_ == ok){
				token.name_ = Token_Name_ok;
			}else if(strncmp(token.value_, cipstat, sizeof(cipstat))==0){
				token.name_ = Token_Name_cipstat;
			}
		}
			
	}
	return token;
}



void Simcom_nextState(Simcom * simcom, Simcom_State state){
	simcom->state_ = state;
	simcom->lastRequest_ = 0;
	simcom->lastError_ = 0;
	simcom->attempt_ = 0;
}

void Simcom_retry(Simcom * simcom){
	simcom->lastError_ = _calendar_get_counter(&CALENDAR_0.device);
}

//-------------se van rellenando funciones por acá?? 



void Simcom_setTimer(Simcom * simcom, unsigned long wait){
	simcom->timer_ = _calendar_get_counter(&CALENDAR_0.device) + wait; //cambiar el millis por la función de microchip
	
}

bool Simcom_timer(Simcom * simcom){
	return _calendar_get_counter(&CALENDAR_0.device) > simcom->timer_; //cambiar millis por la función de microchip
}
