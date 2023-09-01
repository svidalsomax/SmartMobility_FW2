#include "ble.h"

/*
// ---- Maquina de estados del Ble. Se llama constantamente en el process machine del main
*/
void ble_process(Ble *ble){
	strcat((*ble).rxBuffer_, "hola"); //rxBuffer debería leer lo del UART. 
	char buffer[MAX_MESSAGE_LENGTH]; 
	switch ((*ble).bleState_) //definir el blestate_ 
	{
		case BLE_BEGIN:
		{
			//definir buffer AT
			strcpy(buffer, "BLE_BEGIN");
			//strcpy(ble->state_, "BLE_begin");
            //printf(buffer);
            ble_retry(BLE_RENEW, ble, buffer); 
			break;	
		}
		case BLE_RENEW:
		{
			//definir algo para ver el estado del state
			strcpy(buffer, "AT_RENEW");
			//strcpy(ble->state_, "BLE_RENEW");
            //printf(buffer);
            ble_retry(BLE_BEGIN, ble, buffer);
			break; 
		}		
		default: 
		{
			break; 			
		}
	}
}

/*
// ---- Se añaden los escaneos del Ble al vector beaconScan y se borra la info de scan del Ble
*/
ble_Scan ble_getSecan(Ble *ble) {
	ble_process(ble);
	ble_Scan beaconScan = (*ble).scan_;
	//(*ble).scan_ clear acá ver como hacer clear de scan  
	return beaconScan;
}

//parse ,... 

/*
// ---- Setea nombre del Ble
*/
void ble_setName(char *name){ 
	char buffer[MAX_MESSAGE_LENGTH];
	strcpy(buffer, "AT+NAME");
	strcat(buffer, name); 
	//uart_.write(&buffer); cambiar por io_write?? 
	//delay(TIME_TO_DELAY); esta linea sirve solo para MC Studio
}


/*
// ---- Setea baudios de comuniación del Ble
*/
bool ble_setBaud(unsigned long baud){
	char buffer[MAX_MESSAGE_LENGTH];
	char answer[MAX_MESSAGE_LENGTH];
	switch (baud)
	{
	case 9600:
		strcpy(buffer, "AT+BAUD0");
		strcpy(answer, "Ok+Set:0");
		break;

	case 19200:
		strcpy(buffer, "AT+BAUD1");
		strcpy(answer, "Ok+Set:1");
		break;

	case 38400:
		strcpy(buffer, "AT+BAUD2");
		strcpy(answer, "Ok+Set:2");
		break;

	case 57600:
		strcpy(buffer, "AT+BAUD3");
		strcpy(answer, "Ok+Set:3");
		break;

	case 115200:
		strcpy(buffer, "AT+BAUD4");
		strcpy(answer, "Ok+Set:4");
		break;

	case 4800:
		strcpy(buffer, "AT+BAUD5");
		strcpy(answer, "Ok+Set:5");
		break;

	case 2400:
		strcpy(buffer, "AT+BAUD6");
		strcpy(answer, "Ok+Set:6");
		break;

	case 1200:
		strcpy(buffer, "AT+BAUD7");
		strcpy(answer, "Ok+Set:7");
		break;

	case 230400:
		strcpy(buffer, "AT+BAUD8");
		strcpy(answer, "Ok+Set:8");
		break;

	default:
		strcpy(buffer, "AT+BAUD0");
		strcpy(answer, "Ok+Set:0");
		break;
	}

	/*  TODO LO SIGUIENTE DEBERÍA PODER ENCAPSULARSE EN LA FUNCIÓN BLE_SEND_RESP DE LANEK      
	io_write buffer;
	delay(TIME_TO_DELAY);
	char rx_Buffer[MAX_MESSAGE_LENGTH] = io_read;

	--- if para ver si la answer llegó bien ---
	if(rx_Buff.find(answer) != -1)
    {
        return true;
    }else{
        return false;
    }
	*/
    
    return false; 
}

bool ble_sleepMode(Ble *ble){ 
	char buffer[MAX_MESSAGE_LENGTH];
	strcpy(buffer, "AT+SLEEP");
	/*
	io_write(buffer);
	delay(TIME_TO_DELAY);
	io_write(buffer);
	delay(TIME_TO_DELAY);

	char buffer[MAX_MESSAGE_LENGTH] = io_read;
	
	    if(answer.find("OK+SLEEP") != -1)
    {
        return true;
    }else
    {
        return false;
    }
    return false;
	*/
	return false;
}

bool wakeUp(Ble *ble){
	char buffer[MAX_MESSAGE_LENGTH];
	strcpy(buffer, "AAAAAAT"); 
	/*
	uart_.write(&buffer);
    delay(TIME_TO_DELAY);
    delay(TIME_TO_DELAY);


	char answer[MAX_MESSAGE_LENGTH] = io_read();
    if(strcmp(answer, "OK+WAKE"))
    {
        return true;
    }else
    {
       return false;
    }
    return false;

	*/
}; 

/*
// ---- Setea valor del timer
*/
void ble_set_timer(unsigned long t, Ble *ble){
	(*ble).timer_ = t;
	//(*ble).time =  ;
}

/*
// ---- Indica si está listo el timer
*/
bool ble_timer(Ble *ble) 
{
	//return ((*ble).time_ + (*ble).timer_)< some like millis()
}

/*
// ---- Envía comando AT del ble definido antes
*/
void ble_retry(bleStatus state, Ble *ble, char *buffer){
	(*ble).bleState_ = state; 
    strcpy((*ble).state_, buffer);
    //command para enviar comando por uart
}