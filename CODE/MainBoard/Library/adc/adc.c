/*
 *  adc.c
 *  TEST
 *
 *  Created by Joe  Schaack on 8/18/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "adc.h"
#include "Library/init/cpu_init.h"
#include "Library/uart/uart.h"
#define ADCCLK 4500000
#define ADC0 0x1
#define ADC1 0x2
#define ADC2 0x4
#define ADC3 0x8
#define ADC4 0x1
#define ADC5 0x2
#define ADC6 0x4
#define ADC7 0x8


void InitADC (uint8_t bitmask)
{
   SCB_PDRUNCFG &= ~SCB_PDRUNCFG_ADC;
   SCB_SYSAHBCLKCTRL |= SCB_SYSAHBCLKCTRL_ADC;
   if (bitmask & 0x1)
   {
      IOCON_JTAG_TDI_PIO0_11 &= ~(IOCON_JTAG_TDI_PIO0_11_FUNC_MASK | IOCON_JTAG_TDI_PIO0_11_ADMODE_MASK);
      IOCON_JTAG_TDI_PIO0_11 |= (IOCON_JTAG_TDI_PIO0_11_FUNC_AD0 | IOCON_JTAG_TDI_PIO0_11_ADMODE_ANALOG);
   }
   if (bitmask & 0x2)
   {
      IOCON_JTAG_TMS_PIO1_0 &= ~(IOCON_JTAG_TMS_PIO1_0_FUNC_MASK | IOCON_JTAG_TMS_PIO1_0_ADMODE_MASK);
      IOCON_JTAG_TMS_PIO1_0 |= (IOCON_JTAG_TMS_PIO1_0_FUNC_AD1 | IOCON_JTAG_TMS_PIO1_0_ADMODE_ANALOG);
   }
   if (bitmask & 0x4)
   {
      IOCON_JTAG_TDO_PIO1_1 &= ~(IOCON_JTAG_TDO_PIO1_1_FUNC_MASK | IOCON_JTAG_TDO_PIO1_1_ADMODE_MASK) ;
      IOCON_JTAG_TDO_PIO1_1 |= (IOCON_JTAG_TDO_PIO1_1_FUNC_AD2 | IOCON_JTAG_TDO_PIO1_1_ADMODE_ANALOG);
   }
   if (bitmask & 0x8)
   {
      IOCON_JTAG_nTRST_PIO1_2 &= ~(IOCON_JTAG_nTRST_PIO1_2_FUNC_MASK | IOCON_JTAG_nTRST_PIO1_2_ADMODE_MASK);  
      IOCON_JTAG_nTRST_PIO1_2 |= (IOCON_JTAG_nTRST_PIO1_2_FUNC_AD3 | IOCON_JTAG_nTRST_PIO1_2_ADMODE_ANALOG);  
   }
   if (bitmask & 0x10)
   {
      IOCON_SWDIO_PIO1_3 &= ~(IOCON_SWDIO_PIO1_3_FUNC_MASK | IOCON_SWDIO_PIO1_3_ADMODE_MASK);
      IOCON_SWDIO_PIO1_3 |= (IOCON_SWDIO_PIO1_3_FUNC_AD4 | IOCON_SWDIO_PIO1_3_ADMODE_ANALOG);
   }
   if (bitmask & 0x20)
   {
      IOCON_PIO1_4 &= ~(IOCON_PIO1_4_FUNC_MASK | IOCON_PIO1_4_ADMODE_MASK);
      IOCON_PIO1_4 |= (IOCON_PIO1_4_FUNC_AD5 | IOCON_PIO1_4_ADMODE_ANALOG);
   }
   if (bitmask & 0x40)
   {
      IOCON_PIO1_10 &= ~(IOCON_PIO1_10_FUNC_MASK | IOCON_PIO1_10_ADMODE_MASK);
      IOCON_PIO1_10 |= (IOCON_PIO1_10_FUNC_AD6 | IOCON_PIO1_10_ADMODE_ANALOG);
   }
   if (bitmask & 0x80)
   {
      IOCON_PIO1_11 &= ~(IOCON_PIO1_11_FUNC_MASK | IOCON_PIO1_11_ADMODE_MASK);
      IOCON_PIO1_11 |= (IOCON_PIO1_11_FUNC_AD7 | IOCON_PIO1_11_ADMODE_ANALOG);
   }

   ADC_AD0CR = (((SYSCLK/ADCCLK) + 1) << 8) & ~ADC_AD0CR_SEL_MASK;
   ADC_AD0INTEN = ADC_AD0INTEN_ADGINTEN_DISABLE;
      
}
   //currently you use the actual bitmask value (1<<channel)
uint32_t getAnalog (uint32_t shiftval)
{
   if( shiftval > (1<<7) )
      return -1;
   ADC_AD0CR &= ~ADC_AD0CR_SEL_MASK;
   ADC_AD0CR |= (shiftval | ADC_AD0CR_START_STARTNOW);
   while (!(ADC_AD0GDR && ADC_AD0GDR_DONE)); 
   
   return (ADC_AD0GDR & ADC_AD0GDR_RESULT_MASK) >> 6;
}
      
      
      
