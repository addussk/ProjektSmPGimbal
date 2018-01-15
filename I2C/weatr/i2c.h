#ifndef MOJE_I2C_h
#define MOJE_I2C_h
#include "MKL46Z4.h"

void WaitI2C(uint32_t value);
void InitI2C(void);
void EnableI2C(void);
void DisableI2C(void);
void SendRestartI2C(void);
void MasterModeI2C(void);
void SlaveModeI2C(void);
void SendModeI2C(void);
void ReceiveModeI2C(void);
void DisableAckI2C(void);
uint8_t ReadByteI2C(void);
void BusyI2C(void);
uint8_t ReadRegisterI2C(uint8_t reg,uint8_t adress1,uint8_t adress2);
void WriteRegisterI2C(uint8_t reg, uint8_t data,uint8_t adress);
#endif
