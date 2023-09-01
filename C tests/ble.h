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

//Beacon - used to create Beacon Vector initialized and used throght ble_Scan struct. 
typedef struct {
	unsigned long long mac_;
    int rssi_;
} Beacon;

//ble_Scan - used for ...
typedef struct{
	unsigned long scanTime_; 
	Beacon beaconVector[100];
}ble_Scan; 

//BLE Struct - Struct for everything
typedef struct{
	//from public: Functions to work with private specs
	/*
    auto process() -> void;   -> hecho pero no completo
    auto getScan() -> Scan;   -> falta funcion clear
    auto setName(std::string& name) -> void;   -> falta echar a andar en MC Studio y que funione el uart write
    auto setBaud(unsigned long baud) -> bool;   ->  hecho por Lanek?
    auto sleepMode() -> bool;    -> hecho por lanek? 
    auto wakeUp() -> bool;    -> hecho por lanek? 
    auto timer() -> bool;    -> ver de qué forma hacerlo en MC Studio
    auto setTimer(unsigned long t) -> void;   -> hecho pero no completo
    auto retry(std::string& name, bleStatus state) -> void;	   -> Hecho pero no completo
	*/

	//from pivate:
	char txBuffer_[64];
	char rxBuffer_[64];
	bleStatus bleState_;
	unsigned long timer_;
	unsigned long time_;
	int tryCounter_;
	char state_[64];
	bool role_ ;
	int scanTime_;
	ble_Scan scan_;

}Ble;



void ble_process(Ble *ble);
void ble_set_timer(unsigned long t, Ble *ble);
ble_Scan ble_getSecan(Ble *ble);
void ble_setName(char *name);
bool ble_setBaud(unsigned long baud);
bool ble_sleepMode(Ble *ble);
bool wakeUp(Ble *ble);
bool ble_timer(Ble *ble);
void ble_retry(bleStatus state, Ble *ble, char *buffer);