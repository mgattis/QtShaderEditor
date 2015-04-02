#include "CJsonTemplate.h"
#include "QtSE.h"

#include <QApplication>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <QStringList>

#include "CProject.h"

int main( int argc , char *argv[] ) {
    glewInit();

	QApplication a( argc , argv );
    QtSE w;
	w.show();

    return a.exec();
}
