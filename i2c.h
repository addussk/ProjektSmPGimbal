#ifndef i2c_h
#define i2c_h

#include "MKL46Z4.h"   /* Device header */

void WaitI2C(uint32_t value);
void InitializationI2C(void);
void EnableI2C(void);
void DisableI2C(void);
void SendRestartI2C(void);
void MasterModeI2C(void);
void SlaveModeI2C(void);
void TransmitModeI2C(void);
void ReceiveModeI2C(void);
void DisableAckI2C(void);
uint8_t ReadByteI2C(void);
void ClearInteruptI2C(void);
uint8_t ReadRegisterI2C(uint8_t reg,uint8_t adress1,uint8_t adress2);
void WriteRegisterI2C(uint8_t reg, uint8_t data,uint8_t adress);

#endif