
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
//
// MSP430 LCD Code
//



//
// Define symbolic LCM - MCU pin mappings
// We've set DATA PIN TO 4,5,6,7 for easy translation
//


//
// Routine Desc:
//
// This is the function that must be called
// whenever the LCM needs to be told to
// scan it's data bus.
//
// Parameters:
//
// void.
//
// Return
//
// void.
//
void PulseLcm()
{
    //
    // pull EN bit low
    //
    LCM_OUT &= ~LCM_PIN_EN;
    __delay_cycles(200);

    //
    // pull EN bit high
    //
    LCM_OUT |= LCM_PIN_EN;
    __delay_cycles(200);

    //
    // pull EN bit low again
    //
    LCM_OUT &= (~LCM_PIN_EN);
    __delay_cycles(200);
}

//
// Routine Desc:
//
// Send a byte on the data bus in the 4 bit mode
// This requires sending the data in two chunks.
// The high nibble first and then the low nible
//
// Parameters:
//
// ByteToSend - the single byte to send
//
// IsData - set to TRUE if the byte is character data
// FALSE if its a command
//
// Return
//
// void.
//
void SendByte(char ByteToSend, int IsData)
{
    //
    // clear out all pins
    //
    LCM_OUT &= (~LCM_PIN_MASK);

    //
    // set High Nibble (HN) -
    // usefulness of the identity mapping
    // apparent here. We can set the
    // DB7 - DB4 just by setting P1.7 - P1.4
    // using a simple assignment
    //
    LCM_OUT |= (ByteToSend & 0xF0);

    if (IsData == TRUE)
    {
        LCM_OUT |= LCM_PIN_RS;
    }
    else
    {
        LCM_OUT &= ~LCM_PIN_RS;
    }
    //
    // we've set up the input voltages to the LCM.
    // Now tell it to read them.
    //
    PulseLcm();

    //
    // set Low Nibble (LN) -
    // usefulness of the identity mapping
    // apparent here. We can set the
    // DB7 - DB4 just by setting P1.7 - P1.4
    // using a simple assignment
    //
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

    //
    // we've set up the input voltages to the LCM.
    // Now tell it to read them.
    //
    PulseLcm();
}

//
// Routine Desc:
//
// Set the position of the cursor on the screen
//
// Parameters:
//
// Row - zero based row number
//
// Col - zero based col number
//
// Return
//
// void.
//
void LcmSetCursorPosition(char Row, char Col)
{
    char address;

    //
    // construct address from (Row, Col) pair
    //
    
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

//
// Routine Desc:
//
// Clear the screen data and return the
// cursor to home position
//
// Parameters:
//
// void.
//
// Return
//
// void.
//
void ClearLcmScreen()
{
    //
    // Clear display, return home
    //
    SendByte(0x01, FALSE);
    SendByte(0x02, FALSE);
}

//
// Routine Desc:
//
// Initialize the LCM after power-up.
//
// Note: This routine must not be called twice on the
// LCM. This is not so uncommon when the power
// for the MCU and LCM are separate.
//
// Parameters:
//
// void.
//
// Return
//
// void.
//
void InitializeLcm(void)
{
    //
    // set the MSP pin configurations
    // and bring them to low
    //
    LCM_DIR |= LCM_PIN_MASK;
    LCM_OUT &= ~(LCM_PIN_MASK);
    //
    // wait for the LCM to warm up and reach
    // active regions. Remember MSPs can power
    // up much faster than the LCM.
    //
    __delay_cycles(100000);

    //
    // initialize the LCM module
    //
    // 1. Set 4-bit input
    //
    LCM_OUT &= ~LCM_PIN_RS;
    LCM_OUT &= ~LCM_PIN_EN;

    LCM_OUT = 0x20;
    PulseLcm();

    //
    // set 4-bit input - second time.
    // (as reqd by the spec.)
    //
    SendByte(0x28, FALSE);

    //
    // 2. Display on, cursor on, blink cursor
    //
    SendByte(0x0E, FALSE);

    //
    // 3. Cursor move auto-increment
    //
    SendByte(0x06, FALSE);
}

//
// Routine Desc
//
// Print a string of characters to the screen
//
// Parameters:
//
// Text - null terminated string of chars
//
// Returns
//
// void.
//
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

//
// Routine Desc
//
// main entry point to the sketch
//
// Parameters
//
// void.
//
// Returns
//
// void.
//

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



