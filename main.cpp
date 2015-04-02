#include "CJsonTemplate.h"
#include "QtSE.h"

#include <QApplication>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <QStringList>

int main( int argc , char *argv[] )
{
	QApplication a( argc , argv );
    QtSE w;
	w.show();

    return a.exec();
}
