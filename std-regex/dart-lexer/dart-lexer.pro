#-------------------------------------------------
#
# Project created by QtCreator 2012-04-22T00:04:06
#
#-------------------------------------------------

QT       -= core gui

unix: {
  LIBS += -lboost_regex
}

TARGET = dart-lexer
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    lexer.cpp

HEADERS += \
    lexer.h
