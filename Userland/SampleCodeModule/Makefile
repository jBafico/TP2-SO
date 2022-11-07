include ../Makefile.inc

MODULE=0000-sampleCodeModule.bin
MODULE_ELF=$(MODULE:.bin=.elf)
SOURCES=$(wildcard [^_]*.c)
SOURCES_FOLDERS = $(wildcard */*.c)
SOURCES_ASM=$(wildcard asm/*.asm)

OBJECTS = $(SOURCES:.c=.o)
OBJECTS_FOLDERS = $(SOURCES_FOLDERS:.c=.o)
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)

all: $(MODULE)
gdb: all $(MODULE_ELF)

$(MODULE): $(OBJECTS) $(OBJECTS_ASM) $(OBJECTS_FOLDERS)
	$(LD) $(LDFLAGS) -T sampleCodeModule.ld -o ../$(MODULE) $^
	
$(MODULE_ELF): $(OBJECTS) $(OBJECTS_ASM) $(OBJECTS_FOLDERS)
	$(LD) $(LDFLAGS) -T sampleCodeModule.ld --oformat=elf64-x86-64 -o ../$(MODULE_ELF) $^

%.o: %.c
	$(GCC) $(GCCFLAGS) -I./include -c $< -o $@

%.o : %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

clean:
	rm -rf */*.o *.o
	rm -rf obj

.PHONY: all clean print
