#!/usr/bin/env bash

flex $1.l && gcc -o $1.elf lex.yy.c -lfl
rm -f lex.yy.c
