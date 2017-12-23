#src/Makefile

CC = arm-none-eabi-gcc
AR = arm-none-eabi-ar

LIB_A = libhardware.a
BUILD_DIR = build
OBJS = $(BUILD_DIR)/*.o

TEST = LED
TEST += BEEP 
TEST += KEY
TEST += USART
TEST += IWDG 
TEST += TIME3
TEST += PWM
TEST += CAPTURE
TEST += TPAD
TEST += LCD 
TEST += RTC
TEST += RANDNUM
TEST += PM
TEST += ADC
TEST += DAC
TEST += DMA
TEST += I2C
TEST += RS485
TEST += CAN
TEST += SPI
TEST += REMOTE 
TEST +=	TOUCH 
TEST +=	FLASH 
TEST +=	SRAM 
TEST +=	MALLOC 


.PHONY: $(TEST)
.PHONY: all
all: $(TEST)
	$(AR) -cvq $(LIB_A) $(OBJS)

#:<<END
LED:
	make -C LED

BEEP:
	make -C BEEP

KEY:
	make -C KEY

USART:
	make -C USART

EXTI:
	make -C EXTI

TIME3:
	make -C TIME3

PWM:
	make -C PWM

CAPTURE:
	make -C CAPTURE

TPAD:
	make -C TPAD

SRAM:
	make -C SRAM

IWDG:
	make -C IWDG 

LCD:
	make -C LCD 

RTC:
	make -C RTC 

RANDNUM:
	make -C RANDNUM 

PM:
	make -C PM 

ADC:
	make -C ADC 

DAC:
	make -C DAC 

DMA:
	make -C DMA 

I2C:
	make -C I2C 

RS485:
	make -C RS485 

CAN:
	make -C CAN 

SPI:
	make -C SPI 

REMOTE:
	make -C REMOTE 

TOUCH:
	make -C TOUCH 

FLASH:
	make -C FLASH 

SRAM:
	make -C SRAM 

MALLOC:
	make -C MALLOC 

#END
$(OBJS): | $(BUILD_DIR)

.PHONY: clean
clean:
	-rm -fr $(BUILD_DIR)
	-rm -f *.a

