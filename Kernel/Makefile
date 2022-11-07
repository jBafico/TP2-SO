include Makefile.inc

KERNEL=kernel.bin
KERNEL_ELF=$(KERNEL:.bin=.elf)
SOURCES = $(wildcard [^_]*.c)
SOURCES_FOLDERS = $(wildcard */*.c)
SOURCES_ASM = $(wildcard */*.asm)

OBJECTS = $(SOURCES:.c=.o)
OBJECTS_FOLDERS = $(SOURCES_FOLDERS:.c=.o)
OBJECTS_ASM = $(SOURCES_ASM:.asm=.o)

MM=-D NORMAL

LOADERSRC = loader.asm

LOADEROBJECT = $(LOADERSRC:.asm=.o)
STATICLIBS=

all: $(KERNEL)
gdb: all $(KERNEL_ELF)

$(KERNEL): $(LOADEROBJECT) $(STATICLIBS) $(OBJECTS) $(OBJECTS_ASM) $(OBJECTS_FOLDERS)
	$(LD) $(LDFLAGS) -T kernel.ld -o $@ $^

$(KERNEL_ELF): $(LOADEROBJECT) $(STATICLIBS) $(OBJECTS) $(OBJECTS_ASM) $(OBJECTS_FOLDERS)
	$(LD) $(LDFLAGS) -T kernel.ld --oformat=elf64-x86-64 -o $@ $^
	

%.o: %.c
	$(GCC) $(GCCFLAGS) -I ./include $(MM) -c $< -o $@

%.o : %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

$(LOADEROBJECT):
	$(ASM) $(ASMFLAGS) $(LOADERSRC) -o $(LOADEROBJECT)

clean:
	rm -rf *.o */*.o *.bin

.PHONY: all clean
