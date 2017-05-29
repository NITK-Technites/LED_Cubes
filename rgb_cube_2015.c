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
#define SCLK595      BIT4
#define LTCH595 	 BIT5

void conv_ascii(int val,char *asc_val)//function to convert in to a set of ascii char to be read by >> in Cpp program(max val=1024)
{
 int i=0;
 for(i=0;val>=1000;i++)
       val-=1000;
 asc_val[0]=(char)(i+48);
 for(i=0;val>=100;i++)
       val-=100;
 asc_val[1]=(char)(i+48);
 for(i=0;val>=10;i++)
       val-=10;
 asc_val[2]=(char)(i+48);
 for(i=0;val>=1;i++)
       val-=1;
 asc_val[3]=(char)(i+48);
}
void serial_println_8(char val)
{
 while (!(IFG2&UCA0TXIFG));                // USCI_A0 TX buffer ready?
  UCA0TXBUF = val;
 //while (!(IFG2&UCA0TXIFG));                // USCI_A0 TX buffer ready?
  //UCA0TXBUF = '\r';
 //while (!(IFG2&UCA0TXIFG));                // USCI_A0 TX buffer ready?
 // UCA0TXBUF = '\n';
}

unsigned char disp_arr[4][6]={{0x00,0x00,0x00,0x00,0x00,0x00},
							  {0x00,0x00,0x00,0x00,0x00,0x00},
							  {0x00,0x00,0x00,0x00,0x00,0x00},
							  {0xFF,0x00,0x00,0xFF,0x00,0x00}};
unsigned char port_anode[4]={0x01,0x02,0x04,0x08};

void InitializeClocks(void);
void SendData(int);
char rx_row=0,rx_col=0;
char strobe;

void main(void)
{
	char layer_no;
    WDTCTL = WDTPW + WDTHOLD;
    //UART
    P1SEL = BIT1 + BIT2 ;                     // P1.1 = RXD, P1.2=TXD
    P1SEL2 = BIT1 + BIT2;
    UCA0CTL1 |= UCSSEL_2;                      // SMCLK
    UCA0BR0 = 104;                   			        // 1MHz 9600
    UCA0BR1 = 0;                           // 1MHz 9600
    UCA0MCTL = UCBRS2 + UCBRS0;               // Modulation UCBRSx = 5
    UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
    IE2 |= UCA0RXIE;
	//595 initialization
	P1DIR = 0;
    P1DIR |= (SIN595 + SCLK595 + LTCH595);
    P1OUT=0;

    P2DIR=0xFF;
    P2OUT=0x00;

	InitializeClocks();						  // Setup clock

    _bis_SR_register(GIE);

	while (1)
    {

		for(layer_no = 0;layer_no < 4;layer_no++)
		{

			SendData(layer_no);
			P2OUT = port_anode[layer_no];
			P1OUT|=LTCH595;
		    P1OUT&=~(LTCH595);
		    //_delay_cycles(1000);

		}


    }
}


void SendData(int layer_no)
{
	char i,temp;
    P1OUT &= ~(SIN595 + LTCH595 + SCLK595);
    for (i = 0; i < 6; i++)
	{
		temp=disp_arr[layer_no][i];
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




#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{
  strobe= UCA0RXBUF;                    // TX -> RXed character
  if(strobe=='S')
  {
	  rx_row=0;
	  rx_col=0;
	  serial_println_8('K');
	  strobe=0;
  }

  else if(!(strobe=='S'))
  {
	  if(rx_row<4)
	  {
		  if(rx_col<6)
			  disp_arr[rx_row][rx_col++]=strobe;
		  else
		  {
			  rx_row++;
			  rx_col=0;
			  disp_arr[rx_row][rx_col++]=strobe;
		  }
	  }
  }

}

