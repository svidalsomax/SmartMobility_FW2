#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "ble.h"

Ble ble = {0};

int main() {
	strcpy(ble.state_, "hola");
    while(1){
		ble_process(&ble);
		printf(ble.state_);
		sleep(1);
	}
	return 0; 
}