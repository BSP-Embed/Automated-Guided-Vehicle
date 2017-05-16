#ifndef MAIN_H
#define MAIN_H

#include "includes.h"

#define GSM_RFID_DDR			DDRD
#define GSM_RFID_PORT			PORTD
#define GSM_RFID_PIN			PD2

#define MOTOR_PORT				PORTC
#define MOTOR_DDR				DDRC

#define RLY_DDR				DDRD
#define RLY_PORT			PORTD
#define RLY_PIN				PD7

#define	ULTSEN_DDR			DDRD
#define	ULTSEN_PORT			PORTD
#define	ULTSEN_PIN			PIND
#define ECHO_PIN			PD3
#define TRIG_PIN			PD4

//DEFINE CONSTANT
#define ULTRASONIC_CONST	0.01724
#define BODY_WEIGHT			0.4
#define DIST_THRESHOLD		0.3


//DEFINE MACROS
#define StartTmr()			TCCR0  	|= _BV(CS01)
#define StopTmr()			TCCR0  	&= ~_BV(CS01)


#define EnGSM()				do {									\
								GSM_RFID_PORT &= ~_BV(GSM_RFID_PIN); \
								dlyms(1000);						\
							} while(0)

#define EnRFID()			do {									\
							GSM_RFID_PORT |= _BV(GSM_RFID_PIN);		\
							EnUARTInt();							\
							} while(0)

#define sendSMS(x)			do {						\
							DisUARTInt();				\
							EnGSM();					\
							GSMsndmsg(PhNum, MSG[x-1]);	\
							dlyms(1000);				\
							EnRFID();					\
							} while(0)

#define EnUnRFID()			do {						\
							RLY_PORT |= _BV(RLY_PIN); 	\
							dlyms(500);					\
							EnUARTInt();				\
							} while(0)					\

#define DisUnRFID()			do {						\
							RLY_PORT &= ~_BV(RLY_PIN); 	\
							dlyms(100);					\
							} while(0)					\

//FUNCTION PROTOTYPES
static void		 init		(void);
static void 	 disptitl 	(void);
static void 	 tmr1init	(void);
static void  	gpioinit	(void);
static void 	stop		(void);
static void 	Forward		(void);
static void 	Right		(void);
static void 	Left		(void);
static float 	measdist	(int8u disp);
static long		measwt		(int8u disp);
static void 	tmr0init	(void);
static int8u 	readtag		(void);
static void 	EnUARTInt	(void);
static void 	DisUARTInt	(void);
static void 	TaskMove	(void);
static void		TaskDesti	(void);
static void 	TestOVLoad	(void);
static int8u	veritag		(char *tag_temp, char *tag);


#endif
