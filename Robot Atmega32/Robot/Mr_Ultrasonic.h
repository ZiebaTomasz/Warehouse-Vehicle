#ifndef Mr_Ultrasonic
#define Mr_Ultrasonic


#define US_PORT PORTC
#define	US_PIN	PINC
#define US_DDR 	DDRC

#define US_TRIG_POS	PC4
#define US_ECHO_POS	PC5


/********************************************************************

This function measures the width of high pulse in micro second.

********************************************************************/

#define US_ERROR		-1
#define	US_NO_OBSTACLE	-2
uint16_t r;

void HCSR04Init();
void HCSR04Trigger();

void HCSR04Init()
{
	US_DDR|=(1<<US_TRIG_POS);
}

void HCSR04Trigger()
{
	//Send a 10uS pulse on trigger line
	
	US_PORT|=(1<<US_TRIG_POS);	//high
	
	_delay_us(10);				//wait 15uS
	
	US_PORT&=~(1<<US_TRIG_POS);	//low
}

uint16_t GetPulseWidth()
{
	uint32_t i,result;

	//Wait for the rising edge
	for(i=0;i<600000;i++)
	{
		if(!(US_PIN & (1<<US_ECHO_POS)))
		continue;	//Line is still low, so wait
		else
		break;		//High edge detected, so break.
	}

	if(i==600000)
	return US_ERROR;	//Indicates time out
	
	//High Edge Found

	//Setup Timer1
	TCCR1A=0X00;
	//	TCCR1B=(1<<CS11);	//Prescaler = Fcpu/8
	TCCR1B |= (1<<CS12)|(1<<CS10);
	TCNT1=0x00;			//Init counter

	//Now wait for the falling edge
	for(i=0;i<600000;i++)
	{
		if(US_PIN & (1<<US_ECHO_POS))
		{
			if(TCNT1 > 60000) break; else continue;
		}
		else
		break;
	}

	if(i==600000)
	return US_NO_OBSTACLE;	//Indicates time out

	//Falling edge found

	result=TCNT1;

	//Stop Timer
	TCCR1B=0x00;

	if(result > 60000)
	return US_NO_OBSTACLE;	//No obstacle
	else
	return (result>>1);
}
void ultrasonicToLCD(){
	
	int distance = ultrasonicReadDistance();
	char percentString[5];
	itoa(distance,percentString,10);
	
	LCD_GotoXYFont(2,5);
	
	LCD_String(FONT_1X,percentString);
	LCD_String(FONT_1X,"cm");
	
	LCD_Update();
	
//	servo_ultrasonic(20);

}
int ultrasonicReadDistance(){
	HCSR04Init();
	//Send a trigger pulse
	HCSR04Trigger();

	//Measure the width of pulse
	r=GetPulseWidth();

	int d;

	d=(r*2.0);	//Convert to cm  d=(r/58.0)
	return d;
}

#endif
