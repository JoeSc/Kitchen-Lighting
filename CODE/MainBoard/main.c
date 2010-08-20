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
#include "lpc111x.h"
   //#define CLKOUT
#include "Library/pwm/pwm.h"
#include "Library/adc/adc.h"


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
   
      //initPWM(16, 0, (1<<0), 256, 0);
   printf("Starting in main\n");
   cmdInit();

   while (1)
   {
      cmdPoll();
   }
}





void __putchar(const char c)
{
   uartSend(c);
}
int puts(const char * str)
{
   while(*str) __putchar(*str++);
   return 0;
}


