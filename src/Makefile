#src Makefile

include ../Makefile.common

VPATH = ../hardware/LED

OBJS+=startup.o
#OBJS+=startup_stm32f40_41xxx.o
OBJS+=main.o
OBJS+=stm32f4xx_it.o
OBJS+=system_stm32f4xx.o
OBJS+=hardwareinit.o
OBJS+=delay.o
OBJS+=sys.o

all: src
src: app.a

app.a: $(OBJS) # startup.o
	$(AR) cr app.a $(OBJS)

.PHONY: src clean tshow
clean:
	rm -f app.a $(OBJS)

tshow:
	@echo "#############################################################"
	@echo "############ optimize settings: $(InfoTextLib),$(InfoTestSrc)"
	@echo "#############################################################"
