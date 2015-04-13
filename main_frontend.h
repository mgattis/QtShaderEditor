#ifndef MAIN
#define MAIN

#include "CJsonTemplate.h"
#include "QtSE.h"

#include <GL/glew.h>
#include <GL/glut.h>

#include <QApplication>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <QStringList>

#include <QtTest/QtTest>
#include "CObj.h"

int main( int argc , char *argv[] );

class TestFrontend : public QObject
{
	Q_OBJECT

private:
	bool testObj( CObj *obj , const QString &filePath , const QString &basePath );

private slots:
	void initTestCase( void );

	void loadProjectDir( void );

	void loadObjGrassblock( void );
	void loadObjMinecraft( void );
	void loadObjSponza( void );
	void loadObjHead( void );
	void loadObjLostEmpire( void );
	void loadObjRungholt( void );
	void loadObjSanMiguel( void );

	void cleanupTestCase( void );
};

#endif
