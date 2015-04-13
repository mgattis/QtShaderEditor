#ifndef MAIN
#define MAIN

#include "CJsonTemplate.h"
#include "QtSE.h"

#include <QApplication>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <QStringList>
#include <CProject.h>

#include <QtTest/QtTest>

int main( int argc , char *argv[] );

class TestBackend: public QObject
{
private:
    CProject project;
	Q_OBJECT

private slots:
    void projectOpen();

    void shaderInit();
    void textureInit();
    void modelInit();
    void framebufferInit();
    void stageInit();
};

#endif
