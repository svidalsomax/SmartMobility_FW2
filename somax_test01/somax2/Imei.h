/*
 * Imei.h
 *
 * Created: 11-09-2023 18:49:31
 *  Author: SVL / DQ
 */ 

/* 
 *
 * INCLUDES: 
 *
 */

#include <string.h>
#include <stdbool.h>

#ifndef IMEI_H_
#define IMEI_H_

/*
 * VARIABLES 
 */

typedef struct {
	unsigned long long imei_;
} Imei;


/*
 * MÉTODOS 
 */
void imei_init(Imei * imei, char * imei_str);
void imei_loadText(Imei * imei, char * imei_str);
void imei_binary(Imei * imei, char * encoded);
unsigned long long imei_value(Imei * imei);
bool imei_is_equal(Imei * imei1, Imei * imei2);
bool imei_is_not_equal(Imei * imei1, Imei * imei2);



#endif /* IMEI_H_ */