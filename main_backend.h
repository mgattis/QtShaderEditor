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

#include <QtTest/QtTest>

int main( int argc , char *argv[] );

class TestQString: public QObject
{
	Q_OBJECT

private slots:
	void toUpper();
};

#endif
