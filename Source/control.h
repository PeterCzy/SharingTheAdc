#ifndef CONTROL_H
#define CONTROL_H

#include "FX.h"

// Control approach configuration
#define USE_ASYNC_SAMPLING 				0
#define USE_SYNC_NO_FREQ_DIV 			1
#define USE_SYNC_SW_CTL_FREQ_DIV 	0
#define USE_SYNC_HW_CTL_FREQ_DIV 	0

#define SW_CTL_FREQ_DIV_FACTOR (4) // Software division in ISR
#define HW_CTL_FREQ_DIV_CODE (0) // Not used, not supported

#if USE_ASYNC_SAMPLING
#define 	USE_TPM0_INTERRUPT 0
#define 	USE_ADC_HW_TRIGGER 0
#define 	USE_ADC_INTERRUPT 1
#endif

#if USE_SYNC_NO_FREQ_DIV
#define 	USE_TPM0_INTERRUPT 0
#define 	USE_ADC_HW_TRIGGER 1
#define 	USE_ADC_INTERRUPT 1
#endif

#if USE_SYNC_SW_CTL_FREQ_DIV
#define 	USE_TPM0_INTERRUPT 1
#define 	USE_ADC_HW_TRIGGER 0
#define 	USE_ADC_INTERRUPT 1
#endif

#if USE_SYNC_HW_CTL_FREQ_DIV
#define 	USE_TPM0_INTERRUPT 0
#define 	USE_ADC_HW_TRIGGER 1
#define 	USE_ADC_INTERRUPT 1
#endif

// Control Parameters
// default control mode: OpenLoop, BangBang, Incremental, PID, PID_FX
//#define DEF_CONTROL_MODE (Incremental)
#define DEF_CONTROL_MODE (PID_FX)

// Incremental controller: change amount
#define INC_STEP (PWM_PERIOD/100)

// Proportional Gain, scaled by 2^8
#define PGAIN_8 (0x0028)

// PID (floating-point) gains. Guaranteed to be non-optimal. 
#define P_GAIN_FL (0.600f)
#define I_GAIN_FL (0.000f)
#define D_GAIN_FL (0.000f)

// PID_FX (fixed-point) gains. Guaranteed to be non-optimal. 
#define P_GAIN_FX (FL_TO_FX(1.4f))
#define I_GAIN_FX (FL_TO_FX(0.001f))
#define D_GAIN_FX (FL_TO_FX(0.00f))

/*
#define P_GAIN_FX (FL_TO_FX(0.626f))
#define I_GAIN_FX (FL_TO_FX(0.065f))
#define D_GAIN_FX (FL_TO_FX(1.831f))
*/

// Data type definitions
typedef struct {
	float dState; // Last position input
	float iState; // Integrator state
	float iMax, iMin; // Maximum and minimum allowable integrator state
	float pGain, // proportional gain
				iGain, // integral gain
				dGain; // derivative gain
} SPid;

typedef struct {
	FX16_16 dState; // Last position input
	FX16_16 iState; // Integrator state
	FX16_16 iMax, iMin; // Maximum and minimum allowable integrator state
	float pGain, // proportional gain
				iGain, // integral gain
				dGain; // derivative gain
} SPidFX;

typedef enum {OpenLoop, BangBang, Incremental, Proportional, PID, PID_FX} CTL_MODE_E;

// Functions
void Init_HBLED(void);
void Update_Set_Current(void);

// Shared global variables
extern volatile int g_peak_set_current;

#endif // #ifndef CONTROL_H
