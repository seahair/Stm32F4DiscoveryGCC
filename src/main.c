#include "stm32f4xx.h"
#include "hardwareinit.h"
#include "led.h"
#include "usart.h"
#include "delay.h"
#include "beep.h"
#include "key.h"
/*#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_flash.h"
#include "usart.h"
#include "delay.h"
#include "exti.h"
#include "time.h"
#include "pwm.h"
#include "capture.h"
#include <stdio.h>
#include <stdlib.h>
#include "pad.h"
#include "sram.h"*/

void MyKeyTest( u8 key );

int main(int argc, char *argv[])
{
	
	HardInit();
	while(1)
	{
<<<<<<< HEAD
		delay_ms(1000);
		KeyTest( MyKeyTest );	
	}
}

	
=======
		//delay_ms(1000);
		KeyTest( MyKeyTest );	


	}
}

>>>>>>> f95aa9d0679305427ea9271eb560c1da698539a0
void MyKeyTest( u8 key )
{
	switch (key)
	{
		case KEY0:
				LedRed.LedBlink( &LedRed, 1000 );
			break;
		case KEY1:
<<<<<<< HEAD
				LedGreen.LedBlink( &LedGreen, 5000 );
			break;
		case KEY2:
				LedRed.LedOFF( &LedRed );
=======
				LedGreen.LedBlink( &LedGreen, 500 );
			break;
		case KEY2:
				LedRed.LedRollBack( &LedRed );
>>>>>>> f95aa9d0679305427ea9271eb560c1da698539a0
			break;
		case KEYWK:
				Beep.PlayMusic( );	
			break;
	}
}


