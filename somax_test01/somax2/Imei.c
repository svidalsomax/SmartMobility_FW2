/*
 * Imei.c
 *
 * Created: 11-09-2023 18:48:52
 *  Author: SVL / DQ 
 */ 

#include <stdlib.h>
#include "imei.h"
#include "Binary.h"

void imei_init(Imei * imei, char* imei_str){
	imei_loadText(imei, imei_str);
}

void imei_loadText(Imei * imei, char * imei_str){

	imei->imei_ = strtoll(imei_str, NULL, 10);
}

void imei_binary(Imei * imei, char * encoded){
	encode(imei->imei_, 7, encoded);
}

unsigned long long imei_value(Imei * imei){
	return imei->imei_;
}


//faltan los operatorss