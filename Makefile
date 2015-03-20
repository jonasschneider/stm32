#Makefile for STM32F4 targets
#Adapted from https://github.com/snowcap-electronics/stm32-examples/blob/master/Makefile

TARGET=main

CC=arm-none-eabi-gcc
LD=arm-none-eabi-ld
AR=arm-none-eabi-ar
AS=arm-none-eabi-as
CP=arm-none-eabi-objcopy
OD=arm-none-eabi-objdump
SI=./sign.py

BIN=$(TARGET).bin
SIGNED=$(BIN).signed
EXECUTABLE=$(TARGET).elf

HAL=pkg/STM32Cube_FW_F4_V1.5.0/Drivers/STM32F4xx_HAL_Driver

DEVICE=STM32F407xx

OPENOCD_HOST=localhost
OPENOCD_PORT=4444

FLASH_BASE=0x08000000
FLASH_OFFSET=0
FLASH_SIGNOFFSET=0
#FLASH_OFFSET=0x4000
#FLASH_SIGNOFFSET=0x200

FLASH_PGM=$(shell printf 0x%x $$(($(FLASH_BASE)+$(FLASH_OFFSET))))

# And the vector table for the chip matching our target
# and the above evaluation board:
STARTUP = startup_stm32f407xx.s

# We need to specify a linker script for the linker
LDSCRIPT = STM32F407VG_FLASH.ld
LDSCRIPT_IN = $(LDSCRIPT).in
LDFLAGS  = -T$(LDSCRIPT),--gc-sections -lm

SRC = $(STARTUP) \
      $(TARGET).c \
      stm32f4xx_it.c \
      system_stm32f4xx.c \
      periph/clock.c \
      $(HAL)/Src/stm32f4xx_hal_rcc.c \
      $(HAL)/Src/stm32f4xx_hal_gpio.c \
      VirtualWire.c \

# all the packaged includes
INC = -I./ \
	-I$(HAL)/Inc \
	-Ipkg/STM32Cube_FW_F4_V1.5.0/Drivers/CMSIS/Device/ST/STM32F4xx/Include \
	-Ipkg/STM32Cube_FW_F4_V1.5.0/Drivers/CMSIS/Include \
	-Ipkg/STM32Cube_FW_F4_V1.5.0/Drivers/BSP/STM32F4-Discovery \

# HAL objects
SRC += $(HAL)/Src/stm32f4xx_hal.c \
       $(HAL)/Src/stm32f4xx_hal_cortex.c

# BSP objects
SRC += pkg/STM32Cube_FW_F4_V1.5.0/Drivers/BSP/STM32F4-Discovery/stm32f4_discovery.c

#  C source files
CFILES = $(filter %.c, $(SRC))
#  Assembly source files
ASMFILES = $(filter %.s, $(SRC))

# Object files
COBJ = $(CFILES:.c=.o)
SOBJ = $(ASMFILES:.s=.o)
OBJ  = $(COBJ) $(SOBJ)


# Compile thumb for Cortex M4 with debug info
MCU     = cortex-m4
MCFLAGS =-mcpu=$(MCU) -mthumb -mlittle-endian -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb-interwork
OPTIMIZE=-O3 -fdata-sections -ffunction-sections -fsingle-precision-constant
DEBUG   =-g3
CFLAGS  =$(MCFLAGS) $(DEBUG)  $(OPTIMIZE) -MP -MMD

ASFLAGS = $(MCFLAGS) $(DEBUG)

# Defines to be passed to the compiler
DEFINES = -D$(DEVICE) \
          -DVECT_TAB_OFFSET=$(FLASH_OFFSET)+$(FLASH_SIGNOFFSET) \
          -include stm32f4xx_hal_conf.h


all: $(SIGNED)

$(LDSCRIPT): $(LDSCRIPT_IN)
	sed 's/FLASH_BASE/$(FLASH_BASE)/g;s/FLASH_OFFSET/$(FLASH_OFFSET)/g;s/FLASH_SIGNOFFSET/$(FLASH_SIGNOFFSET)/g' < $< >$@

$(SIGNED): $(BIN)
	$(SI) $^ $@ $(FLASH_SIGNOFFSET)

$(BIN): $(EXECUTABLE)
	$(CP) -O binary $^ $@

$(EXECUTABLE): $(OBJ) $(LDSCRIPT)
	$(CC) $(CFLAGS) $(sort $(OBJ)) -Wl,$(LDFLAGS) -o $@

$(COBJ): %.o: %.c
	$(CC) -c $(DEFINES) $(INC) $(CFLAGS) $< -o $@

$(SOBJ): %.o: %.s
	$(AS) -c $(ASFLAGS) $< -o $@

clean:
	rm -f $(OBJ) $(BIN) $(SIGNED) $(EXECUTABLE) $(COBJ:.o=.d) $(LDSCRIPT)

program: $(SIGNED)
	./stm32_flash.py $(realpath $<) $(FLASH_PGM) $(OPENOCD_HOST) $(OPENOCD_PORT)

dfu : $(SIGNED)
	dfu-util -s $(FLASH_PGM) -D $^  --intf 0 --alt 0

-include  $(COBJ:.o=.d)

.PHONY: clean program dfu
