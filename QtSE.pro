#-------------------------------------------------
#
# Project created by QtCreator 2015-02-03T10:03:54
#
#-------------------------------------------------

QT       += core gui widgets opengl

TARGET = QtSE
TEMPLATE = app

MOC_DIR = .moc
OBJECTS_DIR = .obj
UI_DIR = .ui

CONFIG += debug_and_release
LIBS += -lGLEW -lGLU -lGL
QMAKE_CXXFLAGS += -msse -msse2 -mfpmath=sse -fpermissive -ggdb

# Warnings
QMAKE_CXXFLAGS += -Wno-unused-variable -Wno-unused-parameter -Wno-sign-compare

SOURCES += main.cpp\
        QtSE.cpp \
    CResourceManager.cpp \
    CPath.cpp \
    qtil.cpp \
    VGLView.cpp \
    util.cpp \
    CJsonTemplate.cpp \
    VJsonForm.cpp \
	VDraggableTabBar.cpp \
	CProjectProject.cpp \
	IProjectObject.cpp \
	CProjectStage.cpp \
    VTabWidgetArea.cpp

HEADERS  += QtSE.h \
    CResourceManager.h \
    CPath.h \
    qtil.h \
    IFileClass.h \
    VGLView.h \
    util.h \
    CJsonTemplate.h \
    VJsonForm.h \
	VDraggableTabBar.h \
	CProjectProject.h \
	IProjectProject.h \
	IProjectObject.h \
	IProjectStage.h \
	CProjectStage.h \
    VTabWidgetArea.h

DISTFILES += \
    .gitignore
