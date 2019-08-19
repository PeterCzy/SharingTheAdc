#ifndef HBLED_H
#define HBLED_H

// Switching parameters
#define PWM_HBLED_CHANNEL (4)
#define PWM_PERIOD (1000) 
/* 48 MHz input clock. 
	PWM frequency = 48 MHz/(PWM_PERIOD*2) 
	Timer is in count-up/down mode. */

#define LIM_DUTY_CYCLE (PWM_PERIOD)

#define FLASH_PERIOD (200)
#define FLASH_CURRENT_MA (40)

#define DEF_LED_CURRENT_MA (1)
#define DEF_DUTY_CYCLE (2)

// Hardware configuration
#define ADC_SENSE_CHANNEL (8)

#define R_SENSE (2.2f)
#define R_SENSE_MO ((int) (R_SENSE*1000))

#define V_REF (3.3f)
#define V_REF_MV ((int) (V_REF*1000))

#define ADC_FULL_SCALE (0x10000)
#define MA_SCALING_FACTOR (1000)

#define DAC_POS 30
#define DAC_RESOLUTION 4096

// #define MA_TO_DAC_CODE(i) (i*2.2*DAC_RESOLUTION/V_REF_MV) // Introduces timing delay and interesting bug!
#define MA_TO_DAC_CODE(i) ((i)*(2.2f*DAC_RESOLUTION/V_REF_MV))

#define MIN(a,b) ((a<b)?a:b)
#define MAX(a,b) ((a>b)?a:b)

#endif // HBLED_H
