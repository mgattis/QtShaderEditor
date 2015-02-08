#include "CJsonTemplate.h"

CJsonTemplate::CJsonTemplate( const QString &path /* = QString() */ )
{
	parseDefaults( path );
}

CJsonTemplate::~CJsonTemplate()
{

}

void CJsonTemplate::parseDefaults( const QString &path /* = QString() */ )
{
	if( path.isNull() )
		path = "assets/template1.template.json";

	QFile file( path );
	file.open( QIODevice::ReadOnly );

	QByteArray data = file.readAll();

	QJsonParseError error;
	QJsonDocument doc = QJsonDocument::fromJson( data , &error );

	//std::cout << error.errorString().toLatin1().data();

	if( !doc.isEmpty() && doc.isObject() )
	{
		QJsonObject obj = doc.object();
		QStringList keys = obj.keys();

		// Top-level structures
		for( int structIndex = 0 ; structIndex < keys.length() ; structIndex++ )
		{
			std::cout << keys.at( structIndex ).toLatin1().data() << std::endl;

			CJsonStructure *str = new CJsonStructure;
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

						CJsonKeyvalueData *data = new CJsonKeyvalueData;
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
									data->type = CJsonKeyvalueData::boolean;
								else if( typeValue == "float" )
									data->type = CJsonKeyvalueData::floating;
								else if( typeValue == "integer" )
									data->type = CJsonKeyvalueData::integer;
								else if( typeValue == "string" )
									data->type = CJsonKeyvalueData::string;
								else
									data->type = CJsonKeyvalueData::structure;
							}
							else if( key == "value" )
							{
								// keys are read in alphabetical order so type is read before value
								switch( data->type )
								{
									case CJsonKeyvalueData::boolean:
										data->value = keyvalue.value( key ).toString( "false" ) == "true";
										std::cout << data->value.toBool();
										break;
									case CJsonKeyvalueData::floating:
										data->value = keyvalue.value( key ).toDouble();
										std::cout << data->value.toFloat();
										break;
									case CJsonKeyvalueData::integer:
										data->value = keyvalue.value( key ).toInt();
										std::cout << data->value.toInt();
										break;
									case CJsonKeyvalueData::string:
										data->value = keyvalue.value( key ).toString( "false" );
										std::cout << data->value.toString().toLatin1().data();
										break;
									case CJsonKeyvalueData::structure:
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
	}
}
