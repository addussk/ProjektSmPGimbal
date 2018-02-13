#ifndef i2c_h
#define i2c_h

#include "MKL46Z4.h"






typedef enum{
  I2C_ACK = 0,
  I2C_NACK = 1,
} I2C_Acknowledge_Bit;

void		I2C_Init(I2C_Type* i2c);
void    I2C_Disable(I2C_Type* i2c);
void    I2C_DisableInt(I2C_Type* i2c);
void    I2C_Enable(I2C_Type* i2c);
void    I2C_EnableInt(I2C_Type* i2c);
uint8_t I2C_ReadByte(I2C_Type* i2c, uint8_t ack);
void    I2C_Restart(I2C_Type* i2c);
void    I2C_Start(I2C_Type* i2c);
void    I2C_Stop(I2C_Type* i2c);
void    I2C_WriteByte(I2C_Type* i2c, uint8_t data);
void sendACK(I2C_Type* i2c); 
void sendNACK(I2C_Type* i2c);




#endif
