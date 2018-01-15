#include "MKL46Z4.h"
#include "lcd.h"
#include "i2c.h"
#include "mpu6500.h"


int main(){
	InitI2C();
	MpuInit();
	slcdInitialize();
	//MotionInit();
	
	while(1){
		MotionInit();
		slcdDisplay(ReadAxisGyro(1),10);
	}
}
