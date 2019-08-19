#include <stdint.h>
#include <stdio.h>
#include <cmsis_os2.h>
#include <MKL25Z4.h>

#include "LCD.h"
#include "touchscreen.h"
#include "LCD_driver.h"
#include "font.h"
#include "threads.h"
#include "MMA8451.h"
#include "sound.h"
#include "DMA.h"
#include "gpio_defs.h"
#include "debug.h"
#include "control.h"

#include "ST7789.h"
#include "T6963.h"

extern volatile int g_peak_set_current; // Peak flash current

void Thread_Read_TS(void * arg); // 
void Thread_Read_Accelerometer(void * arg); // 
void Thread_Update_Screen(void * arg); // 
void Thread_Sound_Manager(void * arg); // 
void Thread_Refill_Sound_Buffer(void * arg); //
void Thread_Buck_Update_Setpoint(void * arg);

osThreadId_t t_Read_TS, t_Read_Accelerometer, t_Sound_Manager, t_US, t_Refill_Sound_Buffer, t_BUS;
osThreadId_t conv;

// Thread priority options: osPriority[RealTime|High|AboveNormal|Normal|BelowNormal|Low|Idle]

const osThreadAttr_t Read_TS_attr = {
  .priority = osPriorityNormal            
};
const osThreadAttr_t Read_Accelerometer_attr = {
  .priority = osPriorityBelowNormal,      
	.stack_size = READ_ACCEL_STK_SZ
};
const osThreadAttr_t Update_Screen_attr = {
  .priority = osPriorityNormal            
};

const osThreadAttr_t BUS_attr = {
  .priority = osPriorityAboveNormal            
};

osMutexId_t LCD_mutex;

const osMutexAttr_t LCD_mutex_attr = {
  "LCD_mutex",     // human readable mutex name
  osMutexPrioInherit    // attr_bits
};


void Create_OS_Objects(void) {
	LCD_mutex = osMutexNew(&LCD_mutex_attr);

	t_Read_TS = osThreadNew(Thread_Read_TS, NULL, &Read_TS_attr);  
	t_Read_Accelerometer = osThreadNew(Thread_Read_Accelerometer, NULL, &Read_Accelerometer_attr);
	t_US = osThreadNew(Thread_Update_Screen, NULL, &Update_Screen_attr);
	t_BUS = osThreadNew(Thread_Buck_Update_Setpoint, NULL, &BUS_attr);
}

void Thread_Read_TS(void * arg) {
	PT_T p, pp;
	COLOR_T c;
	char buffer[16];
	 
	c.R = 255;
	c.G = 200;
	c.B = 200;
	
	LCD_Text_PrintStr_RC(LCD_MAX_ROWS-2, 0, "Dim <--------> Bright");
	
	while (1) {
		DEBUG_START(DBG_TREADTS_POS);
		if (LCD_TS_Read(&p)) { 
			if (p.Y > ROW_TO_Y(LCD_MAX_ROWS-3)) { 
				// Update g_peak_set_current based on X position of touch
				sprintf(buffer, "g_peak: %6.2d", p.X);
				osMutexAcquire(LCD_mutex, osWaitForever);
				LCD_Text_PrintStr_RC(2, 0, buffer);
				g_peak_set_current = p.X;
				osMutexRelease(LCD_mutex);
				
			} else {		
				// Else draw on screen
				if ((pp.X == 0) && (pp.Y == 0)) {
					pp = p;
				}
				osMutexAcquire(LCD_mutex, osWaitForever);
				LCD_Draw_Line(&p, &pp, &c);
				osMutexRelease(LCD_mutex);
				pp = p;
			} 
		} else {
			pp.X = 0;
			pp.Y = 0;
		}
		DEBUG_STOP(DBG_TREADTS_POS);
		osDelay(THREAD_READ_TS_PERIOD_MS);
	}
}

 void Thread_Read_Accelerometer(void * arg) {
	char buffer[16];
	
	while (1) {
		DEBUG_START(DBG_TREADACC_POS);
	
		read_full_xyz();
		convert_xyz_to_roll_pitch();

		sprintf(buffer, "Roll: %6.2f", roll);
		osMutexAcquire(LCD_mutex, osWaitForever);
		LCD_Text_PrintStr_RC(0, 0, buffer);
		osMutexRelease(LCD_mutex);

		sprintf(buffer, "Pitch: %6.2f", pitch);
		osMutexAcquire(LCD_mutex, osWaitForever);
		LCD_Text_PrintStr_RC(1, 0, buffer);
		osMutexRelease(LCD_mutex);
		DEBUG_STOP(DBG_TREADACC_POS);
		osDelay(THREAD_READ_ACCELEROMETER_PERIOD_MS);
	}
}

 void Thread_Update_Screen(void * arg) {
	int16_t paddle_pos=LCD_WIDTH/2;
	PT_T p1, p2;
	COLOR_T paddle_color; 
	 
	paddle_color.R = 100;
	paddle_color.G = 10;
	paddle_color.B = 100;

	while (1) {
		DEBUG_START(DBG_TUPDATESCR_POS);
		
		if ((roll < -2.0) || (roll > 2.0)) {
			p1.X = paddle_pos;
			p1.Y = PADDLE_Y_POS;
			p2.X = p1.X + PADDLE_WIDTH;
			p2.Y = p1.Y + PADDLE_HEIGHT;
			osMutexAcquire(LCD_mutex, osWaitForever);
			LCD_Fill_Rectangle(&p1, &p2, &black); 		
			osMutexRelease(LCD_mutex);
			
			paddle_pos += roll;
			paddle_pos = MAX(0, paddle_pos);
			paddle_pos = MIN(paddle_pos, LCD_WIDTH-1-PADDLE_WIDTH);
			
			p1.X = paddle_pos;
			p1.Y = PADDLE_Y_POS;
			p2.X = p1.X + PADDLE_WIDTH;
			p2.Y = p1.Y + PADDLE_HEIGHT;
			paddle_color.R = 150+5*roll;
			paddle_color.G = 150-5*roll;
			osMutexAcquire(LCD_mutex, osWaitForever);
			LCD_Fill_Rectangle(&p1, &p2, &white); 		
			p1.X++;
			p2.X--;
			p1.Y++;
			p2.Y--;
			LCD_Fill_Rectangle(&p1, &p2, &paddle_color); 		
			osMutexRelease(LCD_mutex);
		}
		
		DEBUG_STOP(DBG_TUPDATESCR_POS);
		osDelay(THREAD_UPDATE_SCREEN_PERIOD_MS);
	}
}

 void Thread_Buck_Update_Setpoint(void * arg) {
	while (1) {
		osDelay(THREAD_BUS_PERIOD_MS);
		Update_Set_Current();
	}
 }
 
 