#ifndef Mr_Battery
#define Mr_Battery

void battery_voltage()
{

	int a;
	char firstString[5];
	char secondString[5];
	char aString[5]; 
	
	a = pomiar(0);
	a = pomiar(0);
	
	a /= 1.004; // a to voltage, no comma (736 to 733 V)
	int first = a/100;
	
	int second = a %(first*100);
//	second /= 10;
	

	itoa(first,firstString,10);
	itoa(second,secondString,10);
	itoa(a,aString,10);
	
	LCD_GotoXYFont(1,4);
	LCD_FStr(FONT_1X,(unsigned char*)PSTR(" V: "));
	LCD_String(FONT_1X,firstString);

	LCD_FStr(FONT_1X,(unsigned char*)PSTR(","));
	LCD_String(FONT_1X,secondString);	
	LCD_FStr(FONT_1X,(unsigned char*)PSTR("V"));
//	LCD_Update();
	
	int percent;
	if (a<662){
		percent = 0;
	} 
	else if (a >= 843){ // 8.4V; x 1,004
		percent = 100;
	}else
	{
		percent = (a - 662) * 0.552; 
	}
	
	
	char percentString[5];
	itoa(percent,percentString,10);
	
	LCD_GotoXYFont(2,2);
	
	LCD_String(FONT_2X,percentString);
	LCD_FStr(FONT_2X,(unsigned char*)PSTR("%"));
	
	LCD_Update();


}

#endif