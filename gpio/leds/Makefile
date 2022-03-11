LIB_ROOT := ./../../libgd32v

include $(LIB_ROOT)/defs.mk

# Source files.
AS_SRC  = 
C_SRC   = leds.c

LIB := $(LIB_ROOT)/libgd32v.a

# Default rule to build the whole project.
.PHONY: all
all: leds.bin

# Rule to build assembly files.
%.o: %.S
	$(CC) -x assembler-with-cpp $(ASFLAGS) $(INCLUDE) $< -o $@

# Rule to compile C files.
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE) $< -o $@

$(LIB):
	@make -C $(LIB_ROOT)

# Rule to create an ELF file from the compiled object files.
%.elf: $(OBJS) $(LIB)
	$(CC) $^ $(LFLAGS) -o $@

# Rule to create assembly list file from an ELF file.
%.list: %.elf
	$(OD) -D  $< > $@

# Rule to create a raw binary file from an ELF file.
%.bin: %.list
	$(OCP) -S -O binary $*.elf $@

# Rule to clear out generated build files.
.PHONY: clean
clean:
	rm -f $(OBJS) *.elf *.list *.bin
	@make -C $(LIB_ROOT) clean

# Stop make from deleting these suffix'd files.
.PRECIOUS: %.list %.elf %.o
