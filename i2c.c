#include "i2c.h"

uint8_t adres_ukladu=0x1D;

void WaitI2C(uint32_t value);

void InitializationI2C(void){
  SIM->SCGC5  |= SIM_SCGC5_PORTB_MASK;  // Wlaczenie zegara dla portu B
  SIM->SCGC4  |= SIM_SCGC4_I2C0_MASK;   // Wlaczenie zegara dla 
  PORTB->PCR[1] = PORT_PCR_MUX(2) |\
                  PORT_PCR_DSE_MASK;    //PTB 1 (I2C0_SDA) -- ptb1 czas wstrzymania SDA
  PORTB->PCR[0] = PORT_PCR_MUX(2) |\
                  PORT_PCR_DSE_MASK;    //PTB 2 (I2C0_SCL)  --ptb0  czas wstrzymania uruchomienia SCL / czas wstrzymania zatrzymania SCL

  I2C0->F   = 0x14;                     // baudrate: 330kHz - 0x14 baudrate 100k 0x1F 
  I2C0->C1 |= I2C_C1_IICEN_MASK;        // Wlaczenie I2C0 PRZERWANIA 
}
void EnableI2C(void){
	I2C0->C1 |= I2C_C1_IICEN_MASK; //wlaczenie modulu PRZERWANIA
}
void DisableI2C(void){
	I2C0->C1 &= ~I2C_C1_IICEN_MASK; //wylaczenie modulu
}
void SendRestartI2C(void){
	I2C0->C1 |= I2C_C1_RSTA_MASK;		//wyslanie sygnalu restart
} 

void MasterModeI2C(void){
	I2C0->C1 |= I2C_C1_MST_MASK;		//tryb master zmiana z 0 na 1 
}
void SlaveModeI2C(void){
	I2C0->C1 &= ~I2C_C1_MST_MASK;   //tryb slave zmiana z 1 na 0 
}
void TransmitModeI2C(void){
	I2C0->C1 |= I2C_C1_TX_MASK; 	//tryb  nadawania 
}
void ReceiveModeI2C(void){
	I2C0->C1 &= ~I2C_C1_TX_MASK;   //tryb odbioru
}
void DisableAckI2C(void){
	I2C0->C1 |= I2C_C1_TXAK_MASK;		//w trybie odbioru wylaczamy ACK czyli na koniec wysylamy NACK
}
uint8_t ReadByteI2C(void){
	return I2C0->D; 			//zwracamy ramke danych
}
void ClearInteruptI2C(void){
	while((I2C0->S & I2C_S_IICIF_MASK)==0){}	//czekamy na przerwanie
	I2C0->S |= I2C_S_IICIF_MASK;               // Wyzerowanie flagi przerwania
}
void SendByteI2C(uint8_t dane){		//wysylamy bajta
	I2C0->D=dane;
}
void WaitI2C(uint32_t value){
	uint32_t i;
	for(i=0;i<value;i++){__ASM("nop");}
}
//////////////////////////////////////////////
uint8_t ReadRegisterI2C(uint8_t reg,uint8_t adress1,uint8_t adress2){
	uint8_t dane=0;
	EnableI2C();			//aktyw przerwania
	MasterModeI2C();  //ustawienie trybu master
	TransmitModeI2C();	//aktywowanie trybu nadawania
	SendByteI2C(adress1);		//default 0x3a , wyslanie pod adres danych
	ClearInteruptI2C();			
	SendByteI2C(reg);			//rejestr ktory chcemy odczytac
	ClearInteruptI2C();
	SendRestartI2C();
	SendByteI2C(adress2);		//default 0x3b
	ClearInteruptI2C();
	ReceiveModeI2C();
	DisableAckI2C();
	dane=ReadByteI2C();
	ClearInteruptI2C();
	SlaveModeI2C();
	ReceiveModeI2C();
	dane=ReadByteI2C();  //wlasciwy odczyt
	DisableI2C();
	WaitI2C(50);
	return dane;
}
void WriteRegisterI2C(uint8_t reg, uint8_t data,uint8_t adress){
	EnableI2C();
	TransmitModeI2C();
	MasterModeI2C();
	SendByteI2C(adress);	//adres slave default 0x3a
	ClearInteruptI2C();
	SendByteI2C(reg);		
	ClearInteruptI2C();
	SendByteI2C(data);
	ClearInteruptI2C();
	SlaveModeI2C();
	ReceiveModeI2C();
	DisableI2C();	
	WaitI2C(50);
}










