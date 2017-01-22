#!/usr/bin/env bash

# -q (Quite) to skip report file CalcGrammar.out
# -l to skip #line directives in generated code
# -s to print brief parser statistics
lemon -q -s -l CalcGrammar.lemon
rm -f CalcGrammar.cpp
mv CalcGrammar.c CalcGrammar.cpp
