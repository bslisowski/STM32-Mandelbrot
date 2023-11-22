CFLAGS  ?=  -O2 -W -Wall -Wextra -Werror -Wundef -Wshadow -Wdouble-promotion \
            -Wformat-truncation -fno-common -Wconversion \
            -g3 -Os -ffunction-sections -fdata-sections \
            -I. -Iinclude -Icmsis_core/CMSIS/Core/Include -Icmsis_f3/Include \
            -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 $(EXTRA_CFLAGS)
LDFLAGS ?= -Tlink.ld -nostartfiles -nostdlib --specs nano.specs -lm -lc -lrdimon -u _printf_float -lgcc -Wl,--gc-sections -Wl,-Map=$@.map
SOURCES = main.c syscalls.c sysinit.c hal/gpio.c hal/usart.c hal/pwm.c hal/spi.c hal/st7789v.c hal/i2c.c fractals.c
SOURCES += platform.c VL53L4CD_api.c
SOURCES += cmsis_f3/Source/Templates/gcc/startup_stm32f303x8.s

ifeq ($(OS),Windows_NT)
  RM = cmd /C del /Q /F
else
  RM = rm -rf
endif

build: firmware.bin

firmware.elf: cmsis_core cmsis_f3 $(SOURCES) 
	arm-none-eabi-gcc $(SOURCES) $(CFLAGS) $(CFLAGS_EXTRA) $(LDFLAGS) -o $@

firmware.bin: firmware.elf
	arm-none-eabi-objcopy -O binary $< $@

flash: firmware.bin
	st-flash --connect-under-reset --reset --flash=64k write $< 0x08000000

cmsis_core:
	git clone --depth 1 -b 5.9.0 https://github.com/ARM-software/CMSIS_5 $@

cmsis_f3:
	git clone --depth 1 -b v2.3.7 https://github.com/STMicroelectronics/cmsis_device_f3 $@

clean:
	$(RM) firmware.* 

dump:
	arm-none-eabi-objdump -D firmware.elf > firmware.asm

