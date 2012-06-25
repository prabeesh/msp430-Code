#include <msp430.h>
main()
{
P1DIR=0X01;// set P1.0 as output
P1OUT=0x00;// make P1.0 go logic HIGH
TACTL = 0x0220;// Initialize Timer-A; SMCLK; no division, continuous mode

while(1) 
	{
	while(!(TACTL & 1));
	TACTL^=1;// clear TAIFG bit (ie, make it 0)
	P1OUT^=1;// Toggle LED on P1.0
	}
}
