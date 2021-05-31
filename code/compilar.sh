#!/bin/bash
nasm -f elf32 enmascarar.asm -o enmascarar.o
gcc -m32 -o main enmascarar.o main.c

