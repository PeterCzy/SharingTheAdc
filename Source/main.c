/*----------------------------------------------------------------------------
 *----------------------------------------------------------------------------*/
#include <MKL25Z4.H>
#include <stdio.h>
#include "math.h"

#include "gpio_defs.h"

#include <cmsis_os2.h>
#include "threads.h"

#include "LCD.h"
#include "LCD_driver.h"
#include "font.h"

#include "LEDs.h"
#include "timers.h"
#include "sound.h"
#include "DMA.h"
#include "I2C.h"
#include "mma8451.h"
#include "delay.h"
#include "profile.h"

#include "control.h"
#include "tqueue.h"

TQueue thread_queue[4];
int wait_adc = 0;
int conv_type = 0;
int conv_id = 0;
osEventFlagsId_t evflags_id;
/*----------------------------------------------------------------------------
  MAIN function
 *----------------------------------------------------------------------------*/
int main (void) {

	Init_Debug_Signals();
	Init_RGB_LEDs();
	Control_RGB_LEDs(0,0,1);			
	
	Sound_Disable_Amp();
	
	LCD_Init();
	LCD_Text_Init(1);
	LCD_Erase();
	
	LCD_Erase();
	LCD_Text_PrintStr_RC(0,0, "Test Code");

#if 0
	// LCD_TS_Calibrate();
	LCD_TS_Test();
#endif

	LCD_Text_PrintStr_RC(1,0, "Accel...");
	i2c_init();											// init I2C peripheral
	if (!init_mma()) {							// init accelerometer
		Control_RGB_LEDs(1,0,0);			// accel initialization failed, so turn on red error light
		while (1)
			;
	}
	LCD_Text_PrintStr_RC(1,9, "Done");

	Delay(70);
	LCD_Erase();

	Init_HBLED();
	
	evflags_id = osEventFlagsNew(NULL); 
	
	osKernelInitialize();
	Create_OS_Objects();
	osKernelStart();	
}
