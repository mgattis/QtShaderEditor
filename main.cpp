#include "CJsonTemplate.h"
#include "QtSE.h"

#include <QApplication>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <QStringList>
#include <QSettings>

int main( int argc , char *argv[] )
{
	QApplication a( argc , argv );

	QCoreApplication::setOrganizationName( "Riintouge" );
	QCoreApplication::setApplicationName( "QtSE" );

    QtSE w;
	w.show();

    return a.exec();
}
