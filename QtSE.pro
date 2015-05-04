#-------------------------------------------------
#
# Project created by QtCreator 2015-02-03T10:03:54
#
#-------------------------------------------------

QT       += core gui widgets opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtSE
TEMPLATE = app

MOC_DIR = .moc
OBJECTS_DIR = .obj
UI_DIR = .ui

CONFIG += debug_and_release

win32 {
	INCLUDEPATH += C:\dev\glm C:\dev\glew-1.12.0\include
	LIBS += C:\dev\glew-1.12.0\lib\glew32.dll
}
linux {
	LIBS += -lGLEW -lGLU -lGL
}

QMAKE_CXXFLAGS += -msse -msse2 -mfpmath=sse -fpermissive -ggdb
QMAKE_CXXFLAGS += -DGLM_FORCE_RADIANS

# Warnings
QMAKE_CXXFLAGS += -Wno-unused-variable -Wno-unused-parameter -Wno-sign-compare

RESOURCES += QtSE.qrc
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
	IProjectObject.cpp \
    VTabWidgetArea.cpp \
	VGLSLEdit.cpp \
    tinyobjloader/tiny_obj_loader.cc \
	CObj.cpp \
    CStage.cpp \
	CProject.cpp \
	CCamera.cpp \
	VLogger.cpp \
    CProjectionMatrix.cpp \
	CAspectRatio.cpp \
	log.cpp \
    CFramebuffer.cpp \
    COrtho.cpp \
    CPerspective.cpp \
    CModel.cpp \
    CTexture.cpp \
    CShader.cpp \
    CModelViewMatrix.cpp \
    CWavefrontObj.cpp \
    VPreferences.cpp \
    VNewProjectDialog.cpp

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
	IProjectObject.h \
    VTabWidgetArea.h \
	VGLSLEdit.h \
    tinyobjloader/tiny_obj_loader.h \
	CObj.h \
    CStage.h \
	CProject.h \
	CCamera.h \
    StdRedirector.h \
	VLogger.h \
    CProjectionMatrix.h \
	CAspectRatio.h \
	log.h \
    CFramebuffer.h \
    COrtho.h \
    CPerspective.h \
    CModel.h \
    CTexture.h \
    CShader.h \
    CModelViewMatrix.h \
    CWavefrontObj.h \
    VPreferences.h \
    VNewProjectDialog.h

DISTFILES += \
    .gitignore
