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
#if 0
	QJsonObject obj = CJsonTemplate::get()->createTree( "shader" , true );

	QJsonDocument doc( obj );
	QByteArray data = doc.toJson( QJsonDocument::Indented );
	std::cout << data.data();

	return 0;
#endif

#if 1
	QApplication a( argc , argv );
    QtSE w;
	w.show();

    return a.exec();
#endif
}
