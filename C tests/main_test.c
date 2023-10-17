#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>


#include "main_test.h"
#include "ble.h"
#include "imei.h"
//#include "Binary.h"
#include "Position.h"
#include "simcom_driver.h"

Ble ble = {0}; 
Imei imei ={0}; 
Position position = {0};
Position overflow = {0};
Position zero = {0};

Simcom simcom;

void printBinary(char byte) {
    for (int i = 7; i >= 0; i--) {
        putchar((byte & (1 << i)) ? '1' : '0');
    }
}

int main() {

	/*
	 * SIMCOM TEST
	*/
    const char buffer[] = "115,345,2123,4";
    int integers[10];
    int count;

    getIntegers(buffer, integers, &count);

    // Imprimir los números enteros extraídos
    for (int i = 0; i < count; i++) {
        //printf("%d ", integers[i]);
    }
    //printf("\n");	

	Simcom_init(&simcom);
	printf("IP 1: %s \n", simcom.ipaddr_); 

	Simcom_setApn(&simcom, "iot.secure", "", ""); 
	printf("APN: \n apn: %s \n user: %s \n password: %s \n",simcom.apn_ ,simcom.user_,simcom.password_);

	char * buffer2 = Simcom_IPaddress(&simcom); 
	printf("IP 2: %s \n", buffer2); 

	Simcom_process(&simcom);
	printf("After process");


	strcpy(simcom.rxBuffer_, "Hola mundo loco \r\n");

	char *substr = strstr(simcom.rxBuffer_, "\r\n");

	printf("rx_buffer es: %s", simcom.rxBuffer_);

	printf("substr es: %s \n", substr);

	size_t index = substr - simcom.rxBuffer_;

	printf("index: %zu \n", index);

	char token_value[100];
	
	strncpy(token_value, simcom.rxBuffer_, index+2);
	token_value[index + 2] = '\0';

	printf("token_value: %s \n", token_value);


	char s1[]= "123456789";
	printf("s1[]: %s \n", s1); 
	printf("s1[] +3: %s \n", s1+3); 

	//strcpy(ble.state_, "hola");
	//ble_set_timer(TIME_TO_DELAY, &ble);

	

	/*
	 * IMEI TEST
	 */

/*
	printf("Imei: %llu", imei.imei_);
	
	imei_init(&imei, "865553060836667"); 

	printf("Imei: %llu \n", imei.imei_);

	char encodedString[sizeof(imei.imei_)];

	imei_binary(&imei, encodedString);

	Imei imei2 = {0}; 
	imei_init(&imei2, "123456789876234");

	printf("Imeis are equal? %s\n", imei_is_equal(&imei, &imei2) ? "true" : "false"); 
	printf("Imeis are not equal? %s\n", imei_is_not_equal(&imei, &imei2) ? "true" : "false"); 

	for (int i = 0; i < 7; i++) {
		printBinary(encodedString[i]);
        //printf("%02X ", (unsigned char)encodedString[i]);
    }

	printf("\n");
	for (int i = 0; i < 7; i++) {
		//printBinary(encodedString[i]);
        printf("%02X ", (unsigned char)encodedString[i]);
    }
*/

	/*
	 * POSITION TEST
	 */

	/*	
	Position_Init_Default(&overflow);
	Position_Init_Default(&position);

	char *gpsinfo;
	strcpy(gpsinfo,"+CGPSINFO:3113.343286,N,12121.234064,E,250311,072809.3,44.1,0.0,0");
	Position_Init_Str(&position, gpsinfo); 

	printf("%f \n", position.latitude_);
	printf("%f \n", position.longitude_);

	char position_encoded[6];

	binary_position(&position, position_encoded); 

	for (int i = 0; i < 7; i++) {
		//printBinary(encodedString[i]);
        printf("%02X", (unsigned char)position_encoded[i]);
    }
	printf("\n");

	for (int i = 0; i < 7; i++) {
		printBinary(position_encoded[i]);
    }	
*/ 
    while(1){
		//ble_process(&ble);
		//printf("%d",ble.state_);
		
		//printf("%d",ble.timer_);
		sleep(1);
	}
	return 0; 
}