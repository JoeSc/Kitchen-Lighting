##########################################################################
# User configuration and firmware specific object files	
##########################################################################

# The target, flash and ram of the LPC1xxx microprocessor.
# Use for the target the value: LPC11xx, LPC13xx or LPC17xx
TARGET = LPC11xx
FLASH = 32K
SRAM = 8K

# For USB support the LPC1xxx reserves 384 bytes from the sram,
# if you don't want to use the USB features, just use 0 here.
SRAM_USB = 0

VPATH = 
OBJS = main.o




##########################################################################
# GNU GCC compiler prefix and location
##########################################################################
#CROSS_COMPILE = arm-none-eabi-
#CROSS_COMPILE = /opt/local/bin/arm-elf-
CROSS_COMPILE = /usr/local/arm-cs-tools/bin/arm-none-eabi-
AS = $(CROSS_COMPILE)gcc-4.4.1 
CC = $(CROSS_COMPILE)gcc-4.4.1 
LD = $(CROSS_COMPILE)gcc-4.4.1 

#CROSS_COMPILE = /opt/local/bin/arm-elf-
#AS = $(CROSS_COMPILE)gcc-4.5.0 
#CC = $(CROSS_COMPILE)gcc-4.5.0 
#LD = $(CROSS_COMPILE)gcc-4.5.0

SIZE = $(CROSS_COMPILE)size
OBJCOPY = $(CROSS_COMPILE)objcopy
OBJDUMP = $(CROSS_COMPILE)objdump
OUTFILE = firmware

##########################################################################
# Startup files
##########################################################################

LD_PATH = lpc1xxx
LD_SCRIPT = $(LD_PATH)/linkscript.ld
LD_TEMP = $(LD_PATH)/memory.ld

ifeq (LPC11xx,$(TARGET))
  CORTEX_TYPE=m0
else
  CORTEX_TYPE=m3
endif

CPU_TYPE = cortex-$(CORTEX_TYPE)
VPATH += lpc1xxx
OBJS += $(TARGET)_handlers.o LPC1xxx_startup.o cpu_init.o uart.o cmd.o commands.o



##########################################################################
# LPC21ISP program options
##########################################################################
# Settings and variables:
#LPC21ISP = lpc21isp
LPC21ISP = /usr/local/bin/lpc21isp
LPC21ISP_PORT = /dev/tty.usbserial*
LPC21ISP_BAUD = 115200 #38400
LPC21ISP_XTAL = 12000
LPC21ISP_FLASHFILE = $(OUTFILE).hex
# verbose output:
#LPC21ISP_DEBUG = -debug
# enter bootloader via RS232 DTR/RTS (only if hardware supports this
# feature - see Philips AppNote):
LPC21ISP_CONTROL = -control -RstDone
LPC21ISP_OPTIONS = -verify -term #-termonly

OPTIMIZE = -Os

##########################################################################
# Compiler settings, parameters and flags
##########################################################################
CDEFS = -D'BAUDRATE=$(LPC21ISP_BAUD)'
ADEFS = -D'BAUDRATE=$(LPC21ISP_BAUD)'


CFLAGS  = -c $(OPTIMIZE) $(CDEFS) -I. -Wall -mthumb  -ffunction-sections -fdata-sections -fmessage-length=0 -mcpu=$(CPU_TYPE) -DTARGET=$(TARGET) -fno-builtin
ASFLAGS = -c $(OPTIMIZE) $(ADEFS) -I. -Wall -mthumb  -ffunction-sections -fdata-sections -fmessage-length=0 -mcpu=$(CPU_TYPE) -D__ASSEMBLY__ -x assembler-with-cpp
LDFLAGS = -nostartfiles -mcpu=$(CPU_TYPE) -mthumb -Wl,--gc-sections
OCFLAGS = --strip-unneeded

all: firmware
	-@echo "number 2"


%.o : %.c
	$(CC)  $(CFLAGS) -o $@ $<

%.o : %.s
	$(AS) $(ASFLAGS) -o $@ $<


firmware: $(OBJS) $(SYS_OBJS)	
	-@echo "MEMORY {"\
           "  flash(rx): ORIGIN = 0x00000000, LENGTH = $(FLASH)"\
           "  sram(rwx): ORIGIN = 0x10000000+$(SRAM_USB), LENGTH = $(SRAM)-$(SRAM_USB) }"\
	       "INCLUDE $(LD_SCRIPT)" > $(LD_TEMP)
	$(LD) $(LDFLAGS) -T $(LD_TEMP) -o $(OUTFILE).elf $(OBJS)
	-@echo "HI"
	$(SIZE) $(OUTFILE).elf
	-@echo ""
	$(OBJCOPY) $(OCFLAGS) -O binary $(OUTFILE).elf $(OUTFILE).bin
	$(OBJCOPY) $(OCFLAGS) -O ihex $(OUTFILE).elf $(OUTFILE).hex


flash:
		-killall lpc21isp 
		echo '$(LPC21ISP) $(LPC21ISP_OPTIONS) $(LPC21ISP_CONTROL) $(LPC21ISP_DEBUG) "$(CURDIR)/$(LPC21ISP_FLASHFILE)" $(LPC21ISP_PORT) $(LPC21ISP_BAUD) $(LPC21ISP_XTAL)' > debugcommand.txt
		chmod +x debugcommand.txt
		open -a terminal.app debugcommand.txt
		
		#open -a terminal.app $(LPC21ISP) $(LPC21ISP_OPTIONS) $(LPC21ISP_CONTROL) $(LPC21ISP_DEBUG) $(LPC21ISP_FLASHFILE) $(LPC21ISP_PORT) $(LPC21ISP_BAUD) $(LPC21ISP_XTAL)

program:all flash


clean:
	rm -f $(OBJS) $(LD_TEMP) $(OUTFILE).elf $(OUTFILE).bin $(OUTFILE).hex
