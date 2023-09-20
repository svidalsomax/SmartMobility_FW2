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
#include "Imei.h"


int simcom_init(void);
void simcom_send(char* command);
void simcom_send_and_receive(char* command, char* response);
void simcom_receive(char* response);

/******************************************************************************
**  VARIABLES SOMAX
******************************************************************************/

typedef enum{
	cancel,
	creset,
	ate0,
	cmee,
	// cgpshot,
	// cgpsclose,
	cgps,
	simei,
	stk,
	cnvw,
	cgatt,
	cgsockcont,
	cgauth,
	csocksetpn,
	ciptimeout,
	cgpsinfo,
	cipsendmode,
	ciptcpfin,
	netopen,
	ipaddr,
	wait,
	cipopen,
	cipsend0,
	cipsend1,
	cipsend2,
	cipsend3,
	cipstat,
	cipclose,
	netclose,
	gpsoff,
	gpson,
	end
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
	char* value_ ; 
	char* message_ ; 
	int array_[3]; 
	Imei imei_; 
	//Position position_; 
} Token;
	
	
typedef struct {
	char *txBuffer_; 
	char *rxBuffer_;
	
	Simcom_State state_; 
	bool otaMode; 
	bool gpsState; 
	unsigned attempt_; 
	unsigned connection; 
	
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
	//PositionRecord positionRecord_;
	
} Simcom;





#endif /* SIMCOM_DRIVER_H_ */