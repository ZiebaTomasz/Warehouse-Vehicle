#ifndef Mr_ServoDefinition
#define Mr_ServoDefinition

void turn_forward(){
	servo_speed(255,255);
	PORTC |= (1<<PC1); //silnik lewy w przod
	PORTC |= (1<<PC2); //silnk prawy w przod
	
	PORTC &= ~(1<<PC0);
	PORTC &= ~(1<<PC3);	
}

void turn_backward(){
	servo_speed(255,255);
	PORTC |= (1<<PC0); //silnik lewy w tyl
	PORTC |= (1<<PC3); //silnk prawy w tyl
	
	PORTC &= ~(1<<PC1);
	PORTC &= ~(1<<PC2);
}

void turn_left(){
	servo_speed(255,255);
	
	PORTC |= (1<<PC1); //silnk prawy w przod
		
	PORTC &= ~(1<<PC0);
	PORTC &= ~(1<<PC2);
	PORTC &= ~(1<<PC3);
	
}
void turn_soft_left(){
	servo_speed(255,90);
	
	PORTC |= (1<<PC1); //silnik lewy w przod
	PORTC |= (1<<PC2); //silnk prawy w przod
	
	PORTC &= ~(1<<PC0);
	PORTC &= ~(1<<PC3);
	
}

void turn_right(){
	servo_speed(255,255);
	PORTC |= (1<<PC2); //silnk prawy w przod

	PORTC &= ~(1<<PC0);
	PORTC &= ~(1<<PC1);
	PORTC &= ~(1<<PC3);
}

void turn_soft_right(){
	servo_speed(90,255);
	PORTC |= (1<<PC1); //silnik lewy w przod
	PORTC |= (1<<PC2); //silnk prawy w przod
	
	PORTC &= ~(1<<PC0);
	PORTC &= ~(1<<PC3);
}

void turn_around(){
		servo_speed(255,255);
	PORTC |= (1<<PC1);
	PORTC |= (1<<PC3);
		
	PORTC &= ~(1<<PC0);
	PORTC &= ~(1<<PC2);
	_delay_ms(2000);
	PORTC &= ~(1<<PC0);
	PORTC &= ~(1<<PC1);
	PORTC &= ~(1<<PC2);
	PORTC &= ~(1<<PC3);
	
	
}

void turn_around_left(){
	servo_speed(253,254);
		PORTC |= (1<<PC1);
		PORTC |= (1<<PC3);
		
		PORTC &= ~(1<<PC0);
		PORTC &= ~(1<<PC2);
}

void turn_around_right(){
	servo_speed(255,255);
	PORTC |= (1<<PC0);
	PORTC |= (1<<PC2);

	PORTC &= ~(1<<PC1);
	PORTC &= ~(1<<PC3);
		
}

void turn_STOP(){
	servo_speed(255,255);
	PORTC &= ~(1<<PC0);
	PORTC &= ~(1<<PC1);
	PORTC &= ~(1<<PC2);
	PORTC &= ~(1<<PC3);

	

	
}


void slow_turn_forward(){
	servo_speed(190,190);
	PORTC |= (1<<PC1); //silnik lewy w przod
	PORTC |= (1<<PC2); //silnk prawy w przod
	
	PORTC &= ~(1<<PC0);
	PORTC &= ~(1<<PC3);
}
void slow_turn_left(){
	servo_speed(190,190);
	
	PORTC |= (1<<PC1); //silnk prawy w przod
	
	PORTC &= ~(1<<PC0);
	PORTC &= ~(1<<PC2);
	PORTC &= ~(1<<PC3);
	
}
void slow_turn_soft_left(){
	servo_speed(190,90);
	
	PORTC |= (1<<PC1); //silnik lewy w przod
	PORTC |= (1<<PC2); //silnk prawy w przod
	
	PORTC &= ~(1<<PC0);
	PORTC &= ~(1<<PC3);
	
}

void slow_turn_right(){
	servo_speed(190,190);
	PORTC |= (1<<PC2); //silnk prawy w przod

	PORTC &= ~(1<<PC0);
	PORTC &= ~(1<<PC1);
	PORTC &= ~(1<<PC3);
}

void slow_turn_soft_right(){
	servo_speed(90,190);
	PORTC |= (1<<PC1); //silnik lewy w przod
	PORTC |= (1<<PC2); //silnk prawy w przod
	
	PORTC &= ~(1<<PC0);
	PORTC &= ~(1<<PC3);
}







void ultrasonic_center(){
	servo_ultrasonic(20);	
//	_delay_ms(3000);
//	servo_ultrasonic(255);
}
void ultrasonic_left()
{
	servo_ultrasonic(34);
	//_delay_ms(3000);
//	servo_ultrasonic(255);
	
}
void ultrasonic_right()
{
	servo_ultrasonic(7);
//	_delay_ms(3000);
//	servo_ultrasonic(255);
}
void ultrasonic_take_value(int position)
{
	int d = position - 23;
	if (position>23){
		position = 23 - d;
	}else if (position<23){
		position = 23 - d;
	}
	servo_ultrasonic(position);
}

#endif
