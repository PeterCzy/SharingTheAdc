#ifndef THREADS_H
#define THREADS_H

#include <cmsis_os2.h>
#include "LCD_driver.h"
#include <MKL25Z4.H>

#define THREAD_READ_TS_PERIOD_MS (50)  // 1 tick/ms
#define THREAD_READ_ACCELEROMETER_PERIOD_MS (100)  // 1 tick/ms
#define THREAD_SOUND_PERIOD_MS (100)  // 1 tick/ms
#define THREAD_UPDATE_SCREEN_PERIOD_MS (50)
#define THREAD_BUS_PERIOD_MS (10)

#define USE_LCD_MUTEX (1)

// Custom stack sizes for larger threads
#define READ_ACCEL_STK_SZ 768 // 512

void Init_Debug_Signals(void);

// Events for sound generation and control
#define EV_PLAYSOUND (1) 
#define EV_SOUND_ON (2)
#define EV_SOUND_OFF (4)

#define EV_REFILL_SOUND_BUFFER  (1)

void Create_OS_Objects(void);
 
extern osThreadId_t t_Read_TS, t_Read_Accelerometer, t_Sound_Manager, t_US, t_Refill_Sound_Buffer;
extern osMutexId_t LCD_mutex;

 
// Game Constants
#define PADDLE_WIDTH (40)
#define PADDLE_HEIGHT (15)
#define PADDLE_Y_POS (LCD_HEIGHT-4-PADDLE_HEIGHT)
 
#endif // THREADS_H
