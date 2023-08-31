#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#define MAX_MESSAGE_LENGTH 64

#define BLE_ROLE_MASTER     0
#define BLE_ROLE_SLAVE      1
#define TIME_TO_DELAY       750

typedef enum {
	BLE_BEGIN,
	BLE_RENEW,
	BLE_ROLE,
	BLE_IMME,
	BLE_RESET,
	BLE_PROCESS,
}bleStatus;

typedef struct{
	char txBuffer_[64];
	char rxBuffer_[64];
	bleStatus bleState_;
	unsigned long timer_;
	unsigned long time_;
	int tryCounter_;
	char state_[64];
	bool role_ ;
	int scanTime_;
}Ble;

void ble_process(Ble *ble);
void ble_retry(bleStatus state, Ble *ble, char *buffer);
//char* ble_retry(char* buffer, bleStatus state);