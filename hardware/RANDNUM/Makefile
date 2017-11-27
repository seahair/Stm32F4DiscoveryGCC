#LED/Makefile

include ../../Makefile.common
CC = arm-none-eabi-gcc
AR = arm-none-eabi-ar

BUILD_DIR = ../build

SRCS = $(wildcard *.c)
OBJS = $(patsubst %.c, $(BUILD_DIR)/%.o, $(SRCS))
DEPS = $(patsubst %.o, %.d, $(OBJS))

-include $(DEPS)

all : $(OBJS)

$(BUILD_DIR)/%.o: %.c
	@echo make $(OBJS)
	$(CC) -MMD $(CFLAGS) -c -g -o  $@ $<


$(OBJS): | $(BUILD_DIR)

.PHONY: $(BUILD_DIR)
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)


