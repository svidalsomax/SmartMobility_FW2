#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>


#include "main_test.h"
#include "ble.h"
//#include "Binary.h"
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

void erase_string(int start, int length, char * str);

int main() {

	/*
	 * SIMCOM TEST
	*/

/*
    const char buffer[] = "115,345,2123,4";
    int integers[10];
    int count;

    getIntegers(buffer, integers, &count);

	size_t size_of_integers = sizeof(integers)/sizeof(integers[0]);
	//printf("count: %d \n", count);


    for (int i = 0; i < count; i++) {
        //printf("%d \n", integers[i]);
    }

	Simcom_init(&simcom);
	//printf("IP 1: %s \n", simcom.ipaddr_); 

	Simcom_setApn(&simcom, "iot.secure", "", ""); 
	//printf("APN: \n apn: %s \n user: %s \n password: %s \n",simcom.apn_ ,simcom.user_,simcom.password_);

	char * buffer2 = Simcom_IPaddress(&simcom); 
	//printf("IP 2: %s \n", buffer2); 

	Simcom_process(&simcom);
	//printf("After process");

	strcpy(simcom.rxBuffer_, "+CGPSINFO: XXXXXX\r\nOK\r\n");

	char *substr = strstr(simcom.rxBuffer_ + 2, "\r\n");

	//printf("rx_buffer es: %s", simcom.rxBuffer_);

	Token token;
	strcpy(token.value_,"+SIMEI: 863427044253806\r\n");
	imei_loadText(&token.imei_, token.value_+8);

	printf("token.imei_.imei: %zu \n", token.imei_.imei_);

	simcom.imei_.imei_=token.imei_.imei_;

	printf("simcom.imei_.imei_: %zu \n", simcom.imei_.imei_);
	//strncpy(token_value, simcom.rxBuffer_, 10);

	strcpy(token.value_,"+IPADDR: 10.71.155.158\r\n");
	printf("token value_: %s", token.value_);

	simcom.ipaddr_ = token.value_ + 9;
	simcom.ipaddr_[strlen(simcom.ipaddr_)-2]='\0';
	printf("simcom.ipaddr_: %s", simcom.ipaddr_);




	printf("token value es: %s \n", token_value);
	printf("token value strlen: %d \n", strlen(token_value));
	printf("token value sisezof: %d \n", sizeof(token_value));

	size_t index = substr - simcom.rxBuffer_;






	for (int i = 0; (i < 2) && (substr != NULL); i++)
	{
		substr = strstr(substr+2 , "\r\n");
		if (substr != NULL)
		{
			index = substr - simcom.rxBuffer_;
			//printf("index: %zu \n", index);
			//printf("substr es: %s", substr);
		}else{
			index = substr - simcom.rxBuffer_;
			//printf("index NULL?: %zu \n", index);
			//printf("substr NULL?: %s", substr);
		}		
	}

	//printf("index: %zu \n", index);

	//char token_value[100];
	
	//strncpy(token_value, simcom.rxBuffer_, index+2);
	//token_value[index + 2] = '\0';

	//printf("token_value: %s \n", token_value);


	char s1[]= "ERROR\r\n";
	char scpy[5]; 

	int size_of = sizeof(s1);
	int strlen_ = strlen(s1);

	//printf("size_of(s1) : %d \n", size_of);
	//printf("strlen(s1): %d \n", strlen_);


    char str[] = "Hello, World!";
    int start = 0;
    int length = 7;
    int str_length = strlen(str);

	erase_string(start, length, str);
 	//printf("%s\n", str);

    if (start >= 0 && start < str_length && length > 0) {
        for (int i = start; i < str_length - length; i++) {
            str[i] = str[i + length];
        }
        str[str_length - length] = '\0'; // Termina la cadena
    }

    printf("%s\n", str); // Imprimirá "Hello, "





	size_t length = atoi(s1+5);
	printf("length size_t s1+5: %zu \n", length);


	strncpy(scpy,s1+6,4);
	scpy[4]='\0';
	size_t length2 = atoi(scpy);
	
	printf("length size_t scpy: %zu \n", length2);

	printf("s1[]: %s \n", s1); 
	printf("sscpy[]: %s \n", scpy); 
	*/
	

	//char * subs1 = s1+strlen(token_value); 
	//memmove(s1, subs1, strlen(substr)+1);
	//printf("s1[] despues de memmove: %s", s1);
	//strcpy(ble.state_, "hola");
	//ble_set_timer(TIME_TO_DELAY, &ble);

	

	/*
	 * IMEI TEST
	 */

	char encoded_message[1024]={0};

	printf("    [ENCODE TIME]    ");

	unsigned long time_ = 1700061550;
	char encoded_time[5];
	encode(time_, 4, encoded_time);
	encoded_time[4]='\0';

	printf("\n encoded time HEX:");
	for (int i = 3; i >= 0; i--) {
		//printBinary(encodedString[i]);
        printf("%02X ", (unsigned char)encoded_time[i]);
    }
/*
	printf("\n encoded time BIN:");
	for (int i = 3; i >= 0; i--) {
		printBinary(encoded_time[i]);
        //printf("%02X ", (unsigned char)encoded_time[i]);
    }
*/
	printf(" \n      [ENCODE IMEI]");
	char id_imei_to_strcat[2];
	id_imei_to_strcat[0] = (char)(0x40);
	id_imei_to_strcat[1] = '\0';

	strcat(encoded_message, id_imei_to_strcat);  //ID identify
	strcat(encoded_message, encoded_time);  // Time encoded

	printf("Imei: %llu \n", imei.imei_);
	
	imei_init(&imei, "865553060836667"); 

	printf("Imei: %llu \n", imei.imei_);

	char encodedImei[8];

	imei_binary(&imei, encodedImei);
	encodedImei[7]='\0';

	strcat(encoded_message, encodedImei);
	printf("\nencodede message hexa: %c \n", encoded_message[0]);

/*
	Imei imei2 = {0}; 
	imei_init(&imei2, "123456789876234");

	printf("Imeis are equal? %s\n", imei_is_equal(&imei, &imei2) ? "true" : "false"); 
	printf("Imeis are not equal? %s\n", imei_is_not_equal(&imei, &imei2) ? "true" : "false"); 

	for (int i = 6; i >= 0; i--) {
		printBinary(encodedImei[i]);
        //printf("%02X ", (unsigned char)encodedString[i]);
    }
*/ 

	printf("\n");
	for (int i = 6; i >= 0; i--) {
		//printBinary(encodedString[i]);
        printf("%02X ", (unsigned char)encodedImei[i]);
    }
/*
	printf("\n");
	for (int i =0; i < 7; i++) {
		//printBinary(encodedString[i]);
        printf("%02X ", (unsigned char)encodedImei[i]);
    }
*/



	/*
	 * POSITION TEST
	 */

	
	printf(" \n[POSITION SECTION]");

	Position_Init_Default(&overflow);
	Position_Init_Default(&position);

	char gpsinfo[sizeof("+CGPSINFO: 3113.343286,N,12121.234064,E,250311,072809.3,44.1,0.0,0")];
	
	strcpy(gpsinfo,"+CGPSINFO: 3113.343286,N,12121.234064,E,250311,072809.3,44.1,0.0,0");
 
	Position_Init_Str(&position, gpsinfo); 

	printf("%f \n", position.latitude_);
	printf("%f \n", position.longitude_);

	char position_encoded[7];

	binary_position(&position, position_encoded); 

	for (int i = 0; i < 7; i++) {
		//printBinary(encodedString[i]);
        printf("%02X ", (unsigned char)position_encoded[i]);
    }

	printf("Arreglo contado para atrss:");
	for (int i = 6; i >= 0; i--) {
		//printBinary(encodedString[i]);
        printf("%02X ", (unsigned char)position_encoded[i]);
    }

	printf("\n");

	for (int i = 0; i < 7; i++) {
		printBinary(position_encoded[i]);
    }	

	printf("\n");

	printf("PRINT POSITION string: %s\n", position_encoded);
	printf("PRINT POSITION HEXA: %02X\n", position_encoded);









	printf("  [TCP TX BUFFER TEST]   \n");
	char packet_to_send[1024];
	char cuarenta = (char)(0x40);
	printf("curante %c: \n", cuarenta);
	strcat(packet_to_send, &cuarenta);
	packet_to_send[1]='\0';
	printf("static cast a 0x40: %s \n", packet_to_send);

	char cincuenta = 50;
	printf("cincuenta: %c\n", cincuenta);


	
    while(1){
		//ble_process(&ble);
		//printf("%d",ble.state_);
		
		//printf("%d",ble.timer_);
		sleep(1);
	}
	return 0; 
}


void erase_string(int start, int length, char * str){
	int str_length = strlen(str);
	if (start >= 0 && start < str_length && length > 0) {
        for (int i = start; i < str_length - length; i++) {
            str[i] = str[i + length];
        }
        str[str_length - length] = '\0'; // Termina la cadena
    }
}