
#include <msp430.h>

#define LCM_DIR P1DIR
#define LCM_OUT P1OUT

#define LCM_PIN_RS BIT2 // P1.2
#define LCM_PIN_EN BIT1 // P1.1
#define LCM_PIN_D7 BIT7 // P1.7
#define LCM_PIN_D6 BIT6 // P1.6
#define LCM_PIN_D5 BIT5 // P1.5
#define LCM_PIN_D4 BIT4 // P1.4


#define LCM_PIN_MASK ((LCM_PIN_RS | LCM_PIN_EN | LCM_PIN_D7 | LCM_PIN_D6 | LCM_PIN_D5 | LCM_PIN_D4))

#define FALSE 0
#define TRUE 1

//msp430 adc

void adc_init()
	
	{
	ADC10CTL0 = ADC10ON | ADC10SHT_2 | SREF_0;
	ADC10CTL1 = INCH_0 | SHS_0 | ADC10DIV_0 | ADC10SSEL_0 | CONSEQ_0 ;
	ADC10AE0 = BIT0;
	ADC10CTL0 |= ENC ;
	}

void start_conversion()
	
	{
	ADC10CTL0 |= ADC10SC;
	}

unsigned int converting()
	
	{
	return ADC10CTL1 & ADC10BUSY;
	}

// MSP430 LCD Code

void PulseLcm()
{
    
    // pull EN bit low
    
    LCM_OUT &= ~LCM_PIN_EN;
    __delay_cycles(200);

    
    // pull EN bit high
    
    LCM_OUT |= LCM_PIN_EN;
    __delay_cycles(200);

    
    // pull EN bit low again
    
    LCM_OUT &= (~LCM_PIN_EN);
    __delay_cycles(200);
}


void SendByte(char ByteToSend, int IsData)

{
    
    // clear out all pins
   
    LCM_OUT &= (~LCM_PIN_MASK);

    LCM_OUT |= (ByteToSend & 0xF0);

    if (IsData == TRUE)
    {
        LCM_OUT |= LCM_PIN_RS;
    }
    else
    {
        LCM_OUT &= ~LCM_PIN_RS;
    }
   
    PulseLcm();

   
    LCM_OUT &= (~LCM_PIN_MASK);
    LCM_OUT |= ((ByteToSend & 0x0F) << 4);

    if (IsData == TRUE)
    {
        LCM_OUT |= LCM_PIN_RS;
    }
    else
    {
        LCM_OUT &= ~LCM_PIN_RS;
    }

    PulseLcm();
}

void LcmSetCursorPosition(char Row, char Col)
{
    char address;

   
    // construct address from (Row, Col) pair
    
    if (Row == 0)
    {
        address = 0;
    }
    else
    {
        address = 0x40;
    }

    address |= Col;
    SendByte(0x80 | address, FALSE);
}

void ClearLcmScreen()
{
   
    // Clear display, return home
    SendByte(0x01, FALSE);
    SendByte(0x02, FALSE);
}


void InitializeLcm(void)
{
   
    LCM_DIR |= LCM_PIN_MASK;
    LCM_OUT &= ~(LCM_PIN_MASK);
   
    __delay_cycles(100000);

      LCM_OUT &= ~LCM_PIN_RS;
    LCM_OUT &= ~LCM_PIN_EN;

    LCM_OUT = 0x20;
    PulseLcm();

    SendByte(0x28, FALSE);

    SendByte(0x0E, FALSE);

    SendByte(0x06, FALSE);
}


void PrintStr(char *Text)
{
    char *c;
    c = Text;
    
    while ((c != 0) && (*c != 0))
    {
        SendByte(*c, TRUE);
        c++;
    }
}



void main(void)
{
	adc_init();
	
	int i,a;

	char b[5];
 
	WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer

    	InitializeLcm();
  	
	while(1)
	{

	start_conversion();

	while(converting());

	a = ADC10MEM;
	
	itoa(a,b,10);//integer to ASCII
	
	ClearLcmScreen();
	
    	PrintStr(b);
	
	for(i=0;i<5000;i++);
    	
	}	
}



