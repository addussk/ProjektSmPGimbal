#include "MKL46Z4.h"
#include "i2c.h"

void I2C_Init(I2C_Type* i2c)
{
	SIM->SCGC4 |= SIM_SCGC4_I2C1_MASK;				
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;				
	PORTE->PCR[0]  = PORT_PCR_MUX(6);         
  PORTE->PCR[1]  = PORT_PCR_MUX(6);         
  
	I2C1->F   =I2C_F_ICR(0x1F);  
}

void I2C_Disable(I2C_Type* i2c){
  i2c->C1 &= ~I2C_C1_IICEN_MASK;
}

void I2C_Enable(I2C_Type* i2c){
  i2c->C1 |= I2C_C1_IICEN_MASK;
}

uint8_t I2C_ReadByte(I2C_Type* i2c, uint8_t ack){

  i2c->C1 &= ~I2C_C1_TX_MASK;
  i2c->S |= I2C_S_IICIF_MASK;

  (void)i2c->D;

  while((i2c->S & I2C_S_IICIF_MASK) == 0);

  i2c->C1 |= I2C_C1_TX_MASK;
  
  return i2c->D;
}

void I2C_Restart(I2C_Type* i2c){
  i2c->C1 |= I2C_C1_RSTA_MASK;
}

void I2C_Start(I2C_Type* i2c){
  i2c->C1 |= I2C_C1_MST_MASK;
}

void I2C_Stop(I2C_Type * i2c){

  i2c->FLT |= I2C_FLT_STOPF_MASK;
  
  i2c->C1 &= ~I2C_C1_MST_MASK;

  while((i2c->FLT & I2C_FLT_STOPF_MASK) == 0){
    i2c->C1 &= ~I2C_C1_MST_MASK;
  }
}

void I2C_ack(I2C_Type *I2C)
{
	I2C->C1 &= ~I2C_C1_TXAK_MASK;				
}

void I2C_nack(I2C_Type *I2C)
{
	I2C->C1 |= I2C_C1_TXAK_MASK;					
}

void I2C_WriteByte(I2C_Type* i2c, uint8_t data){

	i2c->C1 |= I2C_C1_TX_MASK;
  
  i2c->S |= I2C_S_IICIF_MASK;
  
  i2c->D = data;

  while((i2c->S & I2C_S_IICIF_MASK) == 0);   
}

void sendACK(I2C_Type* i2c) 
{
	i2c->C1 &= ~I2C_C1_TXAK_MASK;
}
	
void sendNACK(I2C_Type* i2c)
{
	i2c->C1 |= I2C_C1_TXAK_MASK;
}
