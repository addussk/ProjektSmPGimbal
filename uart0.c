#include "uart0.h"

#include "MKL46Z4.h" 
#define BUS_CLOCK 24000000
void ini_uart0(void){
	uint16_t divisor;
	// spytac o boud rate, wartosc dzielnika, PRZY BICIE STOPU NA PEWNO UART2?
	
	SIM->SCGC4|=SIM_SCGC4_UART0_MASK;
	SIM->SCGC5|=SIM_SCGC5_PORTA_MASK;  //WYSTARCZY SAM PORTA ? DLA PTA1 i PTA2?
	PORTA->PCR[1] = PORT_PCR_MUX(4);//STAWIENIE ALTERNATYWY NA RX
	PORTA->PCR[2] = PORT_PCR_MUX(4);//STAWIENIE ALTERNATYWY NA TX
	SIM->SOPT2|=SIM_SOPT2_UART0SRC(2);//SET UP zródlo zegara taktujacego modul UART0, w rejestrze SIM->SOPT2
	UART0->C2 &= ~(UART0_C2_TE_MASK | UART0_C2_RE_MASK); //zablokowac nadajnik i odbiornik, w rejestrze UART0->C2, 
	UART0->C4 |=UART0_C4_OSR(11110);//wartosc dzielnika próbkowania nadmiarowego (najlepiej 31)
	
divisor = BUS_CLOCK/(4600*31);
UART0->BDH = UART_BDH_SBR(divisor>>8);
UART0->BDL = UART_BDL_SBR(divisor);

UART0->BDH =!UART0_BDH_SBNS_MASK; //ustawienie 1 bit stop
//dlugosc danej np. na 8 bitów oraz brak sprzetowej obslugi sprawdzania
UART0->C1 =!UART0_C1_M_MASK; 
UART0->C1 =!UART0_C1_PE_MASK; 
//w zaleznosci od potrzeb, wlaczyc przerwania od nadajnika i/lub odbiornika, w rejestrze
UART0->C2|=(UART0_C2_TIE_MASK|	UART0_C2_RIE_MASK);
	
UART2->C2 = UART_C2_TE_MASK | UART_C2_RE_MASK;// Enable transmitter and receiver	
}
void SendString(char tablica[]){
	uint8_t size=0;
	for(size=0;size<=16;size++){
		UART0->D=tablica[size];
		while((UART0->S1 & UART_S1_TDRE_MASK)==0){}
	}
}

uint8_t GetValue(void){
	char temp=0;
	while((UART0->S1 & UART_S1_RDRF_MASK)==0){}
	temp=UART0->D;
	UART0->D = temp;
	SendString("-\n\r");
	return temp;
}