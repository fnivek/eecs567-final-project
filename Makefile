############################################################
# Project
############################################################
PROJECT = eecs567-final-project


############################################################
# Compiler Options
############################################################
OPTIMIZATION = s
STANDARD     = c11
ARCH         = -DSTM32F4 -march=armv7e-m -mthumb -mcpu=cortex-m4
FP           = -mfloat-abi=hard -mfpu=fpv4-sp-d16


############################################################
# Linker Options
############################################################
LDSCRIPT     = stm32f4-discovery.ld

############################################################
# Directories
############################################################
BUILD_DIR     = ./build
OPENCM3_DIR   = ./libopencm3
INCLUDE_DIR   = ./include ./libopencm3/include
LIBRARIES_DIR = ./lib 
SOURCE_DIR    = ./src
LDSCRIPT_DIR  = ./ld


############################################################
# Tools
############################################################
CC = arm-none-eabi-gcc
AR = arm-none-eabi-ar
LD = arm-none-eabi-gcc
GDB = arm-none-eabi-gdb
OBJCOPY = arm-none-eabi-objcopy
OBJDUMP = arm-none-eabi-objdump
SIZE = arm-none-eabi-size
REMOVE  = rm -rfv
STFLASH = st-flash


############################################################
# Source Files
############################################################
ALL_C_FILES = $(shell find $(SOURCE_DIR) -type f -name '*.c')
MAIN_C_FILES = $(filter-out $(SOURCE_DIR)/unit_test.c, $(ALL_C_FILES))
UNIT_TEST_C_FILES = $(filter-out $(SOURCE_DIR)/main.c, $(ALL_C_FILES))
S_FILES = $(shell find $(SOURCE_DIR) -type f -name '*.S')
O_FILES = $(MAIN_C_FILES:.c=.o) $(S_FILES:.S=.o)
UNIT_TEST_O_FILES = $(UNIT_TEST_C_FILES:.c=.o) $(S_FILES:.S=.o)


############################################################
# Tool Flags
############################################################
CC_FLAGS  = -g -Os
CC_FLAGS += -std=$(STANDARD) $(ARCH) $(FP)
CC_FLAGS += -ffunction-sections -fdata-sections -Wall -Wstrict-prototypes
CC_FLAGS += $(patsubst %,-I%,$(INCLUDE_DIR)) -I.

LD_FLAGS  = $(ARCH) $(FP)
LD_FLAGS += -L$(OPENCM3_DIR)/lib -L$(LDSCRIPT_DIR) -T$(LDSCRIPT) -lopencm3_stm32f4
LD_FLAGS += --static -nostartfiles
LD_FLAGS += -Wl,--start-group -lc -lgcc -lnosys -Wl,--end-group


############################################################
# Targets: Actions
############################################################
.SUFFIXES: .c .eep .h .hex .o .elf .s .S
.PHONY: all bin elf flash clean unit_test libopencm3 build_dir

all: build_dir elf

build_dir:
	@mkdir -p build

elf: build_dir $(BUILD_DIR)/$(PROJECT).elf

bin: build_dir $(BUILD_DIR)/$(PROJECT).bin

unit_test: build_dir $(BUILD_DIR)/$(PROJECT)_unit_test.bin

flash: build_dir $(BUILD_DIR)/$(PROJECT).bin
	$(STFLASH) write $< 0x08000000

flash_unit_test: build_dir $(BUILD_DIR)/$(PROJECT)_unit_test.bin
	$(STFLASH) write $< 0x08000000

erase:
	$(STFLASH) erase

clean:
	$(REMOVE) src/*.o src/**/*.o build/*.elf build/*.hex build/*.bin


############################################################
# Targets: Output
############################################################
$(BUILD_DIR)/$(PROJECT).elf: libopencm3 $(O_FILES)
	$(LD) -o $@ $(O_FILES) $(LD_FLAGS)

$(BUILD_DIR)/$(PROJECT)_unit_test.elf: libopencm3 $(UNIT_TEST_O_FILES)
	$(LD) -o $@ $(UNIT_TEST_O_FILES) $(LD_FLAGS)

$(BUILD_DIR)/$(PROJECT).bin: $(BUILD_DIR)/$(PROJECT).elf
	$(OBJCOPY) -Obinary $< $@

$(BUILD_DIR)/$(PROJECT)_unit_test.bin: $(BUILD_DIR)/$(PROJECT)_unit_test.elf
	$(OBJCOPY) -Obinary $< $@

%.o: %.c
	$(CC) $(CC_FLAGS) -c $< -o $@

%.o: %.S
	$(CC) $(AS_FLAGS) -c $< -o $@

libopencm3:
	git submodule update
	make -C libopencm3
