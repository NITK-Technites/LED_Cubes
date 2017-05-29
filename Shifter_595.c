#include <msp430G2553.h>
/*------]]]]-------------------------------------------
                           74HC595
                      -----------------
                  -->|Q1            VCC|<-- Ground
                     |                 |
                  -->|Q2             Q0|--> unused
                     |                 |
                  -->|Q3            DIN|--> unused
                     |                 |
                  <--|Q4             OE|--> Open
                     |                 |
                  <--|Q5           LTCH|<-- Pulled high
                     |                 |
                  <--|Q6           SCLK|--> SIN_595
                     |                 |
                  <--|Q7             MR|--> unused
                     |                 |
                  <--|GND          DOUT|<-- unused
                     |                 |
                      -----------------
  -------------------------------------------------*/
  
  //PIN 595
#define SIN595  	 BIT7
#define SCL595       BIT4
#define LTCH595 	 BIT5

unsigned char disp_arr[2]={0xAA,0x55};
void InitializeClocks(void);
void SendData(void);

void main(void)
{
	char temp_disp;
    WDTCTL = WDTPW + WDTHOLD;

	//595 initialization
	P1DIR = 0;
    P1DIR |= (SIN595 + SCLK595 + LTCH595);
    P1OUT=0;



	InitializeClocks();						  // Setup clock
//    IE2 |= UCA0RXIE;                          // Enable USCI_A0 RX interrupt
    _bis_SR_register(GIE);

	while (1)
    {
		SendData(); // Send current Step data to TLC5940
		P1OUT|=LTCH595;
        P1OUT&=~(LTCH595);
        _delay_cycles(100000);
        temp_disp=disp_arr[0];
        disp_arr[0]=disp_arr[1];
        disp_arr[1]=temp_disp;
    }

}


void SendData(void)
{
	char i,temp;
    P1OUT &= ~(SIN595 + LTCH595 + SCLK595);
    for (i = 0; i < 2; i++)
	{
		temp=disp_arr[i];
		P1OUT&=~(SIN595);
		P1OUT|=SIN595&temp;
		P1OUT|=SCLK595;
		P1OUT&=~SCLK595;
		temp=temp<<1;

		P1OUT&=~(SIN595);
		P1OUT|=SIN595&temp;
		P1OUT|=SCLK595;
		P1OUT&=~SCLK595;
		temp=temp<<1;

		P1OUT&=~(SIN595);
		P1OUT|=SIN595&temp;
		P1OUT|=SCLK595;
		P1OUT&=~SCLK595;
		temp=temp<<1;

		P1OUT&=~(SIN595);
		P1OUT|=SIN595&temp;
		P1OUT|=SCLK595;
		P1OUT&=~SCLK595;
		temp=temp<<1;

		P1OUT&=~(SIN595);
		P1OUT|=SIN595&temp;
		P1OUT|=SCLK595;
		P1OUT&=~SCLK595;
		temp=temp<<1;

		P1OUT&=~(SIN595);
		P1OUT|=SIN595&temp;
		P1OUT|=SCLK595;
		P1OUT&=~SCLK595;
		temp=temp<<1;

		P1OUT&=~(SIN595);
		P1OUT|=SIN595&temp;
		P1OUT|=SCLK595;
		P1OUT&=~SCLK595;
		temp=temp<<1;

		P1OUT&=~(SIN595);
		P1OUT|=SIN595&temp;
		P1OUT|=SCLK595;
		P1OUT&=~SCLK595;
		temp=temp<<1;
	}

}

void InitializeClocks(void)
{
  BCSCTL1 = CALBC1_1MHZ;
  DCOCTL = CALDCO_1MHZ;
}
