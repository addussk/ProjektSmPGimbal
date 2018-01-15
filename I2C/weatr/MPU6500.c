#include "MPU6500.h"
#include "i2c.h"

void MpuInit(void){
	WriteRegisterI2C(0x1B,0x00,MPU_adress);			
	WriteRegisterI2C(0x19,15,MPU_adress);				//sample sets 1khz
	WriteRegisterI2C(0x6B,0x00,MPU_adress);			//wake up accel :)
	WriteRegisterI2C(0x6C,0xC0,MPU_adress);
}

void SetRangeGyro(uint8_t range){		//you can choose 0-2g, 1-4g, 2-8g, 3-16g
	uint8_t data=0;
	if (range==0){data=0;}
	else if(range==1){data=0x08;}
	else if(range==2){data=0x10;}
	else if(range==3){data=0x18;}
	WriteRegisterI2C(GYRO_CONF,data,MPU_adress);
}

uint16_t ReadAxisGyro(uint8_t axis){		//axis=0 -> x,axis=1 -> y, axis=2 -> z
	uint8_t Register=0;
	uint16_t Data=0;
	if(axis==0){Register=GYRO_X_H;}
	else if(axis==1){Register=GYRO_Y_H;}
	else if(axis==2){Register=GYRO_Z_H;}
	else {return 0;}
	Data=ReadRegisterI2C(Register,MPU_adress,(MPU_adress+1));
	Data=Data<<8;
	Data+=ReadRegisterI2C((Register+1),MPU_adress,(MPU_adress+1));
	return Data;
}

void MotionInit(void){
	WriteRegisterI2C(0x68,0x07,MPU_adress);	
	WriteRegisterI2C(0x37,0x00,MPU_adress);			//latch_int default 0x20
	WriteRegisterI2C(0x1B,0x01,MPU_adress);			//5hz hpf
	WriteRegisterI2C(0x1F,1,MPU_adress);
	WriteRegisterI2C(0x20,20,MPU_adress);			//20 ms trwanie 
	WriteRegisterI2C(0x69,0x15,MPU_adress);			//  0b00010101
	WriteRegisterI2C(0x38,0x40,MPU_adress);		
}

