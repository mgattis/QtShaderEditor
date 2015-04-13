#-------------------------------------------------
#
# Project created by QtCreator 2015-02-03T10:03:54
#
#-------------------------------------------------

include( QtSE.pro )

QT       += testlib

TARGET = QtSE_frontend

MOC_DIR = .moc_frontend
OBJECTS_DIR = .obj_frontend
UI_DIR = .ui_frontend

#LIBS += -lglut

# I reject your reality and substitute my own
SOURCES -= main.cpp

SOURCES += main_frontend.cpp
HEADERS  += main_frontend.h
