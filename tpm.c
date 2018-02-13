#include "MKL46Z4.h"  
#include "tpm.h"
#include "slcd.h"
#include "math.h"
#include "i2c.h"
#include "mpu.h"

static int value=0;

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
	
	while((I2C1->S & I2C_S_RXAK_MASK));
	
		value = readY()/16;
		slcdDisplay(value,1);
		delay_mc(100);
	
	TPM0->STATUS &= ~TPM_STATUS_CH0F_MASK;
	
		TPM0->CONTROLS[3].CnV = value;
		TPM0->CONTROLS[4].CnV = 4060-value;		
}