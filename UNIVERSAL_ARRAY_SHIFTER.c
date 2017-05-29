#include <msp430G2553.h>
/*------]]]]-------------------------------------------
TECHNITES 2014
UNIVERSAL 5916 ARRAY_SHIFTER

                        MSP430G2553
                      -----------------
            3.3 V -->|Vcc          Gnd |<-- Ground
                     |                 |
          Unused  -->|P1.0         P2.7|--> unused
                     |                 |
               Rx -->|P1.1         P2.6|--> unused
                     |                 |
               Tx <--|P1.2         TST |--> Open
                     |                 |
                  <--|P1.3         RST |<-- Pulled high
                     |                 |
        SCLK_5916 <--|P1.4         P1.7|--> SIN_5916
                     |                 |
        LTCH_5916 <--|P1.5         P1.6|--> unused
                     |                 |
          SIN_595 <--|P2.0         P2.5|<-- unused
                     |                 |
         SCLK_595 <--|P2.1         P2.4|<--
                     |                 |
         LTCH_595 <--|P2.2         P2.3|<--
                      -----------------
  -------------------------------------------------*/

//PIN 5916 PORT1
#define SIN5916 	 BIT5
#define SCLK5916     BIT4
#define LTCH5916 	 BIT3
#define N_channel	 2

unsigned char disp_arr[N_channel]={0xAA,0x55};
void InitializeClocks(void);
void SendData(void);

void main(void)
{
	char temp_disp,i;
    WDTCTL = WDTPW + WDTHOLD;
	//TLC initialization
	P1DIR = 0;
    P1DIR |= (SIN5916 + SCLK5916 + LTCH5916);
    P1OUT=0;
	InitializeClocks();						  // Setup clock
    _bis_SR_register(GIE);

	while (1)
    {
		SendData();
		P1OUT|=LTCH5916;
        P1OUT&=~(LTCH5916);
        _delay_cycles(10000);
        temp_disp=disp_arr[0];
        for(i=0;i<N_channel-1;i++)
        {
        disp_arr[i]=disp_arr[i+1];
        }
        disp_arr[N_channel-1]=temp_disp;
    }

}


void SendData(void)
{
	char i,temp;
    P1OUT &= ~(SIN5916 + LTCH5916 + SCLK5916);
    for (i = 0; i < N_channel; i++)
	{
		temp=disp_arr[i];
		if(temp&BIT7)
			P1OUT|=SIN5916;
		else
			P1OUT&=~SIN5916;
		P1OUT|=SCLK5916;
		P1OUT&=~SCLK5916;
		temp=temp<<1;

		if(temp&BIT7)
			P1OUT|=SIN5916;
		else
			P1OUT&=~SIN5916;
		P1OUT|=SCLK5916;
		P1OUT&=~SCLK5916;
		temp=temp<<1;

		if(temp&BIT7)
			P1OUT|=SIN5916;
		else
			P1OUT&=~SIN5916;
		P1OUT|=SCLK5916;
		P1OUT&=~SCLK5916;
		temp=temp<<1;

		if(temp&BIT7)
			P1OUT|=SIN5916;
		else
			P1OUT&=~SIN5916;
		P1OUT|=SCLK5916;
		P1OUT&=~SCLK5916;
		temp=temp<<1;

		if(temp&BIT7)
			P1OUT|=SIN5916;
		else
			P1OUT&=~SIN5916;
		P1OUT|=SCLK5916;
		P1OUT&=~SCLK5916;
		temp=temp<<1;

		if(temp&BIT7)
			P1OUT|=SIN5916;
		else
			P1OUT&=~SIN5916;
		P1OUT|=SCLK5916;
		P1OUT&=~SCLK5916;
		temp=temp<<1;

		if(temp&BIT7)
			P1OUT|=SIN5916;
		else
			P1OUT&=~SIN5916;
		P1OUT|=SCLK5916;
		P1OUT&=~SCLK5916;
		temp=temp<<1;

		if(temp&BIT7)
			P1OUT|=SIN5916;
		else
			P1OUT&=~SIN5916;
		P1OUT|=SCLK5916;
		P1OUT&=~SCLK5916;
		temp=temp<<1;
	}

}

void InitializeClocks(void)
{
  BCSCTL1 = CALBC1_1MHZ;
  DCOCTL = CALDCO_1MHZ;
}




