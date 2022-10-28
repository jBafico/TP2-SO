include Makefile.inc

KERNEL=kernel.bin

SOURCES = $(wildcard *.c)
SOURCES_FOLDERS = $(wildcard */*.c)
SOURCES_ASM = $(wildcard */*.asm)

OBJECTS = $(SOURCES:.c=.o)
OBJECTS_FOLDERS = $(SOURCES_FOLDERS:.c=.o)
OBJECTS_ASM = $(SOURCES_ASM:.asm=.o)

LOADERSRC = loader.asm

LOADEROBJECT = $(LOADERSRC:.asm=.o)
STATICLIBS=

all: $(KERNEL)

$(KERNEL): $(LOADEROBJECT) $(STATICLIBS) $(OBJECTS) $(OBJECTS_ASM) $(OBJECTS_FOLDERS)
	$(LD) $(LDFLAGS) -T kernel.ld -o $@ $^

%.o: %.c
	$(GCC) $(GCCFLAGS) -I ./include -c $< -o $@

%.o : %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

$(LOADEROBJECT):
	$(ASM) $(ASMFLAGS) $(LOADERSRC) -o $(LOADEROBJECT)

clean:
	rm -rf *.o */*.o *.bin

.PHONY: all clean
