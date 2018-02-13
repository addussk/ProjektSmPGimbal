#include "MKL46Z4.h"                    	/* Device header */
#include "slcd.h"
#include "leds.h"
#include "extra.h"
#include "tpm.h"
#include "i2c.h"
#include "mpu.h"

int main (void) {
	
	
	slcdInitialize();
	tpmInitialize();
	//UART0_init();
	
	I2C_Init(I2C1);
	setRegister(0x6b,0x81);

	while(1){	
	}
}
