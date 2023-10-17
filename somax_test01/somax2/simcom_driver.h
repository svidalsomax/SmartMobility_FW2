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
#include <stdio.h>

#include "usb_start.h"

#include "Imei.h"
#include "Position.h"

#define DEBUG_SIM

int simcom_init(void);
void simcom_send(char* command);
void simcom_send_and_receive(char* command, char* response);
void simcom_receive(char* response);

/******************************************************************************
**  VARIABLES SOMAX
******************************************************************************/

typedef enum{
	simcom_state_cancel,
	simcom_state_creset,
	simcom_state_ate0,
	simcom_state_cmee,
	// simcom_state_cgpshot,
	// simcom_state_cgpsclose,
	simcom_state_cgps,
	simcom_state_simei,
	simcom_state_stk,
	simcom_state_cnvw,
	simcom_state_cgatt,
	simcom_state_cgsockcont,
	simcom_state_cgauth,
	simcom_state_csocksetpn,
	simcom_state_ciptimeout,
	simcom_state_cgpsinfo,
	simcom_state_cipsendmode,
	simcom_state_ciptcpfin,
	simcom_state_netopen,
	simcom_state_ipaddr,
	simcom_state_wait,
	simcom_state_cipopen,
	simcom_state_cipsend0,
	simcom_state_cipsend1,
	simcom_state_cipsend2,
	simcom_state_cipsend3,
	simcom_state_cipstat,
	simcom_state_cipclose,
	simcom_state_netclose,
	simcom_state_gpsoff,
	simcom_state_gpson,
	simcom_state_end
} Simcom_State;

typedef enum {
	Token_Name_empty,
	Token_Name_notReady,
	Token_Name_blank,
	Token_Name_ok,
	Token_Name_error,
	Token_Name_start,
	Token_Name_pbDone,
	Token_Name_simei,
	Token_Name_gpsinfo,
	Token_Name_netopen,
	Token_Name_ipaddr,
	Token_Name_cipopen,
	Token_Name_sendPrompt,
	Token_Name_cipsend,
	Token_Name_recvFrom,
	Token_Name_ciperror,
	Token_Name_ipclose,
	Token_Name_cipstat,
	Token_Name_cipclose,
	Token_Name_netclose,
	Token_Name_unknown
} Token_Name;

typedef struct {
	Token_Name name_;
	char value_[100] ;
	char* message_ ;
	int array_[3];
	Imei imei_;
	Position position_;
} Token;
	
typedef struct {
	unsigned long time_;
	Position position_;
} PositionRecord;

typedef struct {
	char txBuffer_[100]; 
	char rxBuffer_[100];
	
	Simcom_State state_; 
	bool otaMode_; 
	bool gpsState; 
	unsigned attempt_; 
	unsigned connection_; 
	
	unsigned long startDelay_;
	unsigned long lastRequest_;
	unsigned long lastError_;
	unsigned long timer_;
	unsigned long lastReset_;
	unsigned long lastOnline_;
	unsigned long lastTransmission_;
	unsigned long tDataRxSize_;
	unsigned long tDataTxSize_;
	
	char * apn_; 
	char * user_; 
	char * password_; 
	char * ipaddr_; 

    unsigned long netTimeout_;
    unsigned long openTimeout_;
    unsigned long sendTimeout_;
	
	char * serverIp_; 
	unsigned serverPort_;
	
    unsigned long remoteServerDisconnectionCount_;
    unsigned long networkServerDisconnectionCount_;	
	
    unsigned positionDelay_;
    bool disconectionFlag_;
	char * tcpTxBuffer_; 
	char * tcpRxBuffer_; 
	size_t tcpTxBlock_; 
	
	Imei imei_; 
	PositionRecord positionRecord_;
	
} Simcom;


/******************************************************************************
**  FUNCIONES SOMAX
******************************************************************************/
void getIntegers(const char *buffer, int *integers, int *count);

void Simcom_struct_init(Simcom * simcom );

void Simcom_setStartDelay(Simcom * simcom, unsigned long startDelay);

void Simcom_setApn(Simcom * simcom, char * apn, char * user, char * password);

void Simcom_setTimeouts(Simcom * simcom, unsigned long netTimeout, unsigned long openTimeout, unsigned long sendTimeout);

void Simcom_setServer(Simcom * simcom, char * serverIp, unsigned serverPort);

void Simcom_setPositionDelay(Simcom * simcom, unsigned positionDelay);

Simcom_State Simcom_state(Simcom * simcom);

unsigned Simcom_connection(Simcom * simcom);

unsigned long Simcom_lastReset(Simcom * simcom);

unsigned long Simcom_lastOnline(Simcom * simcom);

unsigned long Simcom_lastTransmission(Simcom * simcom);

Imei Simcom_imei(Simcom * simcom);

PositionRecord Simcom_positionRecord(Simcom * simcom);

unsigned long Simcom_tDataRxSize(Simcom * simcom);

unsigned long Simcom_tDataTxSize(Simcom * simcom);

void Simcom_closeConnection(Simcom * simcom);

void Simcom_setConnection(Simcom * simcom);

char * Simcom_IPaddress(Simcom * simcom);

void Simcom_setOtaMode(Simcom * simcom);

void Simcom_clearOtaMode(Simcom * simcom);

void Simcom_process(Simcom * simcom);

void Simcom_reset(Simcom * simcom);

void Simcom_netclose(Simcom * simcom);

void Simcom_tcpSend(Simcom * simcom, char * buffer);

char * Simcom_tcpReceive(Simcom * simcom);

char * Simcom_tcpTxBuffer(Simcom * simcom);

char * Simcom_tcpRxBuffer(Simcom * simcom);

void Simcom_request(Simcom * simcom, char * command, unsigned maxAttempt, unsigned long wait, unsigned long errorWait);

void Simcom_processResponse(Simcom * simcom, Simcom_State state);

Token Simcom_lexer(Simcom * simcom);

void Simcom_nextState(Simcom * simcom, Simcom_State state);

void Simcom_retry(Simcom * simcom);

void Simcom_setTimer(Simcom * simcom, unsigned long wait);

bool Simcom_timer(Simcom * simcom);

#endif /* SIMCOM_DRIVER_H_ */