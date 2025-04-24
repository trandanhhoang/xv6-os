# Before lab

Watch video  
https://www.youtube.com/watch?v=L6YqHxYHa7A

Read Chapter 1 xv6  
https://pdos.csail.mit.edu/6.828/2021/xv6/book-riscv-rev2.pdf

Example:
https://pdos.csail.mit.edu/6.828/2021/lec/l-overview/

# Set up labs
https://pdos.csail.mit.edu/6.828/2021/labs/util.html

ref: https://pdos.csail.mit.edu/6.828/2021/tools.html1

```bash
brew tap riscv/riscv
brew install riscv-tools
```

The brew formula may not link into /usr/local. You will need to update your shell's rc file (e.g. ~/.bashrc) to add the appropriate directory to $PATH.
PATH=$PATH:/usr/local/opt/riscv-gnu-toolchain/bin

```bash
brew install qemu
```

## Testing your Installation

```bash
$ qemu-system-riscv64 --version
QEMU emulator version 9.2.2
Copyright (c) 2003-2024 Fabrice Bellard and the QEMU Project developers
```

```bash
$ riscv64-unknown-elf-gcc --version
riscv64-unknown-elf-gcc (g04696df09) 14.2.01
Copyright (C) 2024 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE
```

## Try to run in the xv6 directory
```bash
$ make qemu
# ... lots of output ...
init: starting sh
$
```

First error counter

```text
error: infinite recursion detected [-Werror=infinite-recursion] 58 | runcmd(struct cmd *cmd
```

Solve

```text
__attribute__((noreturn)) // add this line
void
runcmd(struct cmd *cmd)
{ ...
```

## Test something
ls 

ctrl-p
xv6 has no ps command, but, if you type Ctrl-p, the kernel will print information about each process. If you try it now, you'll see two lines: one for init, and one for sh.

## How to exit qemu

ctrl-a ,release, then x

# Sleep (ez)

## Guidance

https://pdos.csail.mit.edu/6.828/2021/labs/guidance.html

### Learn C, pointer, endianess

https://pdos.csail.mit.edu/6.828/2019/lec/pointers.c

A few pointer common idioms are in particular worth remembering:

- If int *p = (int*)100, then (int)p + 1 and (int)(p + 1) are different numbers: the first is 101 but the second is 104. When adding an integer to a pointer, as in the second case, the integer is implicitly multiplied by the size of the object the pointer points to.  
- p[i] is defined to be the same as *(p+i), referring to the i'th object in the memory pointed to by p. The above rule for addition helps this definition work when the objects are larger than one byte.
- &p[i] is the same as (p+i), yielding the address of the i'th object in the memory pointed to by p.  

Although most C programs never need to cast between pointers and integers, operating systems frequently do. Whenever you see an addition involving a memory address, ask yourself whether it is an integer addition or pointer addition and make sure the value being added is appropriately multiplied or not.

### Continue check the guidance

Learn man, script

```bash
script hoang.txt
make qemu
```

### (comeback later) Want to see what the assembly is || find out what the instruction is at a particular kernel address

See the file kernel.asm, which the Makefile produces when it compiles the kernel. (The Makefile also produces .asm for all user programs.)

### If the kernel panics (comeback later)

It will print an error message listing the value of the program counter when it crashed; you can search kernel.asm to find out in which function the program counter was when it crashed, or you can run addr2line -e kernel/kernel pc-value (run man addr2line for details). If you want to get backtrace, restart using gdb: run 'make qemu-gdb' in one window, run gdb (or riscv64-linux-gnu-gdb) in another window, set breakpoint in panic ('b panic'), followed by followed by 'c' (continue). When the kernel hits the break point, type 'bt' to get a backtrace.

### If your kernel hangs (comeback later)

(e.g., due to a deadlock) or cannot execute further (e.g., due to a page fault when executing a kernel instruction), you can use gdb to find out where it is hanging. Run run 'make qemu-gdb' in one window, run gdb (riscv64-linux-gnu-gdb) in another window, followed by followed by 'c' (continue). When the kernel appears to hang hit Ctrl-C in the qemu-gdb window and type 'bt' to get a backtrace.

### qemu has a "monitor" (comeback later)

that lets you query the state of the emulated machine. You can get at it by typing control-a c (the "c" is for console). A particularly useful monitor command is info mem to print the page table. You may need to use the cpu command to select which core info mem looks at, or you could start qemu with make CPUS=1 qemu to cause there to be just one core.

## Learn detail GDB

https://pdos.csail.mit.edu/6.828/2019/lec/gdb_slides.pdf


# in one window
make qemu-gdb 
# in another window
```text
$ gdb  
...
The target architecture is set to "riscv:rv64".
warning: No executable has been specified and target does not support
determining executable automatically.  Try using the "file" command.
0x0000000000001000 in ?? ()
$ (gdb) file kernel/kernel
Reading symbols from kernel/kernel...
$ (gdb) info target
Symbols from "/Users/lap14980/gith/xv6-labs-2021/kernel/kernel".
Remote target using gdb-specific protocol:
        `/Users/lap14980/gith/xv6-labs-2021/kernel/kernel', 
        file type elf64-littleriscv.
        Entry point: 0x80000000
        0x0000000080000000 - 0x0000000080008000 is .text
        0x0000000080008000 - 0x0000000080008809 is .rodata
        0x0000000080008810 - 0x0000000080008854 is .data
        0x0000000080009000 - 0x0000000080026240 is .bss
        While running this, GDB does not access memory from...
Local exec file:
        `/Users/lap14980/gith/xv6-labs-2021/kernel/kernel', 
        file type elf64-littleriscv.
        Entry point: 0x80000000
        0x0000000080000000 - 0x0000000080008000 is .text
        0x0000000080008000 - 0x0000000080008809 is .rodata
        0x0000000080008810 - 0x0000000080008854 is .data
        0x0000000080009000 - 0x0000000080026240 is .bss
```

### Set a Breakpoint

```bash
(gdb) break main
Breakpoint 1 at 0x8000033c: file kernel/main.c, line 13.
```

### Continue Execution

```bash
(gdb) c
Continuing.
[Switching to Thread 1.2]

Thread 2 hit Breakpoint 1, main () at kernel/main.c:13
warning: Source file is more recent than executable.
13        if(cpuid() == 0){
```

### Step & print

``` bash
# step through C code
(gdb) step
# assembly instructions
(gdb) stepi
# view variables
(gdb) print p
# see the cal stack
(gdb) backtrace 
# see assembly code
(gdb) disassemble
```


```bash
(gdb) file user/redirect
(gdb) break redirect.c:main
(gdb) break sys_open
(gdb) break sys_exec
```

```bash
(gdb) print p->ofile[1]
```

1. what is p->ofile[1] ?
2. fs.img from mkfs/ ?
3. what is ecall ?

```text
+---------------------------------------------------+
|                Your Laptop (Mac, Apple Silicon)    |
|  +---------------------------------------------+   |
|  | Terminal 1: QEMU (Emulator)                |   |
|  |  | Virtual Disk (fs.img from mkfs/)       |   |
|  |  +--------------------------------------+  |   |
|  |  | Virtual Hardware                    |  |   |
|  |  |  +-------------------------------+  |  |   |
|  |  |  | Processor (RISC-V CPU)       |  |  |   |
|  |  |  |  +----------------------+    |  |  |   |
|  |  |  |  | Instruction Set      |<--+ |  |   |
|  |  |  |  | (RISC-V: ecall)      |    | |  |   |
|  |  |  |  +----------------------+    | |  |   |
|  |  |  +-------------------------------+ |  |   |
|  |  |    ^ Executes                      |  |   |
|  |  +----| Operating System (xv6) ------+  |   |
|  |       | - Kernel (kernel/)            |  |   |
|  |       | - User Programs (user/)       |  |   |
|  |       | - File System (mkfs/)         |  |   |
|  |       +-------------------------------+  |   |
|  +--------------------------------------+   |   |
|  | Terminal 2: GDB (riscv64-unknown-elf-gdb) |   |
|  |  - Connected to QEMU via port 25501     |   |
|  |  - Loaded kernel/kernel symbols         |   |
|  +--------------------------------------+   |   |
|       ^ RISC-V from UC Berkeley             |   |
|       +-------------------------------------+   |
+---------------------------------------------------+
```


### finish sleep
$ ./grade-lab-util sleep


### finish pingpong
$ ./grade-lab-util pingpong




