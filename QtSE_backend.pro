#-------------------------------------------------
#
# Project created by QtCreator 2015-02-03T10:03:54
#
#-------------------------------------------------

include( QtSE.pro )

QT       += testlib

TARGET = QtSE_backend

MOC_DIR = .moc_backend
OBJECTS_DIR = .obj_backend
UI_DIR = .ui_backend

# I reject your reality and substitute my own
SOURCES -= main.cpp

SOURCES += main_backend.cpp
HEADERS  += main_backend.h
