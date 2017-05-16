#include "adc.h"

void adcinit(void)
{
	ADMUX &= 0xE0;							//CHANNEL 0 MUX3...0 = 000
	ADMUX |= _BV(REFS0);					//5V Avcc INTERNAL REFERENCE WITH EXTERNAL CAPACITOR

	#ifdef REF_VOL_256
		ADMUX |= _BV(REFS1);
	#endif

	ADMUX &= ~_BV(ADLAR);					//ADC RIGHT ADJUST RESULT
	ADCSRA |= _BV(ADPS1) | _BV(ADPS2);		//PRESCALAR BY 8 GIVES 115KHz WITH 7.3728MHz Fcpu
	ADCSRA |= _BV(ADEN);					//ADC ENABLE
}
int16u adcget(unsigned char ChannelNumber)
{
	int16u adcval = 0;
	int8u i,adcLSB,adcMSB;

	adcinit();
	
	ADMUX = ((ADMUX & 0Xe0) | ChannelNumber);


	for (i = 0; i < AVERAGEVALUE; i++)	{

	
		ADCSRA |= _BV(ADSC);					//START CONVERSION

		while ((ADCSRA & _BV(ADIF)) == 0); 		//WAIT FOR CONVERSION TO COMPLETE
		ADCSRA |= _BV(ADIF);					//CLEAR FLAG BY WRITING 1 TO THE FLAG BIT

		adcLSB = ADCL;
		adcMSB = ADCH;
		adcval += (adcMSB * 256) + adcLSB;
	
	}

	ADCSRA &= ~_BV(ADEN);
	adcval >>= 6;

	return adcval;
	
}
