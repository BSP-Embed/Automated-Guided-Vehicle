
#include "LCD.h"

static void  LCDEnable		(void);
static void  LCDReset		(void);
static void  LCDWriteByte	(char  ch);

int8u lcdptr;

static void  LCDEnable(void)
{
    LCD_DATA_PORT &= ~ (_BV(LCD_ENABLE_PIN)); 
    dlyus(1);
    LCD_DATA_PORT |=  _BV(LCD_ENABLE_PIN);  
}

void lcdwc(int8u x)
{
    LCD_DATA_PORT &= ~ (_BV(LCD_REGISTER_SELECT_PIN)); 
    LCDWriteByte(x);
}

void lcdwd(char ch)
{
    LCD_DATA_PORT |=  _BV(LCD_REGISTER_SELECT_PIN);
    LCDWriteByte(ch);
}

void lcdws(char *s){
	while (*s) {
			#ifdef LCD_20X4
		switch(lcdptr) {
			case 0x80+20:
				lcdptr = 0xC0;
				break;
			case 0xC0+20:
				lcdptr = 0x94;
				break;
			case 0x94+20:
				lcdptr = 0xD4;
				break;
			case 0xD4+20:
				lcdptr = 0x80;
				break;
		}
		#else
			switch(lcdptr) {
				case 0x80+16:
				lcdptr = 0xC0;
				break;
				case 0xC0+16:
				lcdptr = 0x80;
				break;
			}
		#endif
		lcdwc(lcdptr++);
		lcdwd(*s++);
	}
}

void lcdinit(void)
{
    LCD_DDR = (LCD_DDR & 0x03) | 0xfc;
	LCD_DATA_PORT    &=  0x03;

	dlyms(40);
	LCDReset();
    lcdwc(0x28);
    lcdwc(0x0C);
    lcdwc(0x06);
    lcdwc(0x01); 
	lcdclr();
	#if DISPLAY_INIT > 0
		lcddinit();
	#endif
}
static void LCDReset(void)
{
	lcdwc(0x33);
	dlyms(5);
	lcdwc(0x33);
	dlyus(500);
	lcdwc(0x32);
	dlyus(500);
}

void lcddinit(void)
{
	int8u i, j, adr;
	
	lcdws("  INITIALIZING");
	lcdr2();
	for ( j = 0; j < LCD_NSCRL; j++ ) {
		adr = 0xc0;						// 2nd row, first coloumn
		for ( i = 0; i < 16; i++ ) {
			lcdwc(adr);			
			lcdwd(LCD_DOT);				
			if ( i < 8 ) dlyms(200+(50*i)); else dlyms(25);
			lcdwc(adr);			
			lcdwd(LCD_SPC);			
			adr++;					// increment display aadress
		}
	} 
	lcdclr();
}

static void  LCDWriteByte(char  LCDData)
{
    LCD_DATA_PORT = (LCD_DATA_PORT & 0x0F)|(LCDData & 0xF0);
    LCDEnable();
    LCD_DATA_PORT = (LCD_DATA_PORT & 0x0F)|((LCDData << 4) & 0xF0);
    LCDEnable();
    dlyus(750);
}
void lcdclr(void)
{
	lcdwc(0x01);
	lcdptr = 0x80;	  /* Clear display LCD */
}
void lcdclrr(int8u rowno){
	int8u i;
	switch (rowno) {
		case 0:
				lcdr1();
				break;
		case 1:
				lcdr2();
				break;
	}
	for (i = 0; i < 16; i++)
		lcdwd(LCD_SPC);
	switch (rowno) {
		case 0:
				lcdr1();
				break;
		case 1:
				lcdr2();
				break;
	}
 }
 void lcdr1(void)
 {
	lcdwc(0x80); 	  /* Begin at Line 1 */
	lcdptr = 0x80;
 }
  void lcdr2(void)
 {
	lcdwc(0xc0); 	  /* Begin at Line 1 */
	lcdptr = 0xc0;
 }
 void lcdwint(int8u loc,signed int x)
 {
	 int8u str[10];
	 lcdptr = loc;
	 lcdws("   ");
	 lcdptr = loc;
	 itoa(x, str);
	 lcdws(str);
 }
 void lcdwlng(int8u loc,unsigned long x)
 {
	 int8u str[24];
	 lcdptr = loc;
	 lcdws("           ");
	 lcdptr = loc;
	 ltoa(x, str);
	 lcdws(str);
}

void ltoa(unsigned long n, char s[])
{
	int i;
	
	i = 0;
	do {
		s[i++] = n % 10 + '0';
	} while ((n /= 10) > 0);

	s[i] = '\0';
	reverse(s);
}
 void itoa(signed int n, char s[])
{
	int i, sign;
	
	if ((sign = n) < 0)
		n = -n;
	i = 0;
	do {
		s[i++] = n % 10 + '0';
	} while ((n /= 10) > 0);
	if (sign < 0)
		s[i++] = '-';
	s[i] = '\0';
	reverse(s);
}
#if FLOAT_T0_STRING > 0
void ftoa(float n, char s[])
{
	char temp[6];
	int i, sign;
	int x,y;
	
	if ((sign = n) < 0)
		n = -n;
	
	y = x = n;
	i = 0;
	do {
		s[i++] = x % 10 + '0';
	} while ((x /= 10) > 0);
	
	if (sign < 0)
		s[i++] = '-';
	
	s[i] = '\0';
	reverse(s);
	
	x = (n - y) * DECIMAL_PRECISION;
	s[i++] = '.';
	s[i] = '\0';
	
	i = 0;
	do {
		temp[i++] = x % 10 + '0';
	} while ((x /= 10) > 0);
	temp[i] = '\0';

	reverse(temp);


	strcat(s,temp);

} 
#endif
/* atof: convert string s to double ; pointer version */
double atof(char *s)
{
	double val,power;
	int sign;
	
	for(; isspace(*s); s++);	/* skip white spaces */
	sign = (*s == '-')? -1:1;
	if (*s == '+' || *s == '-')
		s++;
	for (val = 0.0; isdigit(*s); s++)
		val = 10.0 * val + (*s - '0');
	if (*s == '.')
		s++;
	for (power = 1.0; isdigit(*s); s++) {
		val = 10.0 * val + (*s - '0');
		power *= 10.0;
	}
	return sign * val / power;
}

	 
void reverse(char s[])
{
	int c,i,j;
	for (i = 0, j = strlen(s)-1; i < j; i++,j--) 
		c = s[i], s[i] = s[j], s[j] = c;
} 
