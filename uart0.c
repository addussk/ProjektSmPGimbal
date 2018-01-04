#include "uart0.h"

void UART0_init(void){
	int div;
	// zegar UART0 i PORTA
	SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;
	// UART0 [Rx] PORTA 1 i [Tx] PORTA 2
	PORTA->PCR[1] |= PORT_PCR_MUX(2);
	PORTA->PCR[2] |= PORT_PCR_MUX(2);
	// zrodlo zegara taktujacego
	SIM->SOPT2 |= SIM_SOPT2_UART0SRC(2);
	// transmitter and reciver disabled 
	UART0->C2 &= ~(UART0_C2_TE_MASK | UART0_C2_RE_MASK);
	//dzielnik porbkowania nadmiarowego
	UART0->C4 |= UART0_C4_OSR(31);
	//13-bitowa wartosc dzielnika
	div =  ModuleClock/(32* Boud_Rate);
	UART0->BDH |= UART_BDH_SBR(div>>8);
	UART0->BDL |= UART_BDL_SBR(div);
	// 1-bit stopu
	UART0->BDH &= ~(UART_BDH_SBNS_MASK);
	//ramka 8-bit, brak sprzetowego sprawdzania parzystosci
	UART0->C1 &= ~(UART_C1_M_MASK | UART_C1_PE_MASK);
	// przerwania
	UART0->C2 |= UART_C2_TIE_MASK | UART_C2_RIE_MASK;
	//wlaczyc nadajnik i odbiornik
	UART0->C2 |= (UART0_C2_TE_MASK | UART0_C2_RE_MASK);
}

void UART0_transmitter(uint8_t data){
	UART0->D=data;
	while(!(UART0->S1 & UART0_S1_TDRE_MASK)) {}
}

uint8_t UART0_reciver(void){
	while(!(UART0->S1 & UART0_S1_RDRF_MASK));
	return UART0->D;
}

void send_String(char tablica[]){
	
	for(int i=0;i<=strlen(tablica);i++){
		UART0->D=tablica[i];
		while(!(UART0->S1 & UART0_S1_TDRE_MASK));
	}
}