#ifndef Mr_Commands
#define Mr_Commands

unsigned int sensitivity = 223;
volatile unsigned int flaga;

			//InitADC();
void process_command()
{
	switch (command_in[0]) 
	{	
		case 't':
			if (command_in[1] == 'f') {	
				turn_forward();
				break;
			} 		
			else if (command_in[1] == 'b') {
				turn_backward();
				break;	
			}			
			else if (command_in[1] == 'l' )	{	
				turn_left();	
				break;						
			}	
			else if (command_in[1] == 'r' )	{
				turn_right();	
				break;		
			}		
			else if (command_in[1] == 'y' )	{
				turn_around_left();	
				break;			
			}		
			else if (command_in[1] == 't' )	{
				turn_around_right();
				break;			
			}	
			else if (command_in[1] == 's' ) {
				turn_STOP();
				break;			
			}
			
			else if (command_in[1] == 'z' )	{
				turn_soft_left();
				break;
			}
			else if (command_in[1] == 'c' ) {
				turn_soft_right();
				break;
			}
	
			
		case 'i':		
			if (command_in[1] == 'o' ) {
				flaga = 1;
				while (flaga ==1){
					ultrasonicToLCD();
					_delay_ms(300);	
				}
			}		 
		
			else if (command_in[1] == 'l' ){
				flaga = 1;
				while (flaga ==1){
					printLogic();
					ultrasonic_center();
				}		
			}
			else if (command_in[1] == 'x' ){
				flaga = 1;
				while (flaga ==1){
					goOntheLine();
				}
			}			
			
			else if (command_in[1] == 'n' ) {
				flaga = 1;
				while (flaga ==1){
			//		ADC_to_LCD(0,1,1);
						
					ADC_to_LCD(1,1,1,1);
						_delay_ms(20);				
		    	 	ADC_to_LCD(2,2,1,2);
						_delay_ms(29);
					ADC_to_LCD(3,3,1,3);
						_delay_ms(20);
					ADC_to_LCD(4,4,1,4);
						_delay_ms(20);
					ADC_to_LCD(5,5,1,5);
					_delay_ms(100);
					LCD_Clear();
				}
			}else if (command_in[1]=='b'){
				flaga = 1;
				while (flaga ==1){
					skipObstacle();
				}		
			}
						
			else if (command_in[1] == 's' ){
				flaga = 1;
				while (flaga ==1){
					ultrasonicToLCD();
				}
				
			} else {
					turn_STOP();
				
					LCD_Clear();
					LCD_GotoXYFont(0,1);
					LCD_FStr(FONT_2X,(unsigned char*)PSTR("STOP"));
					LCD_Update();
					
				//	battery_voltage();
						
			}
		
		case 'u':
			if (command_in[1] == 'c') //
			{
			//	readUltrasonic();
				ultrasonic_center();
				break;
			}
			else if (command_in[1] == 'l')
			{
				
				ultrasonic_left();
				break;
			}
			else if (command_in[1] == 'r')
			{
				ultrasonic_right();
				break;
			}
			else
			{
				
				int a = (command_in[1] - '0')*10;
				int b = (command_in[2] - '0')*1;
				int c=(a+b);
				
				

					char percentString[5];
					itoa(c,percentString,10);
					
					LCD_GotoXYFont(2,5);
					
					LCD_String(FONT_1X,percentString);
					LCD_String(FONT_1X,"cm");
					
					LCD_Update();
				
				servo_ultrasonic(c);
				

			}
		break;
		
			
		default:
		LCD_Clear();
		LCD_GotoXYFont(0,1);
		LCD_FStr(FONT_1X,(unsigned char*)PSTR("Nieznana komenda"));
		LCD_Update();
		break;	
				
			
				
			
			
	}
		
}





#endif