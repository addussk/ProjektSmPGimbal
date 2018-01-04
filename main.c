#include "MKL46Z4.h"                    	/* Device header */
#include "adc.h"
#include "slcd.h"
#include "tpm.h"
#include "uart0.h"
#include "leds.h"


int main (void) {
	
	slcdInitialize();
	tpmInitialize();
	adcInitialize();

	
	
	
	
	while(1){	
	}
}
