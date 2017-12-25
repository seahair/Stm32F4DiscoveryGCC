#general Makefile

include Makefile.common
LDFLAGS=$(COMMONFLAGS) -fno-exceptions -ffunction-sections -fdata-sections -L$(LIBDIR) -nostartfiles -Wl,--gc-sections,-Tlinker.ld

LDLIBS+=-lm
LDLIBS+=-lhardware -L./hardware/
LDLIBS+=-lstm32

STARTUP=startup.c


		#-Wl,--whole-archive\
		-Wl,--whole-archive\
		-Wl,--no-whole-archive\
			hardware/libhardware.a\

all: libs src hardware
	ctags -R
	$(CC) -o $(PROGRAM).elf $(LDFLAGS)\
			src/app.a\
			$(LDLIBS)\
			src/sys.o
	$(OBJCOPY) -O ihex $(PROGRAM).elf $(PROGRAM).hex
	$(OBJCOPY) -O binary $(PROGRAM).elf $(PROGRAM).bin
#Extract info contained in ELF to readable test-files
	arm-none-eabi-readelf -a $(PROGRAM).elf > $(PROGRAM).info_elf
	arm-none-eabi-size -d -B -t $(PROGRAM).elf > $(PROGRAM).info_size
	arm-none-eabi-objdump -S $(PROGRAM).elf > $(PROGRAM).info_code
	arm-none-eabi-nm -t x -S --numeric-sort -s $(PROGRAM).elf > $(PROGRAM).info_symbol

.PHONY: libs src hardware clean tshow gdb update download

libs:
	$(MAKE) -C libs $@

src:
	$(MAKE) -C src $@

hardware:
	$(MAKE) -C hardware

clean:
	$(MAKE) -C src $@
#	$(MAKE) -C libs $@
	$(MAKE) -C hardware $@
	rm -f $(PROGRAM).elf $(PROGRAM).hex $(PROGRAM).bin $(PROGRAM).info*

tshow:
	@echo "######################################################################################################" 
	@echo "######## optimize settings: $(InfoTextLib), $(InfoTextSrc)" 
	@echo "######################################################################################################"

flash: all
	./do_flash.pl $(TOP)/main.bin

update:
	openocd -f /usr/share/openocd/scripts/interface/jlink.cfg  -f /usr/share/openocd/scripts/target/stm32f4x.cfg -c init -c halt -c "flash write_image erase /home/sea/gitworkspace/Stm32F4DiscoveryGCC/main.hex" # -c reset -c  shutdown

gdb:
	$(GDB) -iex "target remote localhost:3333" -iex 'monitor reset' -iex 'monitor halt' -iex 'load main.elf' -iex 'b main.c:main' -iex 'b main.c:138' main.elf

download:
	openocd -f /usr/share/openocd/scripts/interface/jlink.cfg  -f /usr/share/openocd/scripts/target/stm32f4x.cfg -c init -c halt -c "flash write_image erase /home/sea/gitworkspace/Stm32F4DiscoveryGCC/main.hex"  -c reset -c  shutdown

