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
#TEST += SRAM


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

#END
$(OBJS): | $(BUILD_DIR)

.PHONY: clean
clean:
	-rm -fr $(BUILD_DIR)
	-rm -f *.a

