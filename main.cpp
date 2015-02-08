#include <QApplication>
#include "QtSE.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <QStringList>

#include <iostream>

// value: boolean, integer, float, string

struct CJSONKeyvalueData
{
	enum Type
	{
		structure = 0,
		boolean,
		integer,
		floating,
		string
	};

	CJSONKeyvalueData( void )
	{
		indexable = autoInsert = false;
	}

	QString key;
	Type type;
	QVariant value;
	bool indexable , autoInsert;
};

struct CJSONStructure
{
	QVector< CJSONKeyvalueData* > keyvalues;
};

void parseObj( QJsonObject &obj )
{
	//
}

int main( int argc , char *argv[] )
{
	QFile file( "assets/template1.template.json" );
	file.open( QIODevice::ReadOnly );

	QByteArray data = file.readAll();
	//for( int index = 0 ; index < 10 ; index++ )
		//std::cout << data[ index ];

	QJsonParseError error;
	QJsonDocument doc = QJsonDocument::fromJson( data , &error );

	//std::cout << error.errorString().toLatin1().data();

	if( !doc.isEmpty() && doc.isObject() )
	{
		QMap< QString , CJSONStructure* > structMap;

		QJsonObject obj = doc.object();
		QStringList keys = obj.keys();

		// Top-level structures
		for( int structIndex = 0 ; structIndex < keys.length() ; structIndex++ )
		{
			std::cout << keys.at( structIndex ).toLatin1().data() << std::endl;

			CJSONStructure *str = new CJSONStructure;
			structMap.insert( keys.at( structIndex ) , str );

			if( obj.value( keys.at( structIndex ) ).isArray() )
			{
				QJsonArray array = obj.value( keys.at( structIndex ) ).toArray();

				// Keyvalue objects
				for( int paramIndex = 0 ; paramIndex < array.count() ; paramIndex++ )
				{
					QJsonValue value = array.at( paramIndex );

					if( value.isObject() )
					{
						QJsonObject keyvalue = value.toObject();
						QStringList keyvalueKeys = keyvalue.keys();

						CJSONKeyvalueData *data = new CJSONKeyvalueData;
						str->keyvalues.append( data );

						// Keyvalue information
						for( int keyIndex = 0 ; keyIndex < keyvalueKeys.count() ; keyIndex++ )
						{
							QString key = keyvalueKeys.at( keyIndex );

							if( key == "autoInsert" )
								data->autoInsert = keyvalue.value( key ).toString( "false" ) == "true";
							else if( key == "indexable" )
								data->indexable = keyvalue.value( key ).toString( "false" ) == "true";
							else if( key == "key" )
							{
								data->key = keyvalue.value( key ).toString( "" );
								std::cout << data->key.toLatin1().data() << " : ";
							}
							else if( key == "type" )
							{
								QString typeValue = keyvalue.value( key ).toString( "" );

								if( typeValue == "boolean" )
									data->type = CJSONKeyvalueData::boolean;
								else if( typeValue == "float" )
									data->type = CJSONKeyvalueData::floating;
								else if( typeValue == "integer" )
									data->type = CJSONKeyvalueData::integer;
								else if( typeValue == "string" )
									data->type = CJSONKeyvalueData::string;
								else
									data->type = CJSONKeyvalueData::structure;
							}
							else if( key == "value" )
							{
								// keys are read in alphabetical order so type is read before value
								switch( data->type )
								{
									case CJSONKeyvalueData::boolean:
										data->value = keyvalue.value( key ).toString( "false" ) == "true";
										std::cout << data->value.toBool();
										break;
									case CJSONKeyvalueData::floating:
										data->value = keyvalue.value( key ).toDouble();
										std::cout << data->value.toFloat();
										break;
									case CJSONKeyvalueData::integer:
										data->value = keyvalue.value( key ).toInt();
										std::cout << data->value.toInt();
										break;
									case CJSONKeyvalueData::string:
										data->value = keyvalue.value( key ).toString( "false" );
										std::cout << data->value.toString().toLatin1().data();
										break;
									case CJSONKeyvalueData::structure:
									{
										std::cout << std::endl;
										break;
									}
									default: {}
								}
							}

							//std::cout << keyz.toLatin1().data() << " : " << valuez.toLatin1().data() << std::endl;
						}

						std::cout << std::endl;
					}
				}
			}
		}

return;
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
	}

#if 0
	QApplication a( argc , argv );
    QtSE w;
	w.show();

    return a.exec();
#endif
}
