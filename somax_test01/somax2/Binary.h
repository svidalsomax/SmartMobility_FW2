/*
 * Binary.h
 *
 * Created: 12-09-2023 13:04:29
 *  Author: SVL / DQ
 */ 

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#ifndef BINARY_H_
#define BINARY_H_

void encode(uint64_t integer, int size, char* string);

void encode(uint64_t integer, int size, char* string) {
	int i;
	for (i = 0; i < size; i++) {
		// Extrae un byte del número entero y lo asigna como un carácter en la cadena.
		string[i] = (char)((integer >> (i << 3)) & 0xFF);
	}
}

#endif /* BINARY_H_ */