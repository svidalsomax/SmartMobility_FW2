#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>

#include "ble.h"

Ble ble = {0};

int main() {
	//strcpy(ble.state_, "hola");
	//ble_set_timer(TIME_TO_DELAY, &ble);

	uint32_t numero = 20; 
	char numero_str[20];
	sprintf(numero_str, "%lu", (unsigned long)numero);
	printf(numero_str);
	int time = time(NULL);
	printf("Timestamp: %d\n",time);
    while(1){
		//ble_process(&ble);
		//printf("%d",ble.state_);
		
		//printf("%d",ble.timer_);
		sleep(1);
	}
	return 0; 
}