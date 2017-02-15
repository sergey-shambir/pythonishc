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
    interpreter/Interpreter.hpp \
    interpreter/Token.hpp \
    interpreter/RandomSequence.hpp

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
    interpreter/Interpreter.cpp \
    interpreter/RandomSequence.cpp

























































