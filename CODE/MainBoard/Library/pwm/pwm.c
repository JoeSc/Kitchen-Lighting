/*
 *  pwm.c
 *  TEST
 *
 *  Created by Joe  Schaack on 8/18/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "pwm.h"


#define CH0    0x01
#define CH1    0x02
#define CH2    0x04
#define CH3    0x08

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
         TMR_TMR16B0EMR = (TMR_TMR16B0EMR_EMC3_HIGH | TMR_TMR16B0EMR_EMC2_HIGH 
                           | TMR_TMR16B0EMR_EMC1_HIGH | TMR_TMR16B0EMR_EMC0_HIGH) ;
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
#ifdef NO_JTAG
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
         TMR_TMR16B1TCR = TMR_TMR16B1TCR_COUNTERENABLE_DISABLED;
          /* Some of the I/O pins need to be clearfully planned if
          you use below module because JTAG and TIMER CAP/MAT pins are muxed. */
         SCB_SYSAHBCLKCTRL |= SCB_SYSAHBCLKCTRL_CT16B1;
            //LPC_SYSCON->SYSAHBCLKCTRL |= (1<<8);
         
         /* Setup the external match register */
         TMR_TMR16B1EMR = (TMR_TMR16B1EMR_EMC3_HIGH | TMR_TMR16B1EMR_EMC2_HIGH | TMR_TMR16B1EMR_EMC1_HIGH | TMR_TMR16B1EMR_EMC0_HIGH);
            //LPC_TMR16B1->EMR = (1<<EMC3)|(1<<EMC2)|(1<<EMC1)|(1<<EMC0)|(1<<3)|(match_enable);
            //^ was 2
         /* Setup the outputs */
         /* If match0 is enabled, set the output */
         if (bitmask & 0x01)
         {
            IOCON_PIO1_9 &= ~IOCON_PIO1_9_FUNC_MASK;
               //LPC_IOCON->PIO1_9           &= ~0x07;	
            IOCON_PIO1_9 |= IOCON_PIO1_9_FUNC_CT16B1_MAT0;
               //LPC_IOCON->PIO1_9           |= 0x01;		/* Timer1_16 MAT0 */
         }
         /* If match1 is enabled, set the output */
         if (bitmask & 0x02)
         {
            IOCON_PIO1_10 &= ~IOCON_PIO1_10_FUNC_MASK;
               //LPC_IOCON->PIO1_10          &= ~0x07;
            IOCON_PIO1_10 |= IOCON_PIO1_10_FUNC_CT16B1_MAT1;
               //LPC_IOCON->PIO1_10          |= 0x02;		/* Timer1_16 MAT1 */
         }
         
         /* Enable the selected PWMs and enable Match3 */
         TMR_TMR16B1PWMC = (TMR_TMR16B1PWMC_PWM3_ENABLED) | (bitmask);
            //LPC_TMR16B1->PWMC = (1<<3)|(match_enable);
         
         /* Setup the match registers */
         /* set the period value to a global variable */
            //timer16_1_period 	= period;
         TMR_TMR16B1MR3 = period;
         TMR_TMR16B1MR2 = period/2;
         TMR_TMR16B1MR1 = period/2;
         TMR_TMR16B1MR0 = period/2;
         TMR_TMR16B1PR = prescaler;

            // LPC_TMR16B1->MR3 	= timer16_1_period;
            //         LPC_TMR16B1->MR0	= timer16_1_period/2;
            //         LPC_//TMR16B1->MR1	= timer16_1_period/2;
//         LPC_TMR16B1->MR2	= timer16_1_period/2;
         
         /* Set match control register */
         TMR_TMR16B1MCR =TMR_TMR16B1MCR_MR3_RESET_ENABLED;
            //LPC_TMR16B1->MCR = 1<<10;// | 1<<9;				/* Reset on MR3 */
         NVIC_EnableIRQ(TIMER_16_1_IRQn);
         TMR_TMR16B1TCR = TMR_TMR16B1TCR_COUNTERENABLE_ENABLED;

      }
   }
   
   else 
   {
      if(timerNum == 0)
      {
         TMR_TMR32B0TCR = TMR_TMR32B0TCR_COUNTERENABLE_DISABLED;
         /* Some of the I/O pins need to be clearfully planned if
          you use below module because JTAG and TIMER CAP/MAT pins are muxed. */
            //LPC_SYSCON->SYSAHBCLKCTRL |= (1<<9);
         SCB_SYSAHBCLKCTRL |= SCB_SYSAHBCLKCTRL_CT32B0;
         /* Setup the external match register */
            //LPC_TMR32B0->EMR = (1<<EMC3)|(1<<EMC2)|(1<<EMC1)|(1<<EMC0)|(1<<0)|(match_enable);
         TMR_TMR32B0EMR = ( TMR_TMR32B0EMR_EMC3_HIGH | TMR_TMR32B0EMR_EMC2_HIGH 
                           | TMR_TMR32B0EMR_EMC1_HIGH | TMR_TMR32B0EMR_EMC0_HIGH );
         /* Setup the outputs */
         /* If match0 is enabled, set the output */
         if (bitmask & 0x01)
         {
               //THIS IS THE RX PIN		LPC_IOCON->PIO1_6           &= ~0x07;
               //		  	LPC_IOCON->PIO1_6           |= 0x02;		/* Timer0_32 MAT0 */
         }
         /* If match1 is enabled, set the output */
         if (bitmask & 0x02)
         {
               //THIS IS THE TX PIN   LPC_IOCON-> PIO1_7           &= ~0x07;
               //LPC_IOCON->PIO1_7           |= 0x02;		/* Timer0_32 MAT1 */
         }
         /* If match2 is enabled, set the output */
         if (bitmask & 0x04)
         {
               //LPC_IOCON->PIO0_1           &= ~0x07;	
            IOCON_PIO0_1 &= ~IOCON_PIO0_1_FUNC_MASK;
               //LPC_IOCON->PIO0_1           |= 0x02;		/* Timer0_32 MAT2 */
            IOCON_PIO0_1 |= IOCON_PIO0_1_FUNC_CT32B0_MAT2;
         }
         /* If match3 is enabled, set the output */
         if (bitmask & 0x08)
         {
               //LPC_IOCON->JTAG_TDI_PIO0_11 &= ~0x07;	
            IOCON_JTAG_TDI_PIO0_11 &= ~IOCON_JTAG_TDI_PIO0_11_FUNC_CT32B0_MAT3;
               //LPC_IOCON->JTAG_TDI_PIO0_11 |= 0x03;		/* Timer0_32 MAT3 */
            IOCON_JTAG_TDI_PIO0_11 |= IOCON_JTAG_TDI_PIO0_11_FUNC_CT32B0_MAT3;
         }
         
         /* Enable the selected PWMs and enable Match3 */
            //LPC_TMR32B0->PWMC = (1<<0)|(match_enable);
         TMR_TMR32B0PWMC = TMR_TMR32B0PWMC_PWM0_ENABLED | (bitmask);
         /* Setup the match registers */
         /* set the period value to a global variable */
            // timer32_0_period = period;
            //LPC_TMR32B0->MR0 = timer32_0_period;
         TMR_TMR32B0MR0 = period;
         TMR_TMR32B0MR1 = period/2;
         TMR_TMR32B0MR2 = period/2;
         TMR_TMR32B0MR3 = period/2;
         TMR_TMR32B0PR = prescaler;

            //LPC_TMR32B0->MR3	= timer32_0_period/2;	///2;
            //LPC_TMR32B0->MR1	= timer32_0_period/2;
            //LPC_TMR32B0->MR2	= timer32_0_period/2;
         
            //LPC_TMR32B0->MCR = 2;				/* Reset on MR3 */
         TMR_TMR32B0MCR = TMR_TMR32B0MCR_MR0_RESET_ENABLED;
         
         NVIC_EnableIRQ(TIMER_32_0_IRQn);
         TMR_TMR32B0TCR = TMR_TMR32B0TCR_COUNTERENABLE_ENABLED;
  
      }
      else if ( timerNum == 1)
      {
         TMR_TMR32B1TCR = TMR_TMR32B1TCR_COUNTERENABLE_DISABLED;
         /* Some of the I/O pins need to be clearfully planned if
          you use below module because JTAG and TIMER CAP/MAT pins are muxed. */
            //LPC_SYSCON->SYSAHBCLKCTRL |= (1<<10);
         SCB_SYSAHBCLKCTRL |= SCB_SYSAHBCLKCTRL_CT32B1;
         /* Setup the external match register */
            //LPC_TMR32B1->EMR = (1<<EMC3)|(1<<EMC2)|(2<<EMC1)|(1<<EMC0)|(1<<3)|(match_enable);
         TMR_TMR32B1EMR = ( TMR_TMR32B1EMR_EMC3_HIGH | TMR_TMR32B1EMR_EMC2_HIGH 
                           | TMR_TMR32B1EMR_EMC1_HIGH | TMR_TMR32B1EMR_EMC0_HIGH );         /* Setup the outputs */
         /* If match0 is enabled, set the output */
         if (bitmask & 0x01)
         {
            IOCON_JTAG_TDO_PIO1_1 &= ~IOCON_JTAG_TDO_PIO1_1_FUNC_MASK;
               //LPC_IOCON->JTAG_TDO_PIO1_1  &= ~0x07;	
               //LPC_IOCON->JTAG_TDO_PIO1_1  |= 0x03;		/* Timer1_32 MAT0 */
            IOCON_JTAG_TDO_PIO1_1 |= IOCON_JTAG_TDO_PIO1_1_FUNC_CT32B1_MAT0;
         }
         /* If match1 is enabled, set the output */
         if (bitmask & 0x02)
         {
            IOCON_JTAG_nTRST_PIO1_2 &= IOCON_JTAG_nTRST_PIO1_2_FUNC_MASK;
               //LPC_IOCON->JTAG_nTRST_PIO1_2 &= ~0x07;
               //LPC_IOCON->JTAG_nTRST_PIO1_2 |= 0x03;		/* Timer1_32 MAT1 */
            IOCON_JTAG_nTRST_PIO1_2 |= IOCON_JTAG_nTRST_PIO1_2_FUNC_CT32B1_MAT1;
         }
         /* If match2 is enabled, set the output */
#ifdef NO_JTAG
         if (bitmask & 0x04)
         {
            LPC_IOCON->ARM_SWDIO_PIO1_3   &= ~0x07;
            LPC_IOCON->ARM_SWDIO_PIO1_3   |= 0x03;		/* Timer1_32 MAT2 */
         }
#endif
         /* If match3 is enabled, set the output */
         if (bitmask & 0x08)
         {
               //LPC_IOCON->PIO1_4           &= ~0x07;
            IOCON_PIO1_4 &= ~IOCON_PIO1_4_FUNC_MASK;
               //LPC_IOCON->PIO1_4           |= 0x02;		/* Timer1_32 MAT3 */
            IOCON_PIO1_4 |= IOCON_PIO1_4_FUNC_CT32B1_MAT3;
         }
         

         
         /* Enable the selected PWMs and enable Match3 */
            //LPC_TMR32B1->PWMC = (1<<3)|(match_enable);
         TMR_TMR32B1PWMC = (TMR_TMR32B1PWMC_PWM2_ENABLED | bitmask);
         
         /* Setup the match registers */
         /* set the period value to a global variable */
            //timer32_1_period = period;
//         LPC_TMR32B1->MR0 = timer32_1_period;
//         LPC_TMR32B1->MR3	= timer32_1_period/2;
//         LPC_TMR32B1->MR1	= timer32_1_period/2;
//         LPC_TMR32B1->MR2	= timer32_1_period/2;
         
         TMR_TMR32B1MR2 = period;
         TMR_TMR32B1MR0 = period/2;
         TMR_TMR32B1MR1 = period/2;
         TMR_TMR32B1MR3 = period/2;
         TMR_TMR32B1PR = prescaler;
         
         /* */
            //LPC_TMR32B1->MCR = 1<<0;				/* Reset on MR3 */
         TMR_TMR32B1MCR = TMR_TMR32B1MCR_MR2_RESET_ENABLED;
         NVIC_EnableIRQ(TIMER_32_1_IRQn);
         TMR_TMR32B1TCR = TMR_TMR32B1TCR_COUNTERENABLE_ENABLED;
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
         if (chn == 0)
         {
            TMR_TMR16B1MR0 = val;
         }
         if (chn == 1 )
         {
            TMR_TMR16B1MR1 = val;
         }
         if (chn == 2)
         {
            TMR_TMR16B1MR2 = val;
         }
      }
   }
   
   else 
   {
      if(timerNum == 0)
      {
         if (chn == 0)
         {
            TMR_TMR32B0MR0 = val;
         }
         if (chn == 1 )
         {
            TMR_TMR32B0MR1 = val;
         }
         if (chn == 2)
         {
            TMR_TMR32B0MR2 = val;
         }
         if (chn == 3)
         {
            TMR_TMR32B0MR3 = val;
         }
      }
      else if ( timerNum == 1)
      {
         if (chn == 0)
         {
            TMR_TMR32B1MR0 = val;
         }
         if (chn == 1 )
         {
            TMR_TMR32B1MR1 = val;
         }
         if (chn == 2)
         {
            TMR_TMR32B1MR2 = val;
         }
         if (chn == 3)
         {
            TMR_TMR32B1MR3 = val;
         }
      }
   }   
   
   
   
   
   
}







   //