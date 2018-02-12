#ifndef Mr_Servo
#define Mr_Servo

void servo_initialize()
{
	TCCR1A |= (1<<WGM10);                      // Fast PWM 8bit
	TCCR1B |= (1<<WGM12);
	TCCR1A |= (1<<COM1A1)|(1<<COM1B1) ;        //Clear OC1A/OC1B on Compare Match, set OC1A/OC1B at BOTTOM
	//TCCR1B |= (1<<CS11);
	TCCR1B |= (1<<CS12)|(1<<CS10);
	
	
	//licznik 2
	TCCR0  |= (1<<WGM01)|(1<<WGM00)|(1<<COM01)|(1<<CS00)|(1<<CS02);//|(1<<CS02);

	TCCR2  |= (1<<WGM21)|(1<<WGM20)|(1<<COM21)|(1<<CS20)|(1<<CS21)|(1<<CS22);
	
	//DDRD|=(1<<PD4)|(1<<PD5);   //PWM Pins as Out
	DDRB = 0xFF;				// initialize port B
	DDRD = 0xFF;
//	OCR2 = 20;
//	_delay_ms(900);
//	OCR2=255;
	OCR1A = 255;   //0 degree
//	OCR1B = 255;
}

void servo_speed(int x, int y)
{
//	OCR1A = x;   //0 degree
//	OCR1B = y;
	OCR2 = x;
	OCR0 = y;
	//OCR0 = 10;
	
}
void servo_ultrasonic(int a)
{
	servo_initialize();
	OCR1A = a;   //0 degree
//	OCR1B = y;
//	OCR2 = a;
//	OCR0 = a;
	//_delay_ms(8000);
	//OCR2 = 255;
}


#endif