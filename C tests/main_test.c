#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>

#include "ble.h"
#include "imei.h"
//#include "Binary.h"

Ble ble = {0}; 
Imei imei ={0}; 


void printBinary(char byte) {
    for (int i = 7; i >= 0; i--) {
        putchar((byte & (1 << i)) ? '1' : '0');
    }
}


int main() {
	//strcpy(ble.state_, "hola");
	//ble_set_timer(TIME_TO_DELAY, &ble);
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

    while(1){
		//ble_process(&ble);
		//printf("%d",ble.state_);
		
		//printf("%d",ble.timer_);
		sleep(1);
	}
	return 0; 
}