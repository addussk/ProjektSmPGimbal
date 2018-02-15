#include "MKL46Z4.h"  
#include "tpm.h"
#include "slcd.h"
#include "math.h"
#include "i2c.h"
#include "mpu.h"
#include "uart0.h"
#include "filter.h"

static int value=0;
static int value_uart=0;

void tpmInitialize(){

	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;	// enable clock for Port D & E
	SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK;	// enable clock for TPM
	
	PORTA->PCR[6] |= PORT_PCR_ISF_MASK | PORT_PCR_MUX(3) | PORT_PCR_DSE_MASK;
	PORTA->PCR[7] |= PORT_PCR_ISF_MASK | PORT_PCR_MUX(3) | PORT_PCR_DSE_MASK;

	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1) | SIM_SOPT2_PLLFLLSEL_MASK;     
																																		
	TPM0->SC |= !TPM_SC_CPWMS_MASK | TPM_SC_PS(7); 
	
	TPM0->MOD = 4095;	
			
	TPM0->CONTROLS[3].CnSC |= TPM_CnSC_MSB_MASK |TPM_CnSC_ELSB_MASK;
	TPM0->CONTROLS[4].CnSC |= TPM_CnSC_MSB_MASK |TPM_CnSC_ELSB_MASK; 	

	TPM0->CONTROLS[3].CnV = 0;
  TPM0->CONTROLS[4].CnV = 0; 	
	
	TPM0->SC |= TPM_SC_CMOD(1);		
	
	/* Interrupt Section */
	NVIC_SetPriority(TPM0_IRQn, 1);
	NVIC_ClearPendingIRQ( TPM0_IRQn); 				
	NVIC_EnableIRQ( TPM0_IRQn);	

	TPM0->CONTROLS[3].CnSC |= TPM_CnSC_CHIE_MASK | TPM_CnSC_CHF_MASK;		
	
  TPM0->SC|=TPM_SC_TOIE_MASK | TPM_SC_TOF_MASK ; 
}
void TPM0_IRQHandler(){
	
	// Sprawdza flage od wyslana ACK w i2c
	while((I2C1->S & I2C_S_RXAK_MASK));
	
	/*
	// filtr ale cos nie bangla 
	if(i<=10){
		
		queue[i] = readY()/16;
		
		if(i==10){
			
		value = avarage(queue, 5);
		slcdDisplay(value,1);
		delay_mc(100);
			
			char value_vizualization[]="xxxx\n";
	
		value_vizualization[3] = value/1000 + '0';
		value_vizualization[2] = (value -(value_vizualization[3]*1000))/100 + '0';
		value_vizualization[1] = (value -(value_vizualization[3]*1000)-(value_vizualization[2]*100))/10 + '0';
		value_vizualization[0] = (value -(value_vizualization[3]*1000)-(value_vizualization[2]*100)-(value_vizualization[1]*10)) + '0';		
	
	send_String(value_vizualization);
		
	
	TPM0->STATUS &= ~TPM_STATUS_CH0F_MASK;
	
		TPM0->CONTROLS[3].CnV = value;
		TPM0->CONTROLS[4].CnV = 4060-value;	
			i=0;
		}
	}
	*/
		// odczytuje wartosc z interesujacej nas osii wyrzuca na lcd
		value = readY()/16;
		slcdDisplay(value,1);
		delay_mc(100);
	
	// dane wysylane na uart
	char value_vizualization[]="xxxx\n";
	uint8_t intermediate_value[4];
	
		intermediate_value[3] = value/1000;
		intermediate_value[2] = (value -(intermediate_value[3]*1000))/100;
		intermediate_value[1] = (value -(intermediate_value[3]*1000)-(intermediate_value[2]*100))/10;
		intermediate_value[0] = (value -(intermediate_value[3]*1000)-(intermediate_value[2]*100)-(intermediate_value[1]*10));

		value_vizualization[3]=intermediate_value[0] + '0';
		value_vizualization[2]=intermediate_value[1] + '0';
		value_vizualization[1]=intermediate_value[2] + '0';
		value_vizualization[0]=intermediate_value[3] + '0';
	
	send_String(value_vizualization);
		
	// czysci flage przerwania od tpm kanal 0 CH 0 F
	TPM0->STATUS &= ~TPM_STATUS_CH0F_MASK;
		// kontroluje pwm dla silniczkow na wyjsciach a6 i a7 kanal 3 i 4
		TPM0->CONTROLS[3].CnV = value;
		TPM0->CONTROLS[4].CnV = 4060-value;	

}