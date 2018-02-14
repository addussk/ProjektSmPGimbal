#ifndef mpu_h
#define mpu_h


#include "MKL46Z4.h" 
#include "i2c.h"

#define MPU6500_adress 0x68
#define PWR_MGMT_1 0x6B
#define PWR_MGMT_2 0x6C
#define INT_ENABLE 0x38
#define FIFO_EN 0x23
#define I2C_MST_CTRL 0x24
#define USER_CTRL 0x6A
#define CONFIG 0x1A
#define SMPLRT_DIV 0x19
#define ACCEL_CONFIG 0x1C
#define GYRO_CONFIG 0x1B
#define FIFO_COUNTH 0x92
#define FIFO_R_W 0x74
#define XG_OFFS_USRH 0x13
#define XG_OFFS_USRL 0x14
#define YG_OFFS_USRH 0x15
#define YG_OFFS_USRL 0x16
#define ZG_OFFS_USRH 0x17
#define ZG_OFFS_USRL 0x18

uint16_t read(uint8_t);
void setRegister(uint8_t, uint8_t);
void Init(void);
uint16_t readZ();
uint16_t readX();
void reset(void);
void  readBytes();
void calibrate(float* dest1, float* dest2);

#endif
