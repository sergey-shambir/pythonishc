#!/usr/bin/env bash

# -q (Quite) to skip report file CalcGrammar.out
# -l to skip #line directives in generated code
# -s to print brief parser statistics
lemon -q -s -l BatchGrammar.lemon
rm -f BatchGrammar.cpp
mv BatchGrammar.c BatchGrammar.cpp
