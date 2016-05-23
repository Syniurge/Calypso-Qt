INCLUDEPATH += $$PWD $$QT_BUILD_TREE/src/tools/uic

DEFINES += QT_UIC_D_GENERATOR

# Input
HEADERS += $$PWD/dextractimages.h \
           $$PWD/dwritedeclaration.h \
           $$PWD/dwriteicondata.h \
           $$PWD/dwriteicondeclaration.h \
           $$PWD/dwriteiconinitialization.h \
           $$PWD/dwriteincludes.h \
           $$PWD/dwriteinitialization.h \
           $$PWD/denumconstants.h

SOURCES += $$PWD/dextractimages.cpp \
           $$PWD/dwritedeclaration.cpp \
           $$PWD/dwriteicondata.cpp \
           $$PWD/dwriteicondeclaration.cpp \
           $$PWD/dwriteiconinitialization.cpp \
           $$PWD/dwriteincludes.cpp \
           $$PWD/dwriteinitialization.cpp \
           $$PWD/denumconstants.cpp
