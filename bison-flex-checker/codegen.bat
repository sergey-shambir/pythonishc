win_flex -L -olexer.gen.c lexer.l
win_bison --no-lines --defines=grammar.gen.h --output=grammar.gen.c --graph=grammar.dot grammar.y
