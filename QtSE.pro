#-------------------------------------------------
#
# Project created by QtCreator 2015-02-03T10:03:54
#
#-------------------------------------------------

QT       += core gui widgets opengl

TARGET = QtSE
TEMPLATE = app

CONFIG += debug_and_release
LIBS += -lGLEW -lGLU -lGL
QMAKE_CXXFLAGS += -msse -msse2 -mfpmath=sse -fpermissive

SOURCES += main.cpp\
        QtSE.cpp \
    CResourceManager.cpp \
    CPath.cpp \
    qtil.cpp \
    VGLView.cpp \
    util.cpp \
    VTabEditor.cpp \
    CJsonTemplate.cpp \
    VJsonForm.cpp \
    VDraggableTabBar.cpp

HEADERS  += QtSE.h \
    CResourceManager.h \
    CPath.h \
    qtil.h \
    IFileClass.h \
    VGLView.h \
    util.h \
    VTabEditor.h \
    CJsonTemplate.h \
    VJsonForm.h \
    VDraggableTabBar.h

DISTFILES += \
    .gitignore
