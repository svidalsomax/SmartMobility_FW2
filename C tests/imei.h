/*
 *  Imei.h
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


#endif /* INCFILE1_H_ */