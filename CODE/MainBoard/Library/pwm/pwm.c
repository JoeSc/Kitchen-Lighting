/*
 *  pwm.c
 *  TEST
 *
 *  Created by Joe  Schaack on 8/18/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "pwm.h"


void initPWM ( uint8_t bits_of_timer , uint8_t timerNum , uint8_t bitmask, uint32_t period , uint32_t prescaler)
{
   if(bits_of_timer == 16)
   {
      if(timerNum == 0)
      {
            //LPC_TMR16B0->TCR = 0;
         TMR_TMR16B0TCR = TMR_TMR16B0TCR_COUNTERENABLE_DISABLED;
         SCB_SYSAHBCLKCTRL |= SCB_SYSAHBCLKCTRL_CT16B0;
         
            // I dont think this is necessary ??? lets see
         TMR_TMR16B0EMR = (TMR_TMR16B0EMR_EMC3_HIGH | TMR_TMR16B0EMR_EMC2_HIGH | TMR_TMR16B0EMR_EMC1_HIGH | TMR_TMR16B0EMR_EMC0_HIGH) ;
            // LPC_TMR16B0->EMR = (1<<EMC3)|(1<<EMC2)|(1<<EMC1)|(1<<EMC0)|(1<<3)|(match_enable);
         
         if (bitmask & 0x01)
         {
            IOCON_PIO0_8 &= ~IOCON_PIO0_8_FUNC_MASK;          //LPC_IOCON->PIO0_8           &= ~0x07;	
            IOCON_PIO0_8 |= IOCON_PIO0_8_FUNC_CT16B0_MAT0;     //LPC_IOCON->PIO0_8           |= 0x02;		/* Timer0_16 MAT0 			*/
         }
         if (bitmask & 0x02)
         {
            IOCON_PIO0_9 &= ~IOCON_PIO0_9_FUNC_MASK;          //LPC_IOCON->PIO0_9           &= ~0x07;
            IOCON_PIO0_9 |= IOCON_PIO0_9_FUNC_CT16B0_MAT1;    //LPC_IOCON->PIO0_9           |= 0x02;		/* Timer0_16 MAT1 			*/
         }
#ifdef NOT_JTAG
         if (bitmask & 0x04)
         {
            IOCON_JTAG_TCK_PIO0_10 &= ~IOCON_JTAG_TCK_PIO0_10_FUNC_MASK;         // LPC_IOCON->JTAG_TCK_PIO0_10 &= ~0x07;
            IOCON_JTAG_TCK_PIO0_10 |= IOCON_JTAG_TCK_PIO0_10_FUNC_CT16B0_MAT2;   //LPC_IOCON->JTAG_TCK_PIO0_10 |= 0x03;		/* Timer0_16 MAT2 */
         }
#endif
         
         TMR_TMR16B0PWMC = TMR_TMR16B0PWMC_PWM3_ENABLED | (bitmask);
            //LPC_TMR16B0->PWMC = (1<<3)|(match_enable);
         
         /* Setup the match registers */
         /* set the period value to a global variable */
            //timer16_0_period = period;
            //LPC_TMR16B0->MR3 = timer16_0_period;
            //LPC_TMR16B0->MR0	= timer16_0_period/2;
            //LPC_TMR16B0->MR1	= timer16_0_period/2;
            //LPC_TMR16B0->MR2	= timer16_0_period/2;
         TMR_TMR16B0MR3 = period;
         TMR_TMR16B0MR2 = period / 2;
         TMR_TMR16B0MR1 = period / 2;
         TMR_TMR16B0MR0 = period / 2;
         TMR_TMR16B0PR = prescaler;
         
         /* Set the match control register */
            //LPC_TMR16B0->MCR = 1<<10;				/* Reset on MR3 */
         TMR_TMR16B0MCR = TMR_TMR16B0MCR_MR3_RESET_ENABLED;
         NVIC_EnableIRQ(TIMER_16_0_IRQn);
         TMR_TMR16B0TCR = TMR_TMR16B0TCR_COUNTERENABLE_ENABLED;
            //LPC_TMR16B0->TCR = 1;
      }
      else if ( timerNum == 1)
      {
         /*
          
          */
      }
   }
   
   else 
   {
      if(timerNum == 0)
      {
         
      }
      else if ( timerNum == 1)
      {
         
      }
      
      
      
      
   }   
}

void setPWM ( uint8_t bits_of_timer , uint8_t timerNum , uint8_t chn , uint32_t val)
{
   if(bits_of_timer == 16)
   {
      if(timerNum == 0)
      {
         if (chn == 0)
         {
            TMR_TMR16B0MR0 = val;
         }
         if (chn == 1 )
         {
            TMR_TMR16B0MR1 = val;
         }
#ifdef NOT_JTAG
         if (chn == 2)
         {
            TMR_TMR16B0MR2 = val;
         }
#endif
      }
      else if ( timerNum == 1)
      {
         /*
          
          */
      }
   }
   
   else 
   {
      if(timerNum == 0)
      {
         
      }
      else if ( timerNum == 1)
      {
         
      }
      
      
      
      
   }   
   
   
   
   
   
}







   //