# Cross toolchain files for muteki-shims

Here are some predefined cross files for compiling muteli-shims.

For adding cross files for a new toolchain (e.g. LLVM), note that currently muteki-shims assumes GNU Assembler syntax and other assemblers are untested. Support for new assembler syntax can be added in `syscall_def/gen_syscall_shim.py`.

## arm-bestartos

For a dedicated toolchain named `arm-bestartos`, should be able to output COFF/PE objects directly.

Can be used to generate DLL replicas.

## arm-mingw32ce

For Max Kellermann's CeGCC fork. Compiles in freestanding mode and no Windows CE-specific code will be included.

Can be used to generate DLL replicas.

## arm-none-eabi

For stock ARM GCC outputting AAELF.

**Cannot** be used to generate DLL replicas.

## arm-none-eabi-devkitarm

For devkitARM's ARM GCC outputting AAELF.

**Cannot** be used to generate DLL replicas.
