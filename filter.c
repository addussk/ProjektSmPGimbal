#include "filter.h"

uint16_t avarage(uint16_t *tab, int size){
	
	uint32_t suma=0;
	for (int i=0; i<size;i++){
		suma += queue[i];
		queue[i]=0;
	}
	
	return (suma/size); 
}