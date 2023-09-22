/*
 * Position.h
 *
 * Created: 15-09-2023 13:17:11
 *  Author: SVL / DQC
 */ 

// INCLUDES
#include <string.h>
#include <stdbool.h>
//#include<vector> ?? no se usa vector, pero vamos a asumir ciertos structs como vectores de dos dimensiones

#ifndef POSITION_H_
#define POSITION_H_

typedef struct{
    double latitude_; 
    double longitude_;
    //Position zero; 
    //Position overflow;
    char absoluteZero[20]; 
    char absoluteOverflow[20];
} Position; 

typedef struct {
    unsigned long time_; 
    unsigned long dt_; 
    Position positionVector_;
} Position_Track; 

void Position_Track_init(Position_Track * position_Track);
void Position_Init_Default(Position * position); 
void Position_Init_Coordenate(Position * position, Position * overflow, double latitude, double longitude);
void Position_Init_Str(Position * position, char * position_string);

//arreglar esto
int loadRaw(Position * position, char * position_string);
void binary_position(Position * position, char binary[6]);

bool position_is_equal(Position * position1, Position * position2);
bool position_is_not_equal(Position * position1, Position * position2);

/*
double latitude(Position * position); 

double longitude(Position * position);
*/

extern Position overflow; //inizializar el overflow al principio
extern Position zero;

#endif /* POSITION_H_ */