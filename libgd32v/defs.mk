# GCC RISC-V toolchain programs.
CC = riscv64-unknown-elf-gcc
OCP = riscv64-unknown-elf-objcopy
OD = riscv64-unknown-elf-objdump
AR = riscv64-unknown-elf-ar

# Assembler flags
ASFLAGS = -O0 -c -Wall -fmessage-length=0 -march=rv32imac -mabi=ilp32 \
	-mcmodel=medlow

# Compiler flags
CFLAGS = -O0 -c -Wall -g -fmessage-length=0 --specs=nosys.specs \
	-march=rv32imac -mabi=ilp32 -mcmodel=medlow -std=gnu99

# Linker flags
LFLAGS = -Wall -Wl,--no-relax -Wl,--gc-sections -nostdlib -nostartfiles -lc \
	-lgcc --specs=nosys.specs -march=rv32imac -mabi=ilp32 -mcmodel=medlow \
	-T$(LIB_ROOT)/gd32vf103xb.ld

# Library directories
INCLUDE = -I$(LIB_ROOT)/device_headers \
	-I$(LIB_ROOT)/include \

# Object files to build.
OBJS = $(AS_SRC:.S=.o)
OBJS += $(C_SRC:.c=.o)
