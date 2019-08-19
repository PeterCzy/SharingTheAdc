#include "FX.h"
#include <MKL25Z4.h>
#include "debug.h"

FX16_16 Multiply_FX(FX16_16 a, FX16_16 b) {
	int64_t p, pa, pb;
	// Long multiply first. 
	pa = a;
	pb = b;
	
	FPTB->PTOR = MASK(DBG_CONTROLLER_POS);
	p = pa * pb;
	FPTB->PTOR = MASK(DBG_CONTROLLER_POS);

	// Check for overflow
	
	// normalize after multiplication
	
	p >>= 16;
	
//	if (p>0)
//		p >>= 16;
//	else {
//		p >>= 16;
//		p = 0xffffffffffffffff-p;
//	}

	return (FX16_16)(p&0xffffffff);
}

FX16_16 Add_FX(FX16_16 a, FX16_16 b) {
	FX16_16 p;
	// Add. This will overflow if a+b > 2^16
	p = a + b;
	return p;
}

FX16_16 Subtract_FX(FX16_16 a, FX16_16 b) {
	FX16_16 p;
	p = a - b;
	return p;
}

void Test_FX(void) {
#if 0
	FX16_16 a, b, c;
	
	a = INT_TO_FX(16);
	b = INT_TO_FX(-16);
	c = Multiply_FX(a,b); // negative 256

	a = INT_TO_FX(16);
	b = INT_TO_FX(16);
	c = Multiply_FX(a,b);	// positive 256
#endif
}
