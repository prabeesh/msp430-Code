
#include <msp430.h>

void mask(unsigned short num) {		
 	P1DIR =0xff;
	switch (num) {
 case 0 : P1OUT=0x3F; break;
 case 1 : P1OUT=0x06; break;
 case 2 : P1OUT=0x5B; break;
 case 3 : P1OUT=0x4F; break;
 case 4 : P1OUT=0x66; break;
 case 5 : P1OUT=0x6D; break;
 case 6 : P1OUT=0x7D; break;
 case 7 : P1OUT=0x07; break;
 case 8 : P1OUT=0x7F; break;
 case 9 : P1OUT=0x67; break;
 } 
}


main()
{
int i,a=0;
		
while(1){
	
	mask(a);
	
	a++;
	
	for(i=0;i<30000;i++);}
}
