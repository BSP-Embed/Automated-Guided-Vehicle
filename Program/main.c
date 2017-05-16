#include "main.h"

#define TESTING

#ifdef TESTING
	#define PhNum	 "9902899863"
#else
	#define PhNum	 "9663727944"
#endif

/* RFID Tags for guiding */
const  char *tag[] = { "",
					   "150011840383",		
					   "1500117AE997",
					   "1500115B3F60",	
					   "150011566B39",	
					   "15001140D195",		
					   "150011840282",	
					/* RFID Tags for GOODS */	
					   "15001179C0BD",	
					   "150011743E4E",
					   "1500117D1F66",		
					    };

const char *MSG[] = { "Object Detected",
					  "Destination Reached",
					  "Wrong goods Detected"
					};

struct  {
	volatile int8u Desti:1;
	volatile int8u WGoods:1;
	volatile int8u SBUF:1;
	volatile int8u SMSObjDet:1;
}Flags;

volatile int16u ultpulse;
int8u sbuf[25];
int8u TagNumber;

extern int8u lcdptr;

int main(void)
{
	init();

	while (TRUE) {
		
		TaskMove();
		TaskDesti();
	}
	return 0;
}
static void TaskMove(void)
{
	Forward();
	EnRFID();

	while (!Flags.Desti) {

		if (measdist(0) < DIST_THRESHOLD) {
			if (!Flags.SMSObjDet) {
				stop();
			//	sendSMS(1);
				lcdclrr(1);
				Flags.SMSObjDet = 1;
			}
		} else { 
			Forward();
			Flags.SMSObjDet = 0;
		}

		if (Flags.SBUF) {				
			switch(readtag()) {
				case 1: case 2: case 5:	Forward(); break;
				case 3: Left(); break;
				case 4: Right(); break;
				case 6: stop(); Flags.Desti = 1; break;
			}
			Flags.SBUF = 0;
			EnUARTInt();
		}

	}
}	
static void TaskDesti(void)
{
	lcdclr();
	lcdws("Desti'n Reached");
	beep(1,100);
	dlyms(1000);
	sendSMS(2);
	lcdclrr(1);
	EnUnRFID();

	while (TRUE) {
		if (Flags.SBUF) {				
			switch(readtag()) {
				case 8: beep(1,250);
						lcdr2();
						lcdws("  Wrong Goods");
						DisUnRFID();
						dlyms(1000);
						sendSMS(3);
						lcdclrr(1);
						break;
			}
			Flags.SBUF = 0;
			EnUnRFID();

		}
	}	
}
	
static void init(void)
{
	buzinit();
	beep(2,100);

	ledinit();
	gpioinit();
	
	
	Flags.Desti = 0;
	Flags.WGoods = 0;
	Flags.SBUF = 0;
	Flags.SMSObjDet = 0;
	TagNumber = 0;

	lcdinit();
	uartinit();


//	EnGSM();
//	GSMinit();

	
	tmr1init();
	tmr0init();
	adcinit();

	
//	TestOVLoad();
	disptitl();

	sei();

}
static void TestOVLoad(void)
{
	static flag = 0;
	lcdclr();
	lcdws("  Testing Load");
	lcdr2();
	dlyms(1000);

	while (measwt(1) > 150) {
		if (!flag) {
			lcdws("OverLoad in Vehi");
			flag = 1;
		}
		beep(1,75);
	}
	lcdclr();
	lcdws("  Vehicle Will");
	lcdr2();
	lcdws("Move in 5Seconds");
	dlyms(5000);
}
		
static void gpioinit(void)
{
	
	MOTOR_DDR	|= 0xf0;
	MOTOR_PORT	&= 0x0f;

	GSM_RFID_DDR |= _BV(GSM_RFID_PIN);
	GSM_RFID_DDR &= ~_BV(GSM_RFID_PIN);
	
	ULTSEN_DDR 		|= _BV(TRIG_PIN);
	ULTSEN_DDR 		&= ~_BV(ECHO_PIN);

	ULTSEN_PORT		&= ~_BV(TRIG_PIN);
	ULTSEN_PORT		|= _BV(ECHO_PIN);

	RLY_DDR			|= _BV(RLY_PIN);
	RLY_PORT 		&= ~_BV(RLY_PIN);

}
static void disptitl(void)
{
	lcdclrr(0);
	lcdws("Auto Guided Vehi");
}
		
static void tmr1init(void)
{
	TCNT1H   = 0xD3;
	TCNT1L   = 0x00;
	TIMSK   |= _BV(TOIE1);			//ENABLE OVERFLOW INTERRUPT
	TCCR1A   = 0x00;					
	TCCR1B  |= _BV(CS10) | _BV(CS11); /* PRESCALAR BY 16 */
}
static void tmr0init(void)
{
	TCNT0   =  167;
	TIMSK   |= _BV(TOIE0);			//ENABLE OVERFLOW INTERRUPT
	
}
/* overflows at every 100msec */
ISR(TIMER1_OVF_vect) 
{ 
	static int8u i,j,k;

	TCNT1H = 0xD3;
	TCNT1L = 0x00;
	
	if (++i >= 50) i = 0;
	switch(i) {
		case 0: case 2: ledon(); break;
		case 1: case 3: ledoff(); break;
	} 
}
/* OverFlows every 100us */
ISR(TIMER0_OVF_vect) 
{ 
	TCNT0 = 167;
	++ultpulse;
}
static int8u readtag(void)
{
	int8u i;

	#if defined DISPLAY_TAGNUMBER >= 1
		int8u tag_count ;
		
		lcdclrr(1);
		lcdws(sbuf);
		for(;;);
	#endif
	
 	beep(1,100);

	for (i = 1; i <= 9; i++)
	  if(veritag(sbuf,tag[i]))
		return i;

	return 0;
  			 
}
static int8u veritag(char *tag_temp, char *tag)
{
	int8u i;
 

	for( i = 0; i < 12 ;i++)
	   	if(*tag_temp++ != *tag++)
			return 0;
		
	return 1;
}

ISR(USART_RXC_vect)
{
	static int8u i,cnt;

	if ((sbuf[i++] = UDR) && ++cnt == 12) {
		Flags.SBUF = 1;
		sbuf[i] = '\0';
		i = 0;
		cnt = 0;
		DisUARTInt();
	}
}
static void EnUARTInt(void)
{
	int8u tmp;
	tmp = UDR;
	tmp = UDR;
	tmp = UDR;
	UCSRB |= (1 << 7);	/* Enable receive complete INT */
}
static void DisUARTInt(void)
{
	UCSRB &= ~(1 << 7);	/* Enable receive complete INT */
}
static void Left(void)
{
	MOTOR_PORT = MOTOR_PORT & 0x0f | 0x50;
	beep(1,100);
	lcdr2();
	lcdws("  MOVING LEFT   ");
	dlyms(2500);
	Forward();
}

static void Right(void)
{
	MOTOR_PORT = MOTOR_PORT & 0x0f | 0xa0;
	beep(1,100);
	lcdr2();
	lcdws("  MOVING RIGHT  ");
	dlyms(2500);
	Forward();
}

static void Forward(void)
{
	MOTOR_PORT = MOTOR_PORT & 0x0f | 0x60;
	lcdr2();
	lcdws(" MOVING FORWARD ");

}
static void stop(void)
{
	MOTOR_PORT &= 0x0f;
	beep(2,250);
	lcdr2();
	lcdws("VEHICLE STOPPED ");
}
static float measdist(int8u disp)
{
	float dist;
	char s[10];

	ULTSEN_PORT		|= _BV(TRIG_PIN);
	dlyus(10);
	ULTSEN_PORT		&= ~_BV(TRIG_PIN);
	ultpulse = 0;

	while ((ULTSEN_PIN & _BV(ECHO_PIN)) == 0);
	StartTmr();
	while (ULTSEN_PIN & _BV(ECHO_PIN));
	StopTmr();
	
	dist = ultpulse * ULTRASONIC_CONST;
	
	if (disp == 1) {
		lcdptr = 0xc9;
		lcdws("    ");
		ftoa(dist,s);
		lcdptr = 0xc9;
		lcdws(s);
	}

	return dist;
}
static long measwt(int8u disp)
{
	int8u i;
	volatile signed long weight;


	weight = 0;
	for (i = 0; i < 8; i++)	
		weight += adcget(0x0B);
		 
	weight >>= 3;

	weight = weight - 800;
		 
 	if (disp) {
		lcdwint(0xc0,weight);
	} 
	
	return weight;	

}
