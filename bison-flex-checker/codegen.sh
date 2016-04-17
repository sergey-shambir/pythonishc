#!/usr/bin/env bash

flex -L -olexer.gen.c lexer.l
bison --no-lines --defines=grammar.gen.h --output=grammar.gen.c --graph=grammar.dot grammar.y
