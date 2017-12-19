#ifndef uart0_h
#define uart0_h

#include "MKL46Z4.h"   /* Device header */

void ini_uart0(void);
void SendString(char tablica[]);
uint8_t GetValue(void);

#endif