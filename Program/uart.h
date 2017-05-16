#ifndef SERIALCOMMUNICATION_H
#define SERIALCOMMUNICATION_H

//INCLUDE HEADER FILE
#include "includes.h"

#define F_CPU		7372800l

//DEFINE CONSTANT
#define USART_BAUDRATE		9600
#define BAUD_PRESCALE		((F_CPU / (USART_BAUDRATE * 16UL)) - 1)

//FUNCTION PROTOTYPES
void	 uartinit	(void);
void 	 putchar	(int8u ch);
int8u	 getchar	(void);
void 	 puts		(int8u *s);

#endif
