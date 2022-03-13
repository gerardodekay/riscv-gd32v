LIB_ROOT := .

include $(LIB_ROOT)/defs.mk

# Source files.
AS_SRC := $(wildcard ./*.[S])
AS_SRC += $(wildcard ./src/*.[S])
C_SRC := $(wildcard ./src/*.[c])

LIB := libgd32v.a

# Default rule to build the whole project.
.PHONY: all
all: $(LIB)

# Rule to build assembly files.
%.o: %.S
	$(CC) -x assembler-with-cpp $(ASFLAGS) $(INCLUDE) $< -o $@

# Rule to compile C files.
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE) $< -o $@

$(LIB): $(OBJS)
	$(AR) crf $@ $(OBJS)
	$(OD) -D $@ > $@.list

# Rule to clear out generated build files.
.PHONY: clean
clean:
	rm -f $(OBJS) $(LIB) *.elf *.list *.bin

# Stop make from deleting these suffix'd files.
.PRECIOUS: %.list %.elf %.o
