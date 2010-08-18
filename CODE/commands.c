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

#include <stdio.h>
#include <string.h>   // memset
#include <ctype.h>    // isdigit, isspace, etc.
#include <cpu_init.h>

#include "cmd.h"

#ifdef CFG_CHIBI
#include "drivers/chibi/chb.h"
#include "drivers/chibi/chb_drvr.h"
#endif

#ifdef CFG_I2CEEPROM
#include "drivers/eeprom/mcp24aa/mcp24aa.h"
#endif

#ifdef CFG_LM75B
#include "drivers/sensors/lm75b/lm75b.h"
#endif

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




#ifdef CFG_CHIBI

/**************************************************************************/
/*! 
 Gets or sets the 16-bit sensor node address.  This value can be 
 anything between 1-65534 (0x0001-0xFFFE), and in decimal or
 hexadecimal notation.  All hexadecimal values must be preceded by
 '0x' or '0X' to be properly interpreted (ex. 0x009F).
 */
/**************************************************************************/
void cmd_chibi_addr(uint8_t argc, char **argv)
{
   if (argc > 0)
   {
         // Make sure hexadecimal values are 16-bit or less
      if ((strlen (argv[0]) > 2) && (!strncmp (argv[0], "0x", 2) || !strncmp (argv[0], "0X", 2)) && (strlen (argv[0]) > 6))
      {
         printf("Invalid Address: 16-bit hexadecimal value required (ex. '0x12EF').%s", "\n");
         return;
      }
      
         // Try to convert supplied value to an integer
      int32_t addr;
      getNumber (argv[0], &addr);
      
         // Check for invalid values (getNumber may complain about this as well)
      if (addr <= 0 || addr > 0xFFFF)
      {
         printf("Invalid Address: Value from 1-65534 or 0x0001-0xFFFE required.%s", "\n");
         return;
      }
      if (addr == 0xFFFF)
      {
         printf("Invalid Address: 0xFFFF is reserved for global transmissions.%s", "\n");
         return;
      }
      
         // Write address to EEPROM and update peripheral control block
      chb_set_short_addr((uint16_t)addr);
      chb_pcb_t *pcb = chb_get_pcb();
      printf("Address set to: 0x%04X%s", pcb->src_addr, "\n");
   }
   else
   {
         // Display the current address
      chb_pcb_t *pcb = chb_get_pcb();
      printf("0x%04X%s", pcb->src_addr, "\n");
   }
}

/**************************************************************************/
/*! 
 
 */
/**************************************************************************/
void cmd_chibi_tx(uint8_t argc, char **argv)
{
   uint8_t i, len, *data_ptr, data[50];
   uint16_t addr;
   
      // Make sure hexadecimal address values are 16-bit or less
   if ((strlen (argv[0]) > 2) && (!strncmp (argv[0], "0x", 2) || !strncmp (argv[0], "0X", 2)) && (strlen (argv[0]) > 6))
   {
      printf("Invalid Address: 16-bit hexadecimal value required (ex. '0x12EF').%s", "\n");
      return;
   }
   
      // Try to convert supplied address to an integer
   int32_t addr32;
   getNumber (argv[0], &addr32);
   
      // Check for invalid values (getNumber may complain about this as well)
   if (addr32 <= 0 || addr32 > 0xFFFF)
   {
      printf("Invalid Address: Value from 1-65534 or 0x0001-0xFFFE required.%s", "\n");
      return;
   }
   
      // Address seems to be OK
   addr = (uint16_t)addr32;
   
      // Get message contents
   data_ptr = data;
   for (i=0; i<argc-1; i++)
   {
      len = strlen(argv[i+1]);
      strcpy((char *)data_ptr, (char *)argv[i+1]);
      data_ptr += len;
      *data_ptr++ = ' ';
   }
   *data_ptr++ = '\0';
   
      // Send message
   chb_write(addr, data, data_ptr - data);
}

#endif

#ifdef CFG_I2CEEPROM

/**************************************************************************/
/*! 
 Reads a single byte at the supplied EEPROM address
 */
/**************************************************************************/
void cmd_i2ceeprom_read(uint8_t argc, char **argv)
{
   uint16_t addr;
   uint8_t value; // buffer[1] = { 0x00 };
   
      // Make sure hexadecimal address value is 16-bit or less
   if ((strlen (argv[0]) > 2) && (!strncmp (argv[0], "0x", 2) || !strncmp (argv[0], "0X", 2)) && (strlen (argv[0]) > 6))
   {
      printf("Invalid Address: 16-bit hexadecimal value required (ex. '0x00EF').%s", "\n");
      return;
   }
   
      // Try to convert supplied address to an integer
   int32_t addr32;
   getNumber (argv[0], &addr32);
   
      // Check for invalid values (getNumber may complain about this as well)
   if (addr32 < 0 || addr32 > MCP24AA_MAXADDR)
   {
      printf("Address out of range: Value from 0-%d or 0x0000-0x%04X required.%s", MCP24AA_MAXADDR, MCP24AA_MAXADDR, "\n");
      return;
   }
   
      // Address seems to be OK
   addr = (uint16_t)addr32;
   mcp24aaReadByte(addr, &value);
   
   printf("0x%02X%s", value, "\n");
}

/**************************************************************************/
/*! 
 Writes a single byte at the supplied EEPROM address
 */
/**************************************************************************/
void cmd_i2ceeprom_write(uint8_t argc, char **argv)
{
   uint16_t addr;
   uint8_t val;
   
      // Make sure hexadecimal address value is 16-bit or less
   if ((strlen (argv[0]) > 2) && (!strncmp (argv[0], "0x", 2) || !strncmp (argv[0], "0X", 2)) && (strlen (argv[0]) > 6))
   {
      printf("Invalid Address: 16-bit hexadecimal value required (ex. '0x00EF').%s", "\n");
      return;
   }
   
      // Try to convert supplied address to an integer
   int32_t addr32;
   getNumber (argv[0], &addr32);
   
      // Check for invalid values (getNumber may complain about this as well)
   if (addr32 < 0 || addr32 > MCP24AA_MAXADDR)
   {
      printf("Address out of range: Value from 0-%d or 0x0000-0x%04X required.%s", MCP24AA_MAXADDR, MCP24AA_MAXADDR, "\n");
      return;
   }
   
      // If Chibi is enabled, make sure we are not overwriting the short or IEEE address in EEPROM
#ifdef CFG_CHIBI
   if ((addr32 >= CFG_CHIBI_EEPROM_IEEEADDR) && (addr32 <= CFG_CHIBI_EEPROM_IEEEADDR + 7))
   {
      printf("Reserved Address: 0x%04X to 0x%04X is reserved for Chibi IEEE address%s", CFG_CHIBI_EEPROM_IEEEADDR, CFG_CHIBI_EEPROM_IEEEADDR + 7, "\n");
      return;
   }
   if ((addr32 >= CFG_CHIBI_EEPROM_SHORTADDR) && (addr32 <= CFG_CHIBI_EEPROM_SHORTADDR + 1))
   {
      printf("Reserved Address: 0x%04X to 0x%04X is reserved for Chibi address%s", CFG_CHIBI_EEPROM_SHORTADDR, CFG_CHIBI_EEPROM_SHORTADDR + 1, "\n");
      return;
   }
#endif
   
      // Address seems to be OK
   addr = (uint16_t)addr32;
   
      // Make sure hexadecimal data is 8-bit or less
   if ((strlen (argv[1]) > 2) && (!strncmp (argv[1], "0x", 2) || !strncmp (argv[1], "0X", 2)) && (strlen (argv[1]) > 4))
   {
      printf("Invalid Data: 8-bit hexadecimal value required (ex. '0xEF').%s", "\n");
      return;
   }
   
      // Try to convert supplied data to an integer
   int32_t val32;
   getNumber (argv[1], &val32);
   
      // Check for invalid values (getNumber may complain about this as well)
   if (val32 < 0 || val32 > 0xFF)
   {
      printf("Invalid Data: Value from 0-255 or 0x00-0xFF required.%s", "\n");
      return;
   }
   
      // Data seems to be OK
   val = (uint8_t)val32;
   
      // Instantiate error message placeholder
   mcp24aaError_e error = MCP24AA_ERROR_OK;
   
      // Write data at supplied address
   error = mcp24aaWriteByte(addr, val);
   if (error)
   {
         // Handle any errors
      switch (error)
      {
         case (MCP24AA_ERROR_I2CINIT):
            printf("Error: Unable to initialised I2C%s", "\n");
            return;
         case (MCP24AA_ERROR_ADDRERR):
            printf("Error: Supplied address is out of range%s", "\n");
            return;
         default:
            break;
      }
   }
   
      // Write successful
   printf("EEPROM: 0x%02X written at address 0x%04X%s", val, addr, "\n");
}

#endif

#ifdef CFG_LM75B

/**************************************************************************/
/*! 
 Gets the current temperature in degrees celsius from the LM75B
 */
/**************************************************************************/
void cmd_lm75b_gettemp(uint8_t argc, char **argv)
{
   int32_t temp = 0;
   
      // Get the current temperature (in 0.125°C units)
   lm75bGetTemperature(&temp);
   
      // Multiply value by 125 for fixed-point math (0.125°C per unit)
   temp *= 125;
   
      // Use modulus operator to display decimal value
   printf("Current Temperature: %d.%d C%s", temp / 1000, temp % 1000, "\n");
}

#endif





