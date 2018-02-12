#ifndef Mr_ADC
#define Mr_ADC
//#include <avr/io.h>

//#include <util/delay.h>

void InitADC()
{

  ADMUX=(1<<REFS0);                                  	// For Aref=AVcc;
  
  ADCSRA=(1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0); // perscaler 128. 160 mhz/128 = 126khz
}

uint16_t pomiar(uint8_t ch)
{
	//Select ADC Channel ch must be 0-7
	ch=ch&0b00000111;
	ADMUX&=0b11100000;
	ADMUX|=ch;

	//Start Single conversion
	ADCSRA|=(1<<ADSC);

	//Wait for conversion to complete
	while(!(ADCSRA & (1<<ADIF)));

	//Clear ADIF by writing one to it
	//Note you may be wondering why we have write one to clear it
	//This is standard way of clearing bits in io as said in datasheets.
	//The code writes '1' but it result in setting bit to '0' !!!

	ADCSRA|=(1<<ADIF);

	return(ADC);		
}

void ADC_to_LCD(int measure,int count, int x, int y)
{
		//LCD_Clear();
		//LCD_Update();
		int a;
		char pomiar0[10];
		char liczba[4];	
		a = pomiar(measure);
		a = pomiar(measure);
		itoa(a,pomiar0,10);
		itoa(count,liczba,10);
		LCD_GotoXYFont(x,y);
		LCD_FStr(FONT_1X,(unsigned char*)PSTR("Pom: "));
		
		LCD_String(FONT_1X,liczba);
		LCD_FStr(FONT_1X,(unsigned char*)PSTR(": "));
		LCD_String(FONT_1X,pomiar0);
	//	_delay_ms(5);
		LCD_Update();		
}
void ADC_IR_Sensors()
{
	int a0 = pomiar(1);
	
	_delay_ms(1);
	int a1 = pomiar(2);
	
	char pomiar0[10];
	char pomiar1[10];
	
	itoa(a0,pomiar0,10);
	itoa(a1,pomiar1,10);
	
	LCD_GotoXYFont(1,1);
	LCD_String(FONT_1X,pomiar0);
	LCD_GotoXYFont(1,2);
	LCD_String(FONT_1X,pomiar1);
	LCD_Update();
	_delay_ms(200);
	LCD_Clear();
	LCD_Update();
	 
	
	
	
}

/*
void Distance_cm(int pin, int x, int y)
{

	int a;
	char pomiar0[10];
	char liczba[4];
	
	a = pomiar(pin);
	
	
	
	LCD_GotoXYFont(1,3);
	LCD_FStr(FONT_1X,(unsigned char*)PSTR("a: "));
	char naA[10];
	itoa(a,naA,10);
	LCD_String(FONT_1X,naA);
	
	a /=4;
	
	
	
	itoa(a,pomiar0,10);
	LCD_GotoXYFont(x,y);
	LCD_FStr(FONT_1X,(unsigned char*)PSTR("Odleglosc: "));
	
	
	LCD_String(FONT_1X,pomiar0);
	LCD_Update();
	
	
}
*/
#endif