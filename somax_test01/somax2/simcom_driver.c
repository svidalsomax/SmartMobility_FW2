/*
 * simcom_driver.c
 *
 *  Author: MWF
 */ 

#include "simcom_driver.h"

#include <ctype.h>
#include <stdlib.h>

#define MAX_MESSAGE_LENGTH_SIMCOM 1024 // Cambiar tambien USART_1_BUFFER_SIZE en driver_init.c
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
	char at_cmd[MAX_MESSAGE_LENGTH_SIMCOM] = {0};
	strcpy(at_cmd, "");
	strcat(at_cmd, command);
	io_write(simcom_io, (uint8_t *)at_cmd, strlen(at_cmd));
	delay_ms(100);
}

void simcom_send_and_receive(char* command, char* response)
{
	char at_cmd[MAX_MESSAGE_LENGTH_SIMCOM] = {0};
	strcpy(at_cmd, "");
	strcat(at_cmd, command);
	io_write(simcom_io, (uint8_t *)at_cmd, strlen(at_cmd));
	delay_ms(100);
	io_read(simcom_io, response, 1024);  //1024 era el max length
	delay_ms(100); // se añadieron 3 time to delay para ver si eso era el problema de tener la respuesta completa.
}

void simcom_receive(char* response)
{
	io_read(simcom_io, response, MAX_MESSAGE_LENGTH_SIMCOM);
	delay_ms(120);
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
    char buffer[1024] = {0};
	simcom_receive(buffer);
	strcat(simcom->rxBuffer_ , buffer);

    #ifdef DEBUG_SIM
        usb_serialPrint("BUFFER INICIAL PROCESS: "); 
		usb_serialPrint(buffer);
		usb_serialPrint("\n");
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
	
	//Reset	
	case simcom_state_creset:
		usb_serialPrint("SIMCOM_STATE: RESET\n");
		
		Simcom_request(simcom, "AT+CRESET\r", 1, 3600000, 0);
		
		if(Simcom_timer(simcom)){
			usb_serialPrint("Simcom_timer ok en reset\n");
			simcom->rxBuffer_[0]="\0"; //rxBuffer.clear();
			simcom->lastReset_ = _calendar_get_counter(&CALENDAR_0.device);
			Simcom_nextState(simcom, simcom_state_ate0);
		}
		
		break;
		
	// Echo off 
	case simcom_state_ate0:
		usb_serialPrint("SIMCOM_STATE: ATE0\n");
		
		#ifdef DEBUG_SIM 
			Simcom_request(simcom, "ATE1\r", 5, 3000, 1000);
		#else 
			Simcom_request(simcom, "ATE0\r", 5, 3000, 1000);
		#endif
		
		Simcom_processResponse(simcom, simcom_state_cmee); 

		break;
		
	case simcom_state_cmee: 
		usb_serialPrint("SIMCOM_STATE: CMEE\n");
	
		#ifdef DEBUG_SIM
			Simcom_request(simcom, "AT+CMEE=2\r", 5, 3000, 1000);
		#else
			Simcom_request(simcom, "AT+CMEE=1", 5, 3000, 1000);
		#endif		
		
		Simcom_processResponse(simcom, simcom_state_cgps);
		
		break;
		
	case simcom_state_cgps: 
		usb_serialPrint("SIMCOM_STATE: CGPS\n");
	
		Simcom_request(simcom, "AT+CGPS=1,1\r", 5, 3000, 1000); 	
		Simcom_processResponse(simcom, simcom_state_simei);
		break; 
		
	case simcom_state_simei:
		usb_serialPrint("SIMCOM_STATE: SIMEI\n");
		Simcom_request(simcom, "AT+SIMEI?\r", 5, 3000, 1000);
		
		while(1){
			usb_serialPrint("\n WHILE 1 - SIMEI \n");
			Token token = Simcom_lexer(simcom, true);
			usb_serialPrint("\n salgo del simcom lexer");
			if ((token.name_ == Token_Name_empty) || (token.name_ == Token_Name_notReady)){
				break;
			}else if(token.name_ == Token_Name_simei){
				usb_serialPrint("\n    Token name simei asignado    \n");
				simcom->imei_.imei_ = token.imei_.imei_;
			}else if(token.name_ == Token_Name_ok){
				usb_serialPrint("\n    Token name ok despues de simei asignado     \n");
				if(simcom->imei_.imei_){
					usb_serialPrint("\n    next_state de simei     \n");
					Simcom_nextState(simcom, simcom_state_stk);
				}
			}
		}
		break;
	
	case simcom_state_stk: 
		usb_serialPrint("SIMCOM_STATE: STK\n");
		Simcom_request(simcom, "AT+STK=0\r", 5, 3000, 1000);
		Simcom_processResponse(simcom, simcom_state_cgatt); //aca debería cambiar a estadoCNVW pero por simcomnueva se pasa a CGDCONT
		break;
	
	//GPRS Attach
	case simcom_state_cgatt:
		usb_serialPrint("SIMCOM_STATE: CGATT\n");
		Simcom_request(simcom, "AT+CGATT=1\r", 5, 3000, 1000);
		Simcom_processResponse(simcom, simcom_state_cgdcont);
		break;
	
	//SET APN
	case simcom_state_cgdcont:
		usb_serialPrint("SIMCOM_STATE: CGDCONT\n");
		Simcom_request(simcom, "AT+CGDCONT=1,\"IP\",\"iot.secure\"\r", 5, 3000, 1000);
		Simcom_processResponse(simcom, simcom_state_cgact);
		break;
	
	// Active PDP context 
	case simcom_state_cgact: 
		usb_serialPrint("SIMCOM_STATE: CGACT\n");
		Simcom_request(simcom, "AT+CGACT=1,1\r", 5, 3000, 1000);
		Simcom_processResponse(simcom, simcom_state_cgauth);
		break; 
		
	// Set PDP context username and password
	case simcom_state_cgauth:
		usb_serialPrint("SIMCOM_STATE: CGAUTH\n");
		Simcom_request(simcom, "AT+CGAUTH=1,1,\" \", \" \"\r", 5, 3000, 1000); //no hay usuario ni pass, pero debe quedar en espacio
		Simcom_processResponse(simcom, simcom_state_ciptimeout);
		break;
		
	case simcom_state_ciptimeout:
		usb_serialPrint("SIMCOM_STATE: CIPTIMEOUT\n");
		break;
	
    default:
		usb_serialPrint("SIMCOM_STATE: DEFAULT \n");
        break;
    }
	
	//si el txBuffer_ no está vacío, envíar el buffer
	if(!(simcom->txBuffer_[0]=='\0')){
		usb_serialPrint("txbuffer:"); 
		usb_serialPrint(simcom->txBuffer_);
		simcom_send(simcom->txBuffer_);
		string_clear(simcom->txBuffer_,sizeof(simcom->txBuffer_)); //limpia el txBuffer para que no se vuelva a mandar
		simcom->lastRequest_=0;
	}

	//si el txBuffer_ está vacío y lastRequest existe, actualizar lastReques_ de la simcom
	if(simcom->txBuffer_[0] == '\0' && !simcom->lastRequest_){
		usb_serialPrint("tXBuffer vacio");
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
	
	usb_serialPrint("SIMCOM REQUEST\n");

	if(!simcom->attempt_ || (simcom->lastRequest_ && (t - simcom->lastRequest_ > wait)) || (simcom->lastError_ && (t - simcom->lastError_ > errorWait)))
	{	
		if(simcom->attempt_ < maxAttempt)
		{	
			simcom->attempt_++;
			simcom->lastError_ = 0;
			strcpy(simcom->txBuffer_,command);
			usb_serialPrint("SIMCOM REQUEST: attempt ++, lasterror = 0, txbuffer = ");			
			usb_serialPrint(command);
			usb_serialPrint("\n");
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
		Token token = Simcom_lexer(simcom, true);
		usb_serialPrint("\n salgo del simcom lexer");
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


Token Simcom_lexer(Simcom * simcom, bool pull){
	usb_serialPrint("simcom_lexer | entro al Simcom_lexer \n");
	Token token = {0}; 
    unsigned nInteger = 0;
    unsigned offset = 0;
	
	//usb_serialPrint("simcom_lexer | RxBuffer: ");
	usb_serialPrint(simcom->rxBuffer_);
	//usb_serialPrint("\n");
	
	if ((simcom->rxBuffer_[0] != '\0') && (simcom->rxBuffer_[0] == '>')){
		token.name_ = Token_Name_sendPrompt;
		strcpy(token.value_ ,">");
		token.value_[2]='\0';
	}
	else
	{
		char rn[2]="\r\n";
		char *rn_pointer = strstr(simcom->rxBuffer_, rn); //puntero a donde se encuentr \r\n en rxBuffer_
		usb_serialPrint("simcom_lexer | substr:"); 
		usb_serialPrint(rn_pointer);
		//usb_serialPrint("\n");
		if (rn_pointer == NULL)	{
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
			size_t index = rn_pointer - simcom->rxBuffer_; //calcular el indice de donde esta \r\n restando posiciones de memoria
			
			//744 de simcom cpp:
			
			strncpy(token.value_,simcom->rxBuffer_,index + 2); //index + 2 es el largo de la subcadena del  rxbuffer que se copará a token.value_.
			token.value_[index+2+1] = '\0';
			
			usb_serialPrint("simcom_lexer | token.value_:");
			usb_serialPrint(token.value_);
			usb_serialPrint("\n");
			
			if(strcmp(token.value_,blank)==0){
				token.name_ = Token_Name_blank;
			}else if(strcmp(token.value_,ok)==0){
				token.name_ = Token_Name_ok;
			}else if(strncmp(token.value_, cipstat, strlen(cipstat))==0){ // cambiar cipstat porque no llegará nunca ahí
				token.name_ = Token_Name_cipstat;
			}else if(strncmp(token.value_, error, strlen(error)) == 0){ 
				token.name_ = Token_Name_error;
				token.array_[0]=-1;
			}else if ((strncmp(token.value_, cmeError, strlen(cmeError)) == 0) || (strncmp(token.value_, cmsError, strlen(cmsError)) == 0)){
				token.name_ = Token_Name_error;
				offset = strlen(cmeError)-1;
				nInteger = 1;
			}else if (strncmp(token.value_, start, strlen(start)) == 0){
				token.name_ = Token_Name_start;
			}else if (strncmp(token.value_, pbDone, strlen(pbDone)) == 0){
				token.name_ = Token_Name_pbDone;
			}else if (strncmp(token.value_, gpsinfo, strlen(gpsinfo)) == 0){
				for (int i = 0; (i < 2) && (rn_pointer != NULL); i++)
				{
					rn_pointer = strstr(rn_pointer+2 , "\r\n");
				}
				if (rn_pointer == NULL)
				{
					token.name_ = Token_Name_notReady;
				}else{
					index = rn_pointer - simcom->rxBuffer_;	
					token.name_ = Token_Name_gpsinfo;
					strncpy(token.value_ ,simcom->rxBuffer_, index);
					Position_loadRaw(&token.position_, token.value_);
				}
			}else if (strncmp(token.value_, simei, strlen(simei)) == 0){
				if (strlen(token.value_) == 25)
				{
					token.name_ = Token_Name_simei;
					imei_loadText(&token.imei_,token.value_+8); //acá está el error
				}else{
					token.name_ = Token_Name_unknown;
				}				
			}else if(strncmp(token.value_, netopen, strlen(netopen)) == 0){
				token.name_ = Token_Name_netopen;
				offset = sizeof(netopen) - 1;
				nInteger = 1; 
			}else if(strncmp(token.value_, ipaddr, strlen(ipaddr)) == 0){
				token.name_ = Token_Name_ipaddr;
				offset = sizeof(ipaddr) - 1;
				nInteger = 1; 
			}else if(strncmp(token.value_, cipopen, strlen(cipopen)) == 0){
				token.name_ = Token_Name_cipopen;
				offset = sizeof(cipopen) - 1;
				nInteger = 2;				
			}else if(strncmp(token.value_, cipsend, strlen(cipsend)) == 0){
				token.name_ = Token_Name_cipsend;
				offset = sizeof(cipsend) - 1;
				nInteger = 3;
			}else if(strncmp(token.value_, cipclose, strlen(cipclose)) == 0){
				token.name_ = Token_Name_cipclose;
				offset = sizeof(cipclose) - 1;
				nInteger = 2;
			}else if(strncmp(token.value_, netclose, strlen(netclose)) == 0){
				token.name_ = Token_Name_netclose;
				offset = sizeof(netclose) - 1;
				nInteger = 1;
			}else if(strncmp(token.value_, recvFrom, strlen(recvFrom)) == 0){
				token.name_ = Token_Name_notReady;
				
				if (strlen(simcom->rxBuffer_)>index + 6)
				{
					char s[5];
					s[4]='\0';
					strncpy(s,simcom->rxBuffer_ + index + 6 , 4);
					
					size_t length = atoi(s);
					
					rn_pointer = strstr(rn_pointer+2 , "\r\n");
					
					if (rn_pointer != NULL)
					{
						index = rn_pointer - simcom->rxBuffer_;
						if (length)
						{
							index += 2; 
							if (index + length <= strlen(simcom->rxBuffer_))
							{
								token.name_ = Token_Name_recvFrom;
								
								strncpy(token.value_,simcom->rxBuffer_,index+length);
								token.value_[index+length]='\0';
								
								strncpy(token.message_,simcom->rxBuffer_ + index,length);
								token.message_ [length] = '\0';
							} else {token.name_ = Token_Name_notReady;}
							
						} else {token.name_ = Token_Name_unknown;}
					}
				}
			}else if (strncmp(token.value_, ciperror, strlen(ciperror)) == 0){
				token.name_ = Token_Name_ciperror;
				offset = sizeof(ciperror)-1;
				nInteger = 1;
			}else if (strncmp(token.value_, ipclose, strlen(ipclose)) == 0){
				token.name_ = Token_Name_ipclose;
				offset = sizeof(ipclose)-1;
				nInteger = 2;
			} else {
				token.name_ = Token_Name_unknown;
				usb_serialPrint("no pilla coincidencia en token ok");
			}
			
			if (nInteger > 0)
			{
				int vector[index];
				int count;
				char buffer[index-offset]; 
				strncpy(buffer, token.value_ + offset, index-offset);
				getIntegers(buffer, vector, &count);
				
				if (count == nInteger)
				{
					switch (nInteger)
					{
						case 3: 
							token.array_[2] = vector [2];
						case 2: 
							token.array_[1] = vector [1];
						case 1: 
							token.array_[0] = vector [0];
					}
				} else {
					token.name_ = Token_Name_notReady;
				}
				
			}
			
		}
	}
	
	//quitar parte que se analizo del rxBuffer: 
	if(pull && (token.name_ != Token_Name_notReady)){
		usb_serialPrint("----    TOKEN OK Y DISTINTO A NOT READY ----   \n");
		char * substr_rxBuffer_ = simcom->rxBuffer_ + strlen(token.value_);
		memmove(simcom->rxBuffer_, substr_rxBuffer_, strlen(substr_rxBuffer_)+1); //rxBuffer toma el valor desde el indice que tiene substr_RxBuffer_
	}

	char token_name_str_[2]; 
	sprintf(token_name_str_, "%d", token.name_);
	usb_serialPrint("simcom_lexer | TOKEN NAME: ");
	usb_serialPrint(token_name_str_);
	usb_serialPrint("\n");
	
	usb_serialPrint("simcom_lexer | rxBuffer despues de memmove: ");
	usb_serialPrint(simcom->rxBuffer_);
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
void Simcom_async(Simcom * simcom, Token * token){
	if(token->name_ == Token_Name_gpsinfo){
		simcom->positionRecord_.time_ = _calendar_get_counter(&CALENDAR_0.device);
		simcom->positionRecord_.position_ = token->position_; 
	}
	else if (token->name_ == Token_Name_recvFrom)
	{
		strcat(simcom->tcpRxBuffer_, token->message_);
	}
	else if (token->name_ == Token_Name_ipclose) //server close connection 
	{
		if(simcom->remoteServerDisconnectionCount_ < 60UL)
		{
			simcom->remoteServerDisconnectionCount_++;
		}
		
		Simcom_setTimer(simcom,(1UL+simcom->remoteServerDisconnectionCount_)*15000UL);
		if (simcom->state_ <simcom_state_cipsend3)
		{
			Simcom_nextState(simcom, simcom_state_cipopen);
		}else 
		{
			Simcom_nextState(simcom, simcom_state_cipclose);
		}
	}
	else if (token->name_ == Token_Name_netclose)
	{
		if(token->array_[0] != 0 && !simcom->disconectionFlag_){
			Simcom_nextState(simcom, simcom_state_netopen);
		}
	}
	else if ((token->name_ == Token_Name_start) && (simcom->state_ != simcom_state_creset)) 
	{
		Simcom_nextState(simcom, simcom_state_creset);
		simcom->attempt_ = 1;
		simcom->lastRequest_ = _calendar_get_counter(&CALENDAR_0.device);
		Simcom_setTimer(simcom, 20000);
	}
	else if (token->name_ == Token_Name_cipstat) //ESTE ESTADO TIENE QUE CAMBIAR
	{
		char t_val[strlen(token->value_)];
		strcpy(t_val, token->value_);
		Simcom_erasePartOfString(0,strlen(cipstat),t_val);
		
		char delimeter[]= ",";
		
		//hay que sacar indice de donde está el delimeter en t_val
		char * delimeter_ptr = strstr(t_val, delimeter); //puntero a donde se encuentr "," en t_val
		
		size_t pos_delimeter_ptr = delimeter_ptr - t_val; //calcular el indice de donde esta \r\n restando posiciones de memoria
		char n_token[pos_delimeter_ptr+1];
		strncpy(n_token, t_val,pos_delimeter_ptr);
		n_token[pos_delimeter_ptr+1] = '\0';
		
		simcom->tDataTxSize_ = (unsigned long) atoi(n_token);
		
		Simcom_erasePartOfString(0,pos_delimeter_ptr+strlen(delimeter), t_val);
		
		delimeter_ptr = strstr(t_val, delimeter);
		pos_delimeter_ptr = delimeter_ptr - t_val;
		strncpy(n_token, t_val,pos_delimeter_ptr);
		n_token[pos_delimeter_ptr+1] = '\0';
		
		simcom->tDataRxSize_ = (unsigned long) atoi(n_token);
	}
}


void Simcom_setTimer(Simcom * simcom, unsigned long wait){
	simcom->timer_ = _calendar_get_counter(&CALENDAR_0.device) + wait; //cambiado el millis por la función de microchip
	
}

bool Simcom_timer(Simcom * simcom){
	return _calendar_get_counter(&CALENDAR_0.device) > simcom->timer_; //cambiado millis por la función de microchip
}

void Simcom_erasePartOfString(int start, int length, char * str){
	int str_length = strlen(str);
	if (start >= 0 && start < str_length && length > 0) {
		for (int i = start; i < str_length - length; i++) {
			str[i] = str[i + length];
		}
		str[str_length - length] = '\0'; // Termina la cadena
	}	
}



//
//
// 
// ------------- FUNCIONES EXTRAASS ----------
//
//
//

void string_clear(char *str, size_t size) {
	for (size_t i = 0; i < size; i++) {
		str[i] = '\0'; // Asigna '\0' (carácter nulo) a cada elemento
	}
}