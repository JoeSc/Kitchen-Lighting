/**************************************************************************/
/*! 
 @file     commands.c
 @author   K. Townsend (microBuilder.eu)
 @date     22 March 2010
 @version  0.10
 
 @brief    Entry point for all commands in the 'core/cmd' command-line
 interpretter.  Every menu item defined in cmd_tbl.h points
 to a method that should be located here for convenience
 sake.  (The only exception is 'help', which exists in
 any project and is handled directly by core/cmd/cmd.c). All
 methods have exactly the same signature (argc + argv).
 
 @section LICENSE
 
 Software License Agreement (BSD License)
 
 Copyright (c) 2010, microBuilder SARL
 All rights reserved.
 
 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:
 1. Redistributions of source code must retain the above copyright
 notice, this list of conditions and the following disclaimer.
 2. Redistributions in binary form must reproduce the above copyright
 notice, this list of conditions and the following disclaimer in the
 documentation and/or other materials provided with the distribution.
 3. Neither the name of the copyright holders nor the
 names of its contributors may be used to endorse or promote products
 derived from this software without specific prior written permission.
 
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ''AS IS'' AND ANY
 EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
 DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/**************************************************************************/

#include "stdio.h"
#include "string.h"   // memset
#include <ctype.h>   // isdigit, isspace, etc.
#include "Library/init/cpu_init.h"
#include "Library/pwm/pwm.h"
#include "Library/adc/adc.h"

#include "cmd.h"


/**************************************************************************/
/*!
 @brief  Attempts to convert the supplied decimal or hexadecimal
 string to the matching 32-bit value.  All hexadecimal values
 must be preceded by either '0x' or '0X' to be properly parsed.
 
 @param[in]  s
 Input string
 @param[out] result
 Signed 32-bit integer to hold the conversion results
 
 @section Example
 
 @code
 char *hex = "0xABCD";
 char *dec = "1234";
 
 // Convert supplied values to integers
 int32_t hexValue, decValue;
 getNumber (hex, &hexValue);
 getNumber (dec, &decValue);
 
 @endcode
 */
/**************************************************************************/
static int getNumber (char *s, int32_t *result)
{
   int32_t value;
   uint32_t mustBeHex = TRUE;
   uint32_t sgn = 1;
   const unsigned char hexToDec [] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 255, 255, 255, 255, 255, 255, 255, 10, 11, 12, 13, 14, 15};
   
   if (!s)
      return 0;
   
      // Check if this is a hexadecimal value
   if ((strlen (s) > 2) && (!strncmp (s, "0x", 2) || !strncmp (s, "0X", 2)))
   {
      mustBeHex = TRUE;
      s += 2;
   }
   
   if(!strncmp(s, "n", 1))
   {
      mustBeHex = FALSE;
      s+=1;
   }
      // Check for negative sign
   if (!mustBeHex && *s && (*s == '-'))
   {
      sgn = -1;
      s++;
   }
   
      // Try to convert value
   for (value = 0; *s; s++)
   {
      if (mustBeHex && isxdigit (*s))
         value = (value << 4) | hexToDec [toupper (*s) - '0'];
      else if (isdigit (*s))
         value = (value * 10) + (*s - '0');
      else
      {
         printf ("Malformed number. Must be decimal number, or hex value preceeded by '0x'%s", "\n");
         return 0;
      }
   }
   
      // Set number to negative value if required
   if (!mustBeHex)
      value *= sgn;
   
   *result = value;
   
   return 1;
}
/*static int getNumber (char *s, int32_t *result)
 {
 int32_t value;
 uint32_t mustBeHex = FALSE;
 uint32_t sgn = 1;
 const unsigned char hexToDec [] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 255, 255, 255, 255, 255, 255, 255, 10, 11, 12, 13, 14, 15};
 
 if (!s)
 return 0;
 
 // Check if this is a hexadecimal value
 if ((strlen (s) > 2) && (!strncmp (s, "0x", 2) || !strncmp (s, "0X", 2)))
 {
 mustBeHex = TRUE;
 s += 2;
 }
 
 // Check for negative sign
 if (!mustBeHex && *s && (*s == '-'))
 {
 sgn = -1;
 s++;
 }
 
 // Try to convert value
 for (value = 0; *s; s++)
 {
 if (mustBeHex && isxdigit (*s))
 value = (value << 4) | hexToDec [toupper (*s) - '0'];
 else if (isdigit (*s))
 value = (value * 10) + (*s - '0');
 else
 {
 printf ("Malformed number. Must be decimal number, or hex value preceeded by '0x'%s", "\n");
 return 0;
 }
 }
 
 // Set number to negative value if required
 if (!mustBeHex)
 value *= sgn;
 
 *result = value;
 
 return 1;
 }*/


/**************************************************************************/
/*! 
 'hello' command handler
 */
/**************************************************************************/
void cmd_hello(uint8_t argc, char **argv)
{
   if (argc > 0)
   {
      printf("Hello %s%s", argv[0],"\n");
   }
   else
   {
      printf("Hello World!%s", "\n");
   }
}

/**************************************************************************/
/*! 
 'sysinfo' command handler
 */
/**************************************************************************/
void cmd_sysinfo(uint8_t argc, char **argv)
{
   printf("%-30s : %d Hz %s", "Core System Clock", SYSCLK, "\n");
   printf("%-30s : %d mS %s", "Systick Timer Delay", SYSTICK_LED_MS, "\n");
   printf("%-30s : %d BPS %s", "UART Baud Rate", BAUDRATE, "\n");
   
#ifdef CFG_CHIBI
   chb_pcb_t *pcb = chb_get_pcb();
   printf("%-30s : %s%s", "Wireless Frequency", "868 MHz", "\n");
   printf("%-30s : 0x%04X%s", "Wireless Node Address", pcb->src_addr, "\n");
#endif
   
#ifdef CFG_LM75B
   int32_t temp = 0;
   lm75bGetTemperature(&temp);
   temp *= 125;
   printf("%-30s : %d.%d C%s", "System Temperature", temp / 1000, temp % 1000, "\n");
#endif
   
      // printf("%-30s : %s", "<Property Name>", "\n");
}


void cmd_md(uint8_t argc, char **argv)
{
   
   uint32_t addr;
   uint32_t numLongs = 8;
   /* if( (strlen(argv[0]) < 8) && strncmp(argv[0], "0x", 2))
    {
    printf("Invalid hex number");
    return;
    }*/
   
   if(!getNumber (argv[0], &addr))
      return 0;
   
   
   
   addr = addr - addr % 0x4;      //round to nearest 4
   
   uint32_t divnum = addr & 0xF;
   if(!divnum)
      divnum = 0x10;
   
   if(argc > 1)
   {
      getNumber(argv[1], &numLongs);
   }
   
   uint32_t end = addr + numLongs*4;
   
   for(; addr<end ; addr+=4)
   {
      if( !((addr - divnum) % 0x10))
         printf("\n0x%08x: ",addr);
      uint32_t val = (*(uint32_t *) (addr));
      printf("%08x ", val);
   }
   
   return 0;
}

void cmd_mw(uint8_t argc, char **argv)
{
   uint32_t addr;
   uint32_t data;
   uint32_t val;
   
   if(!getNumber (argv[0], &addr))
      return 0;
   
   addr = addr - addr % 0x4;
   
   if(!getNumber (argv[1], &data))
      return 0;
   
   val = (*(uint32_t *) (addr));
   printf("\nData was 0x%08x\n",val);
   
   (*(uint32_t *) (addr)) = data;
   
   return 0;
}

void cmd_pwmInit(uint8_t argc, char **argv)
{
   uint32_t whichtimer, timernum , enablemask , period;
   if (!getNumber(argv[0], &whichtimer))
      return 0;
   
   if (!getNumber(argv[1], &timernum))
      return 0;
   
   if (!getNumber(argv[2], &enablemask))
      return 0;
   if(!getNumber(argv[3], &period))
      return 0;
   printf("Setting timer%dB%d mat 0x%X to period of %d\n", whichtimer,timernum,enablemask,period);
   initPWM(whichtimer, timernum, enablemask, period, 0  );
}

void cmd_pwmSet(uint8_t argc, char **argv)
{
   uint32_t whichtimer, timernum , enablemask , period;
   if (!getNumber(argv[0], &whichtimer))
      return 0;
   
   if (!getNumber(argv[1], &timernum))
      return 0;
   
   if (!getNumber(argv[2], &enablemask))
      return 0;
   if(!getNumber(argv[3], &period))
      return 0;
   setPWM(whichtimer, timernum, enablemask, period)   ;
   printf("<#message#>");
}

void cmd_adcInit( uint8_t argc, char **argv)
{
   uint32_t chn;
   if (!getNumber(argv[0], &chn))
      return 0;
   
   InitADC(1<<chn);
   printf("ADC chn %d inited\n",chn);
   
}
   
void cmd_adcRead( uint8_t argc, char **argv)
{
   uint32_t chn;
   if (!getNumber(argv[0], &chn))
      return 0;
   
   printf("Read ADC ch.%d  Got value of %d",chn, getAnalog(1<<chn));
}






