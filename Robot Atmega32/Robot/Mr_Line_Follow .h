#ifndef Mr_Line_Follow
#define Mr_Line_Follow

#define true  1
#define false 0

volatile bool sensor1 = false; 
volatile bool sensor2 = false;
volatile bool sensor3 = false;
volatile bool sensor4 = false;
volatile bool sensor5 = false;

void goOntheLine(){
	valueToLogic();
	goTo();	
}


void valueToLogic(){
	if (pomiar(1) > 150)
	{
		sensor1 = false;
	}else{
		sensor1 = true;
	}
	
	if (pomiar(2) > 150)
	{
		sensor2 = false;
	}else{
		sensor2 = true;
	}
	
	if (pomiar(3) > 435)
	{
		sensor3 = false;
	}else{
		sensor3 = true;
	}
	
	if (pomiar(4) > 210)
	{
		sensor4 = false;
	}else{
		sensor4 = true;
	}
	
	if (pomiar(5) > 170)
	{
		sensor5 = false;
	}else{
		sensor5 = true;
	}			
}
void goTo(){
	if (sensor1 && sensor2 && sensor3 && sensor4 && sensor5){
		turn_STOP();
	}
	else if(sensor1 || sensor2){
		turn_left();
	}

	else if(sensor4 || sensor5){
		turn_right();

	}
	else if(sensor3){
		turn_forward();

	}else{
		turn_forward();
	}					
}

void printLogic(){
	LCD_Clear();
	valueToLogic();
	
	if (sensor1){
		LCD_GotoXYFont(1,1);
		LCD_FStr(FONT_1X,(unsigned char*)PSTR("sensor1: true"));
	}
	else if (!sensor1){
		LCD_GotoXYFont(1,1);
		LCD_FStr(FONT_1X,(unsigned char*)PSTR("sensor1: false"));
	}
	if (sensor2){
		LCD_GotoXYFont(1,2);
		LCD_FStr(FONT_1X,(unsigned char*)PSTR("sensor2: true"));
	}
	else if (!sensor2){
		LCD_GotoXYFont(1,2);
		LCD_FStr(FONT_1X,(unsigned char*)PSTR("sensor2: false"));
	}
	if (sensor3){
		LCD_GotoXYFont(1,3);
		LCD_FStr(FONT_1X,(unsigned char*)PSTR("sensor3: true"));
	}
	else if (!sensor3){
		LCD_GotoXYFont(1,3);
		LCD_FStr(FONT_1X,(unsigned char*)PSTR("sensor3: false"));
	}
	if (sensor4){
		LCD_GotoXYFont(1,4);
		LCD_FStr(FONT_1X,(unsigned char*)PSTR("sensor4: true"));
	}
	else if (!sensor4){
		LCD_GotoXYFont(1,4);
		LCD_FStr(FONT_1X,(unsigned char*)PSTR("sensor4: false"));
	}
	if (sensor5){
		LCD_GotoXYFont(1,5);
		LCD_FStr(FONT_1X,(unsigned char*)PSTR("sensor5: true"));
	}
	else if (!sensor5){
		LCD_GotoXYFont(1,5);
		LCD_FStr(FONT_1X,(unsigned char*)PSTR("sensor5: false"));
	}
	LCD_Update();
					
	
	_delay_ms(1000);
	
	
	
}


void skipObstacle(){
	


int	distance = ultrasonicReadDistance();
	distance = ultrasonicReadDistance();


	char percentString[5];
	itoa(distance,percentString,10);
	
	LCD_GotoXYFont(2,5);
	
	LCD_String(FONT_1X,percentString);
	LCD_String(FONT_1X,"cm");
	
	LCD_Update();


	
	ultrasonic_center();
	_delay_ms(100);
	
	if (distance>20){
		turn_forward();
		_delay_ms(400);
		
	}else if(distance<=20){

		turn_STOP();
		_delay_ms(1300);
		
		ultrasonic_left();
		_delay_ms(1000);
		int left = ultrasonicReadDistance();
		
		
		ultrasonic_right();
		_delay_ms(1000);
		int right = ultrasonicReadDistance();
		
		if (left > right)
		{
			ultrasonic_center();
			turn_left();
			_delay_ms(1700);
		}else{
			ultrasonic_center();
			turn_right();
			_delay_ms(1700);
		}
	}
}



#endif
