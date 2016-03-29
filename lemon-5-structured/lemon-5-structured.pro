TEMPLATE = app

CONFIG += c++11

HEADERS += \
    BatchGrammar.h \
    BatchLexer.h \
    BatchParser.h \
    InterpreterContext.h \
    StringPool.h \
    Token.h \
    BatchAST.h \
    BatchParser_private.h

SOURCES += \
    BatchGrammar.cpp \
    BatchLexer.cpp \
    BatchParser.cpp \
    InterpreterContext.cpp \
    main.cpp \
    StringPool.cpp \
    BatchAST.cpp
