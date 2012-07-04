
#include <msp430.h>

void delay()
{
unsigned int i = 0;
while(++i < 30000);
}

main()
{
	P1DIR =0xf;
	
	while(1)
	{
	P1OUT =BIT0;
	delay();
	P1OUT =BIT1;
	delay();
	P1OUT =BIT2;
	delay();
	P1OUT =BIT3;
	delay();
	}
}
