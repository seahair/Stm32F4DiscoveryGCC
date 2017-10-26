#src/Makefile

CC = arm-none-eabi-gcc
AR = arm-none-eabi-ar

LIB_A = libhardware.a
BUILD_DIR = build
OBJS = $(BUILD_DIR)/*.o

TEST = LED
TEST += BEEP 
#TEST += KEY
TEST += USART
#TEST += EXTI
#TEST += TIME
#TEST += PWM
#TEST += CAPTURE
#TEST += PAD
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

TIME:
	make -C TIME

PWM:
	make -C PWM

CAPTURE:
	make -C CAPTURE

PAD:
	make -C PAD

SRAM:
	make -C SRAM

#END
$(OBJS): | $(BUILD_DIR)

.PHONY: clean
clean:
	-rm -fr $(BUILD_DIR)
	-rm -f *.a

