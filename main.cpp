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
	QJsonObject obj = CJsonTemplate::get()->createTree( "project" , true );

	QJsonDocument doc( obj );
	QByteArray data = doc.toJson( QJsonDocument::Indented );
	std::cout << data.data();

	return 0;

#if 0
		for( QJsonObject::iterator it = obj.begin() ; it != obj.end() ; it++ )
		{
			CJSONStructure *str = new CJSONStructure;
			structMap.insert( it.key() , str );

			QString name = it.key();
			std::cout << name.toLatin1().data() << "\n";

			if( it.value.isArray() )
			{
				QJsonArray array = it.value.toArray();

				for( int valueIndex = 0 ; valueIndex < array.count() ; valueIndex++ )
				{
					QJsonValue value = array.at( index );

					if( value.isObject() )
					{
						std::cout << "yey\n";
					}
				}
			}
			else
				std::cout << "nope\n";
		}
#endif

#if 0
	QApplication a( argc , argv );
    QtSE w;
	w.show();

    return a.exec();
#endif
}
