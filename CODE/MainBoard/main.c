/*
 *  main.c
 *  Test1
 *
 *
 *  Created by Joe  Schaack on 8/13/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */




   //THE COMMAND LINE INTERFACE FROM MICROBUILDER.DE LOOKS INTERESETING ...
#include "Library/uart/stdio.c"
#include "Library/init/cpu_init.h"
#include "Library/uart/uart.h"
#include "Library/cli/cmd.h"
   //#define CLKOUT



int main (void)
{
   cpuInit();
   uartInit(BAUDRATE);

   
#ifdef CLKOUT
   IOCON_PIO0_1 &= ~(0x3f);
   IOCON_PIO0_1 |= IOCON_PIO0_1_FUNC_CLKOUT;
   
   SCB_CLKOUTCLKSEL |= 0x3; // select system osci
   
   SCB_CLKOUTCLKUEN = SCB_CLKOUTCLKUEN_UPDATE;
   SCB_CLKOUTCLKUEN = SCB_CLKOUTCLKUEN_DISABLE;
   SCB_CLKOUTCLKUEN = SCB_CLKOUTCLKUEN_UPDATE;
   while( !(SCB_CLKOUTCLKUEN & SCB_CLKOUTCLKUEN_UPDATE));
   SCB_CLKOUTCLKDIV = SCB_CLKOUTCLKDIV_DIV1;
   
#endif   
   
   
   

   printf("Starting in main\n");
   cmdInit();

   while (1)
   {
      cmdPoll();

      
//      Delay(1);
//      GPIO_GPIO3DATA ^= (1<<3);
//      while(uartDataAvailable())
//      {
//         printf("i got : %c   Time is %d \n",uartReceive(),millis());
//         int i;
//         for (i='a'; i<'z'; i++) 
//         {
//            uartSend(i);
//            uartSend("\n");
//         }
//      }
   }
}



/**************************************************************************/
/*! 
 @brief Sends a single byte over UART.
 
 @param[in]  byte
 Byte value to send
 */
/**************************************************************************/
void __putchar(const char c)
{
   uartSend(c);
}

/**************************************************************************/
/*! 
 @brief Sends a single byte over UART.
 
 @param[in]  byte
 Byte value to send
 */
/**************************************************************************/
int puts(const char * str)
{
   while(*str) __putchar(*str++);
   return 0;
}


