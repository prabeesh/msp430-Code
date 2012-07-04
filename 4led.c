
#include <msp430.h>

main()
{
	P1DIR =0xf;
	int i;	
	while(1)
	{
	P1OUT =BIT0;
	for(i=0;i<30000;i++);
	P1OUT =BIT1;
	for(i=0;i<30000;i++);
	P1OUT =BIT2;
	for(i=0;i<30000;i++);
	P1OUT =BIT3;
	for(i=0;i<30000;i++);
	}
}
