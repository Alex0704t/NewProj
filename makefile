#Target file name (without extension)
TARGET=NewProj
#STM lib directory
STM_CORE_DIR=./CMSIS/Include
STM_DIR=./Device

#User directory
DIRS=. ./User
#Headers & source directory
INC_DIRS=$(STM_CORE_DIR) $(STM_DIR) $(DIRS)
SRC_DIRS=$(STM_DIR) $(DIRS)
#mp3 section
#MP3_DIRS = \
./Audio \
./Audio/audio \
./Audio/helix_mp3 \
./Audio/helix_mp3/pub \
./Audio/helix_mp3/real \
#INC_DIRS+=$(MP3_DIRS)
#SRC_DIRS+=$(MP3_DIRS)
#Startup
STARTUP=./Device/startup/startup_stm32f4xx.c
#STARTUP=./Device/startup/startup_stm32f407xx.s
#Define commands
CC=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy
STLINK=st-link_cli
LDSCRIPT=standalone.ld
SIZE=arm-none-eabi-size
REMOVE=rm -f
LINKER_FLAGS=-nostartfiles -Xlinker -o$(TARGET).elf -Xlinker -M -Xlinker \
-Map=$(TARGET).map --specs=nano.specs -lgcc -lm -u _printf_float 
DEBUG=-g2
#OPTIM=-O2
OPTIM=-O3
STANDART=-std=gnu11
M_CPU=cortex-m4
M_FPU=fpv4-sp-d16
M_FLOAT=hard
#M_FLOAT=softfp
CFLAGS=$(DEBUG) $(STANDART) \
$(foreach dir, $(INC_DIRS), -I$(dir)) \
-D STM32F407xx \
-D inline= -mthumb -mcpu=$(M_CPU) -mfpu=$(M_FPU) -mfloat-abi=$(M_FLOAT) \
$(OPTIM) -T $(LDSCRIPT) -ggdb 
#Source & object files
SOURCE=$(wildcard $(foreach dir, $(SRC_DIRS), $(wildcard $(dir)/*.c)))
OBJS=$(SOURCE:.c=.o)
#Targets
all: begin $(TARGET).bin size finished end
$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) $(TARGET).elf -O ihex $(TARGET).hex
$(TARGET).elf: $(OBJS) startup.o
	$(CC) $(CFLAGS) $(OBJS) startup.o $(LINKER_FLAGS)
$(OBJS): %.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@
startup.o: $(STARTUP)
	$(CC) -c $(CFLAGS) -O1 $(STARTUP) -o startup.o
size:
	$(SIZE) --format=berkeley $(TARGET).elf
#Define Messages
#English
MSG_ERRORS_NONE = Errors: none
MSG_BEGIN = Begin ---------
MSG_END = End  --------
MSG_CLEANING = Cleaning the project ---
begin:
	@echo
	@echo $(MSG_BEGIN)
finished:
	@echo $(MSG_ERRORS_NONE)
end:
	@echo $(MSG_END)
	@echo
clean: begin clean_list end
clean_list :
	@echo $(MSG_CLEANING)
	$(REMOVE) $(OBJS)
	$(REMOVE) $(TARGET).hex
	$(REMOVE) $(TARGET).bin
	$(REMOVE) $(TARGET).elf
	$(REMOVE) $(TARGET).map
	$(REMOVE) startup.o
rebuild: begin clean_list $(TARGET).bin size finished end	
download: begin program end
rebuild_load: begin clean_list $(TARGET).bin size finished program end
program: $(TARGET).bin
	$(STLINK) -p $(TARGET).hex -run
erase: begin full_erase end	
full_erase:
	$(STLINK) -me	
print: 
	@echo SOURCE:
	@echo $(SOURCE)
	@echo OBJS:
	@echo $(wildcard $(foreach dir, $(SRC_DIRS), $(wildcard $(dir)/*.o)))
	@echo HEADERS:
	@echo $(wildcard $(foreach dir, $(INC_DIRS), $(wildcard $(dir)/*.h))) 
	@echo INC_DIRS:
	@echo $(INC_DIRS)
	@echo SRC_DIRS:
	@echo $(SRC_DIRS)
	
	