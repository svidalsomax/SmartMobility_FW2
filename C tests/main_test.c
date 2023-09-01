#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "ble.h"

Ble ble = {0};

int main() {
	strcpy(ble.state_, "hola");
	ble_set_timer(TIME_TO_DELAY, &ble);
    while(1){
		ble_process(&ble);
		printf(ble.state_);
		printf("%d",ble.timer_);
		sleep(1);
	}
	return 0; 
}