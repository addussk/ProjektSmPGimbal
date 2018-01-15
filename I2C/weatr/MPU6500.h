#include "MKL46Z4.h"


#ifndef MPU6500_h
#define MPU6500_h

#define MPU_adress  (0xD0)		//everything is 8 bit data
#define GYRO_X_H    (0x43)
#define GYRO_X_L    (0x44)
#define GYRO_Y_H	  (0x45)
#define GYRO_Y_L	  (0x46)
#define GYRO_Z_H    (0x47)
#define GYRO_Z_L    (0x48)
#define GYRO_CONF   (0x1B)

void MpuInit(void);
void SetRangeGyro(uint8_t range);
uint16_t ReadAxisGyro(uint8_t axis);
void MotionInit(void);
#endif
