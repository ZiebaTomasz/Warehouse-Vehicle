#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <string.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <stdint.h>
#include "Mr_LCD_img.h"

#include "Mr_LCD.h"
#include "Mr_USART.h"
#include "Mr_Servo.h"
#include "Mr_ADC.h"
#include "Mr_Commands.h"
#include "Mr_Battery.h"
#include "Mr_ServoDefinition.h"
#include "Mr_Line_Follow .h"
#include "Mr_Ultrasonic.h"

volatile unsigned char data_count;
volatile unsigned char command_ready;



int main(void)
{	
	InitADC();
	servo_initialize();
	USART_init();
	LCD_Init();
	
//	DDRC |=(1<<PC0) | (1<<PC1) ;
//	DDRC |= (1<<PC2) | (1<<PC3);
///	PCD_Contr(0xff);
//	LCD_Clear();
//	LCD_Img(niuniek);

//	LCD_Update();
	


	DDRA = (1 << 6); /* Make trigger pin as output */
	PORTD = 0xFF;		/* Turn on Pull-up */
	
	
	LCD_Update();

		
	sei();			/* Enable global interrupt */
	

servo_ultrasonic(20);
_delay_ms(2000);
battery_voltage();

	while (1)
	{
					
		if (command_ready == TRUE)
		{
			copy_command();
			process_command();
			command_ready = FALSE;		
		}
		
	}
	
	
}


ISR (USART_RXC_vect)
{
	//volatile	int flaga;
	// Get data from the USART in register
	data_in[data_count] = UDR;

	// End of line!
	if (data_in[data_count] == '\n') {
		 flaga = 0;
		command_ready = TRUE;
		
		// Reset to 0, ready to go again
		data_count = 0;
		} else {
		data_count++;
	}
}
