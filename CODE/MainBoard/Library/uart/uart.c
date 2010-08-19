/*
 *  uart.c
 *  TEST
 *
 *  Created by Joe  Schaack on 8/13/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "uart.h"
#include "Library/init/cpu_init.h"

   // Space holder if I decide to use and interrupt to handle the uart
   // since the buffer is 14 bytes long the need mostlikely will never arise
void UART_IRQHandler(void)
{
}


   // Initialize the uart with the baud rate passed to it
void uartInit(uint32_t baudrate)
{
   uint32_t regVal, num;
   
   
   NVIC_DisableIRQ(UART_IRQn);
   
   /* Set 1.6 UART RXD */
   IOCON_PIO1_6 &= ~IOCON_PIO1_6_FUNC_MASK;
   IOCON_PIO1_6 |= IOCON_PIO1_6_FUNC_UART_RXD;
   
   /* Set 1.7 UART TXD */
   IOCON_PIO1_7 &= ~IOCON_PIO1_7_FUNC_MASK;	
   IOCON_PIO1_7 |= IOCON_PIO1_7_FUNC_UART_TXD;
   
   /* Enable UART clock */
   SCB_SYSAHBCLKCTRL |= (SCB_SYSAHBCLKCTRL_UART);
   SCB_UARTCLKDIV = SCB_UARTCLKDIV_DIV1;     /* divided by 1 */
   
   /* 8 bits, no Parity, 1 Stop bit */
   UART_U0LCR = (UART_U0LCR_Word_Length_Select_8Chars |
                 UART_U0LCR_Stop_Bit_Select_1Bits |
                 UART_U0LCR_Parity_Disabled |
                 UART_U0LCR_Parity_Select_OddParity |
                 UART_U0LCR_Break_Control_Disabled |
                 UART_U0LCR_Divisor_Latch_Access_Enabled);
   
   regVal = SCB_UARTCLKDIV;
   num = (((SYS_CLOCK * SCB_SYSAHBCLKDIV)/regVal)/16)/baudrate;
   
   UART_U0DLM = (num & 0xFF00) >> 8;
   UART_U0DLL = num & 0xFF;
   
   /* Set DLAB back to 0 */
   UART_U0LCR = (UART_U0LCR_Word_Length_Select_8Chars |
                 UART_U0LCR_Stop_Bit_Select_1Bits |
                 UART_U0LCR_Parity_Disabled |
                 UART_U0LCR_Parity_Select_OddParity |
                 UART_U0LCR_Break_Control_Disabled |
                 UART_U0LCR_Divisor_Latch_Access_Disabled);
   
   /* Enable and reset TX and RX FIFO. */
   UART_U0FCR = (UART_U0FCR_FIFO_Enabled | 
                 UART_U0FCR_Rx_FIFO_Reset | 
                 UART_U0FCR_Tx_FIFO_Reset); 
   
   regVal = UART_U0LSR;
   
   while (( UART_U0LSR & (UART_U0LSR_THRE|UART_U0LSR_TEMT)) != (UART_U0LSR_THRE|UART_U0LSR_TEMT) );
   while ( UART_U0LSR & UART_U0LSR_RDR_DATA )
   {
      /* Dump data from RX FIFO */
      regVal = UART_U0RBR;
   }
   UART_U0IER = 0x0;
   UART_U0TER = UART_U0TER_TXEN_Enabled;
}

   // Send a character throught the uart
   // currently it waits for the buffer to be empty
   // TODO: look into only waiting when the buffer is full
void uartSend(char data)
{
   while ( !(UART_U0LSR & UART_U0LSR_THRE) ); // sit and spin untill fifo is empty.  
                                              //it would be nice to be able to fill the fifo
                                              // 16 bytes and then wait for it to be empty?
   UART_U0THR = data;
}

   // returns 1 when valid data is in the receive buffer
uint32_t uartDataAvailable()
{
   return UART_U0LSR & UART_U0LSR_RDR_DATA;
}
   // read the data from the receive buffer
char uartReceive()
{
   return UART_U0RBR;
}





