# LettuceOS - v1.0

Based on x64BareBones, a basic setup to develop operating systems for the Intel 64 bits architecture.

This project is based on a UNIX based operating system where we implement many key OS features such as:
- Multiprocess Scheduling
- Memory Management with 2 different managers: Buddy and K&R
- Interruption Handling
- Keyboard Driver
- Interprocess Communication via Pipes

## Environment setup:

### 1 - Install the following packages before building the Toolchain and Kernel:

    user@linux:$ nasm qemu gcc make

### 2 - Build the Toolchain

Execute the following commands on the x64BareBones project directory:

    user@linux:$ cd Toolchain
    user@linux:$ make all

### 3 - Build the Kernel

From the project directory run one of the following:

If you want to use the default (K&R) Memory Manager:

    user@linux:$ make all

If you want to use the Buddy Memory Manager:

    user@linux:$ make MM=BUDDY all

### 4 - Run the OS

From the project directory run:

    user@linux:$ ./run.sh