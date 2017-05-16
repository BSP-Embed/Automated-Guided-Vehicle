#ifndef ADC_H
#define ADC_H

//INCLUDE HEADER FILE
#include "includes.h"

//USER DEFINED DATA TYPE
typedef unsigned char ADCUbyte;

//DEFINE CONSTANT
#define AVERAGEVALUE 	64

//FUNCTION PROTOTYPE
void 	adcinit	(void);
int16u 	adcget	(int8u ChNum);

#endif

