

#include "MKL46Z4.h"  
#include "adc.h"

void adcInitialize() {
	
	
SIM->SCGC6 |= SIM_SCGC6_ADC0_MASK;		//  Clock
	
ADC0->CFG1 |= ADC_CFG1_ADICLK(1)  | ADC_CFG1_ADIV(1) | ADC_CFG1_ADLSMP_MASK;     
																																									
ADC0->CFG2 |= ADC_CFG2_ADHSC_MASK | ADC_CFG2_ADLSTS(0);														
																																										
ADC0->SC2 |= ADC_SC2_REFSEL_MASK;																									
	
ADC0->SC3 |= ADC_SC3_AVGS(3)  | ADC_SC3_AVGE_MASK;																
																																										
adcCalibrate();
	
ADC0->CFG1 |= ADC_CFG1_MODE(1);																								
	
ADC0->SC1[0] |= ADC_SC1_AIEN_MASK;	 																									
}

uint16_t adcCalibrate(){
	
      uint16_t cal;
      
    
      ADC0->SC3 |= ADC_SC3_CAL_MASK;
      while(ADC0->SC3 & ADC_SC3_CAL_MASK); 
      if(ADC0->SC3 & ADC_SC3_CALF_MASK) {
       
        return 1;
   } 
	 
	 cal = ADC0->CLP0 + ADC0->CLP1 + ADC0->CLP2 + ADC0->CLP3 + ADC0->CLP4 + ADC0->CLPS;	
	 cal /= 2;	
	 cal |= (1 << 15); 	
	 ADC0->PG = cal;	
	 cal = ADC0->CLM0 + ADC0->CLM1 + ADC0->CLM2 + ADC0->CLM3 + ADC0->CLM4 + ADC0->CLMS;	
	 cal /= 2;		
   cal |= (1 << 15);		
   ADC0->MG = cal;		
	 
	 return 2;
	 
}
