/*
 * Position.c
 *
 * Created: 15-09-2023 13:16:55
 *  Author: SVL / DQC
 */ 


#include "Position.h"
#include <stdio.h>
/**
* \brief Funcion que inicializa el objeto Position_Track con Position_Track.time_=0 y Position_Track.dt_=0
* \param Position_Track Paso de struct de Position_Track por referencia. Debe haber sido declarado anteriormente en el programa.
*/
void Position_Track_init(Position_Track * position_Track){
	position_Track->time_ = 0;
	position_Track->dt_ = 0;
}

/**
* \brief Funcion que inicializa el objeto Position con latitude = 0 y longitude =180.
* \param Position Paso de struct de Position por referencia. Debe haber sido declarado anteriormente en el programa.
*/
void Position_Init_Default(Position * position){
	position->latitude_ = 0.;
	position->longitude_ = 180.;
}

/**
* \brief Funcion que inicializa el objeto Position con latitude y longitude según los valores double entregados. Acá falta explicar respecto a lo del Overflow.
*/
void Position_Init_Coordenate(Position * position, Position * overflow, double latitude, double longitude){
	if ((latitude < -90.) || (latitude > 90.) || (longitude < -180.) || (longitude > 180.)){
		position->latitude_=overflow->latitude_;
		position->longitude_=overflow->longitude_;
	}
	else {
		position->latitude_ = latitude;
		position->longitude_ = (longitude == 180.) ? -180. : longitude;
	}
}

/**
* \brief Funcion que inicializa el objeto Position entregando una cadena de char y el objeto posición al cual inicizaliar.
* \param position Puntero a objeto de posición que se desea inicializar
* \param position_string cadena de caracteres que inizializa el objeto
*/
void Position_Init_Str(Position * position, char * position_string){
	loadRaw(position, position_string);
}

/**
* \brief Funcion que carga la posición proveniente en cadena de char de la simcom. Toma los campos esperados de la simcom y los transforma a números para guardarlos en los parámetros del objeto.
*/
void loadRaw(Position * position, char * position_string){
	char begin_position_string [10];
	strncpy(begin_position_string, position_string, 10);
	if (strcmp(begin_position_string,"+GPSINFO:") == 0)
	{
		position->latitude_ = 34566;
		printf("enTRA CASO 1");
	}
	else{
		position->latitude_= 200;
		printf("enTRA CASO 2");
	}
}

void Position_is_equal(Position * pos1, Position * pos2){

}