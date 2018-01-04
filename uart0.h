#ifndef UART0_H
#define UART0_H

#include "MKL46Z4.h"
#include "string.h"

#define Boud_Rate 9600
#define ModuleClock 8000000

void UART0_init(void);
void UART0_transmitter(uint8_t data);
uint8_t UART0_reciver(void);
void send_String(char tablica[]);

#endif
