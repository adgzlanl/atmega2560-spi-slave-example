#include<avr/io.h>

#define FOSC 16000000// 8Mhz Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1
uint16_t value2,value3;
void USART_Transmit( unsigned char data );
void USART_Init( unsigned int ubrr);
unsigned char USART_Receive( void );

#define DD_MISO    PB3

#define DD_MOSI    PB2

#define DD_SCK     PB1

#define DD_SS      PB0

#define DDR_SPI    DDRB

void SPI_SlaveInit(void)
{
  // Set MISO output, all others input
  DDR_SPI |= (1<<DD_MISO);
  // Enable SPI
  SPCR |= (1<<SPE)|(1<<SPR0);
}

char SPI_SlaveReceive(void)
{
  // Wait for reception complete
  // SPI Status Reg & 1<<SPI Interrupt Flag
  while(!(SPSR & (1<<SPIF)));

  // Return data register
  return SPDR;
}

void USART_Init( unsigned int ubrr)
{
  /* Set baud rate */
  UBRR0H = (unsigned char)(ubrr>>8);
  UBRR0L = (unsigned char)ubrr;
  /* Enable receiver and transmitter */
  UCSR0B = (1<<RXEN0)|(1<<TXEN0);
  /* Set frame format: 8data, 2stop bit */
  UCSR0C = (1<<USBS0)|(3<<UCSZ00);
}

/*******************************************************/
void USART_Transmit( unsigned char data )
{
  /* Wait for empty transmit buffer */
  while ( !( UCSR0A & (1<<UDRE0)) );
  /* Put data into buffer, sends the data */
  UDR0 = data;
}
/*******************************************************/
unsigned char USART_Receive( void )
{
  /* Wait for data to be received */
  while ( !(UCSR0A & (1<<RXC0)) );
  /* Get and return received data from buffer */
  return UDR0;
}

int main(void)
{
  SPI_SlaveInit();
  USART_Init (MYUBRR);
  pinMode(13,OUTPUT);
 

  while(1)
  {
        USART_Transmit(SPI_SlaveReceive());
        USART_Transmit(13);
        USART_Transmit(10);
        
    if (SPI_SlaveReceive()=='A')
    {
      digitalWrite(13,HIGH);  
    }


    if (SPI_SlaveReceive()=='B')
    {
      digitalWrite(13,LOW);  
    }
  }

}