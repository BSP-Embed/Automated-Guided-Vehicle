#ifndef _APP_H
#define _APP_H

/* Main settings */

/*comment below line for release*/

#define EEPROM_DISPLAY				0

/******** MAIN CONFIGURATION ENDS ****************/

/* LCD CONFIGURATION */
#define LCD_DDR           	   		DDRB 
#define LCD_DATA_PORT          		PORTB
#define LCD_ENABLE_PIN        		PB3 
#define LCD_REGISTER_SELECT_PIN     PB2
#define DISPLAY_INIT				0
#define LCD_NSCRL					2
#define FLOAT_T0_STRING				1
#define DECIMAL_PRECISION			10

/******** LCD CONFIGURATION ENDS ****************/

/* BUZZER: CONFIGURATION */
#define	BUZ_DDR						DDRC
#define	BUZ_PORT					PORTC
#define	BUZ_PIN						PC1
#define OFF_TIME					100
/******** BUZZER CONFIGURATION ENDS ****************/

/* LED: CONFIGURATION */

#define	LED_DDR			DDRC
#define	LED_PORT		PORTC
#define	LED_PIN			PC0

#define OFF_TIME		100
/******** LED CONFIGURATION ENDS ****************/

/* GSM: CONFIGURATION */

#define MSG_START					""
#define MSG_END						". Automated SMS By: Guided Vehicle using RFID system."

/******** ADC CONFIGURATION ENDS ****************/
#define REF_VOL_256

/******** GSM CONFIGURATION ENDS ****************/

/******** EEPROM CONFIGURATION ****************/
#define EEPROM_DISPLAY		0

/******** RFID  CONFIGURATION ****************/
//#define DISPLAY_TAGNUMBER	1

#endif
