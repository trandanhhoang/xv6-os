## First lab

https://pdos.csail.mit.edu/6.828/2021/labs/util.html

### Set up

ref: https://pdos.csail.mit.edu/6.828/2021/tools.html

brew tap riscv/riscv
brew install riscv-tools

The brew formula may not link into /usr/local. You will need to update your shell's rc file (e.g. ~/.bashrc) to add the appropriate directory to $PATH.
PATH=$PATH:/usr/local/opt/riscv-gnu-toolchain/bin

brew install qemu

### Testing your Installation

```bash
$ qemu-system-riscv64 --version
QEMU emulator version 9.2.2
Copyright (c) 2003-2024 Fabrice Bellard and the QEMU Project developers
```

```bash
$ riscv64-unknown-elf-gcc --version
riscv64-unknown-elf-gcc (g04696df09) 14.2.0
Copyright (C) 2024 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE
```

### in the xv6 directory
$ make qemu
# ... lots of output ...
init: starting sh
$

### first error counter

```text
error: infinite recursion detected [-Werror=infinite-recursion] 58 | runcmd(struct cmd *cmd
```

solve

```text
__attribute__((noreturn)) // add this line
void
runcmd(struct cmd *cmd)
{ ...
```

### test something
ls 
ctrl-p

### how to exit qemu

ctrl-a ,release, then x

