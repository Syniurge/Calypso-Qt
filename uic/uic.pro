# option(host_build)

QT       += core
TARGET = uic

DEFINES += QT_UIC QT_NO_CAST_FROM_ASCII

include(uic.pri)
include(cpp/cpp.pri)
include(d/d.pri)

HEADERS += uic.h

SOURCES += main.cpp \
           uic.cpp

# load(qt_tool)
