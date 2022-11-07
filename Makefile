MM = NORMAL

all:  bootloader kernel userland image



gdb: all kernel_elf userland_elf


kernel_elf:
	cd Kernel; make gdb

userland_elf:
	cd Userland; make gdb

bootloader:
	cd Bootloader; make all

kernel:
	cd Kernel; make all MM=-D$(MM)

userland:
	cd Userland; make all

image: kernel bootloader userland
	cd Image; make all


clean:
	cd Bootloader; make clean
	cd Image; make clean
	cd Kernel; make clean
	cd Userland; make clean
	rm -rf check
	rm -rf .config

check:
	mkdir -p check
	cppcheck --quiet --enable=all --force --inconclusive . 2> ./check/cppout.txt

	pvs-studio-analyzer trace -- make
	pvs-studio-analyzer analyze
	plog-converter -a '64:1,2,3;GA:1,2,3;OP:1,2,3' -t tasklist -o ./check/report.tasks ./PVS-Studio.log
	
	rm PVS-Studio.log
	mv strace_out check

.PHONY: bootloader image collections kernel userland all clean
