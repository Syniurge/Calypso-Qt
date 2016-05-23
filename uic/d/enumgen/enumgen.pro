CONFIG += c++11

QT += core
QT -= gui

TARGET = enumgen
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += enumgen.cpp

INCLUDEPATH += /usr/lib/llvm-3.6/include
LIBS += -L/usr/lib/llvm-3.6/lib -lclang
