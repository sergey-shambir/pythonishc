QT -= gui core

unix:LIBS += -lrt

QMAKE_CXXFLAGS += -std=c++0x

OTHER_FILES += \
    readme.txt

HEADERS += \
    duck/IDuck.hpp \
    duck/AbstractDuck.hpp \
    strategy/FlyLikeDuck.hpp \
    strategy/IFly.hpp \
    strategy/IQuack.hpp \
    duck/MallardDuck.hpp \
    strategy/QuackLikeDuck.hpp \
    strategy/IRotate.hpp \
    strategy/Rotate.hpp \
    strategy/FlyInBathroom.hpp \
    duck/ToyDuck.hpp \
    duck/RedheadDuck.hpp \
    strategy/RotateWrongWay.hpp \
    interpreter/SourceStream.hpp \
    interpreter/Scanner.hpp \
    interpreter/Token.hpp \
    interpreter/RandomSequence.hpp \
    interpreter/ast/INode.h \
    interpreter/ast/AST_IVisitor.h \
    interpreter/ast/AST_WhileStmt.h \
    interpreter/ast/AST_Stmt.h \
    interpreter/ast/AST_ActionStmt.h \
    interpreter/ast/AST_StmtGroup.h \
    interpreter/ast/AST_TreeBuilder.h \
    interpreter/error/Emitter.h \
    interpreter/error/IListener.h \
    interpreter/ASTInterpreter.h \
    interpreter/ast/ast.h \
    interpreter/ast/AST_PrettyPrinter.h

SOURCES += \
    duck/AbstractDuck.cpp \
    strategy/FlyLikeDuck.cpp \
    duck/MallardDuck.cpp \
    strategy/QuackLikeDuck.cpp \
    strategy/Rotate.cpp \
    strategy/FlyInBathroom.cpp \
    duck/ToyDuck.cpp \
    duck/RedheadDuck.cpp \
    strategy/RotateWrongWay.cpp \
    main.cpp \
    interpreter/Scanner.cpp \
    interpreter/RandomSequence.cpp \
    interpreter/ast/AST_WhileStmt.cpp \
    interpreter/ast/AST_Stmt.cpp \
    interpreter/ast/AST_ActionStmt.cpp \
    interpreter/ast/AST_StmtGroup.cpp \
    interpreter/ast/AST_TreeBuilder.cpp \
    interpreter/error/Emitter.cpp \
    interpreter/ASTInterpreter.cpp \
    interpreter/ast/AST_PrettyPrinter.cpp




























































































