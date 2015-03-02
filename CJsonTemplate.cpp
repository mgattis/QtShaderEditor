#include "CJsonTemplate.h"

void VJsonFormItem::toArray( QJsonArray &array ) const
{
	for( int index = 0 ; index < this->childCount() ; index++ )
	{
		VJsonFormItem *child = (VJsonFormItem*)this->child( index );

		/*if( item->isArray )
		{
			QJsonArray array;
			addArrayToSave( array , item->child( index ) );
			array.append( QJsonValue( array ) );
		}
		else*/ switch( child->type )
		{
			case CJsonKeyvalueData::structure:
			{
				QJsonObject obj;
				child->toStructure( obj );
				array.append( QJsonValue( obj ) );

				break;
			}
			case CJsonKeyvalueData::boolean:
				array.append( QJsonValue( (bool)child->text( 2 ).toInt() ) );
				break;
			case CJsonKeyvalueData::integer:
				array.append( QJsonValue( child->text( 2 ).toInt() ) );
				break;
			case CJsonKeyvalueData::floating:
				array.append( QJsonValue( child->text( 2 ).toFloat() ) );
				break;
			case CJsonKeyvalueData::string:
				array.append( QJsonValue( child->text( 2 ).toLatin1().data() ) );
				break;
			default: {}
		}
	}
}

void VJsonFormItem::toStructure( QJsonObject &obj ) const
{
	for( int index = 0 ; index < this->childCount() ; index++ )
	{
		VJsonFormItem *child = (VJsonFormItem*)this->child( index );
		child->toObject( obj );
	}
}

void VJsonFormItem::toObject( QJsonObject &obj ) const
{
	if( isArray )
	{
		QJsonArray array;
		toArray( array );
		obj.insert( this->text( 0 ) , QJsonValue( array ) );
	}
	else switch( this->type )
	{
		case CJsonKeyvalueData::structure:
		{
			QJsonObject childObj;
			this->toStructure( childObj );
			obj.insert( this->text( 0 ) , QJsonValue( childObj ) );
		}
		case CJsonKeyvalueData::boolean:
			obj.insert( this->text( 0 ) , QJsonValue( (bool)this->text( 2 ).toInt() ) );
			break;
		case CJsonKeyvalueData::integer:
			obj.insert( this->text( 0 ) , QJsonValue( this->text( 2 ).toInt() ) );
			break;
		case CJsonKeyvalueData::floating:
			obj.insert( this->text( 0 ) , QJsonValue( this->text( 2 ).toFloat() ) );
			break;
		case CJsonKeyvalueData::string:
			obj.insert( this->text( 0 ) , QJsonValue( this->text( 2 ).toLatin1().data() ) );
			break;
		default: {}
	}
}

CJsonTemplate::CJsonTemplate( const QString &path /* = QString() */ )
{
	parseDefaults( path );
}

CJsonTemplate::~CJsonTemplate()
{

}

void CJsonTemplate::parseDefaults( const QString &path /* = QString() */ )
{
	QFile file;

	if( path.isNull() )
		file.setFileName( "assets/template1.template.json" );
	else
		file.setFileName( path );

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
			//std::cout << keys.at( structIndex ).toLatin1().data() << std::endl;

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

							if( key == "guiInsert" )
								data->guiInsert = keyvalue.value( key ).toBool();
							else if( key == "indexable" )
								data->indexable = keyvalue.value( key ).toBool();
							else if( key == "key" )
							{
								data->key = keyvalue.value( key ).toString();
								//std::cout << data->key.toLatin1().data() << " : ";
							}
							else if( key == "projectInsert" )
								data->projectInsert = keyvalue.value( key ).toBool();
							else if( key == "type" )
							{
								QString typeValue = keyvalue.value( key ).toString();

								if( typeValue == "boolean" )
									data->type = CJsonKeyvalueData::boolean;
								else if( typeValue == "float" )
									data->type = CJsonKeyvalueData::floating;
								else if( typeValue == "integer" )
									data->type = CJsonKeyvalueData::integer;
								else if( typeValue == "string" )
									data->type = CJsonKeyvalueData::string;
								else
								{
									data->type = CJsonKeyvalueData::structure;
									data->value = typeValue;
								}
							}
							else if( key == "value" )
							{
								// keys are read in alphabetical order so type is read before value
								switch( data->type )
								{
									case CJsonKeyvalueData::boolean:
										data->value = keyvalue.value( key ).toString( "false" ) == "true";
										//std::cout << data->value.toBool();
										break;
									case CJsonKeyvalueData::floating:
										data->value = keyvalue.value( key ).toDouble();
										//std::cout << data->value.toFloat();
										break;
									case CJsonKeyvalueData::integer:
										data->value = keyvalue.value( key ).toInt();
										//std::cout << data->value.toInt();
										break;
									case CJsonKeyvalueData::string:
										data->value = keyvalue.value( key ).toString();
										break;
									case CJsonKeyvalueData::structure:
										//std::cout << data->value.toString().toLatin1().data();
										//break;
									default: {}
								}
							}

							//std::cout << keyz.toLatin1().data() << " : " << valuez.toLatin1().data() << std::endl;
						}

						//std::cout << std::endl;
					}
				}
			}
		}
	}
}

QJsonObject CJsonTemplate::createTree( const QString &name , bool gui ) const
{
	QJsonObject obj;
	QVector< CJsonKeyvalueData* > *keyvalues = (QVector< CJsonKeyvalueData* >*)structMap[ name ];

	for( int index = 0 ; index < keyvalues->length() ; index++ )
	{
		CJsonKeyvalueData *data = keyvalues->at( index );

		if( gui ? data->guiInsert : data->projectInsert )
		{
			if( data->indexable )
			{
				QJsonArray array;

				if( data->type == CJsonKeyvalueData::structure )
					array.insert( 0 , QJsonValue( createTree( keyvalues->at( index )->value.toString() , gui ) ) );
				else
					array.insert( 0 , QJsonValue::fromVariant( data->value ) );

				obj.insert( keyvalues->at( index )->key , QJsonValue( array ) );
			}
			else
			{
				if( data->type == CJsonKeyvalueData::structure )
					obj.insert( keyvalues->at( index )->key , QJsonValue( createTree( keyvalues->at( index )->value.toString() , gui ) ) );
				else
					obj.insert( keyvalues->at( index )->key , QJsonValue::fromVariant( data->value ) );
			}
		}
	}

	return obj;
}

void CJsonTemplate::createTree( const QString &name , QTreeWidgetItem *parent ) const
{
	QVector< CJsonKeyvalueData* > *keyvalues = (QVector< CJsonKeyvalueData* >*)structMap[ name ];

	for( int index = 0 ; index < keyvalues->length() ; index++ )
	{
		const CJsonKeyvalueData *data = keyvalues->at( index );
		//std::cout << data->key.toLatin1().data() << std::endl;

		//if( data->guiInsert )
		{
			if( data->indexable )
			{
				if( data->type == CJsonKeyvalueData::structure )
				{
					VJsonFormItem *array = new VJsonFormItem( parent , data->type , true , data->value );
					array->setText( 0 , data->key );
					array->setText( 1 , "ARRAY" );

					VJsonFormItem *item = new VJsonFormItem( array , data->type , false , data->value );
					item->setText( 0 , "[0]" );
					item->setText( 1 , data->getValueName() );

					createTree( data->value.toString() , item );
				}
				else
				{
					VJsonFormItem *array = new VJsonFormItem( parent , data->type , true , data->value );
					array->setText( 0 , data->key );
					array->setText( 1 , "ARRAY" );

					VJsonFormItem *item = new VJsonFormItem( array , data->type , false , data->value );
					item->setText( 0 , "[0]" );
					item->setText( 1 , data->getValueName().toUpper() );
					item->setData( 2 , Qt::DisplayRole , data->value );
				}
			}
			else
			{
				VJsonFormItem *item = new VJsonFormItem( parent , data->type , false , data->value );
				item->setText( 0 , data->key );
				item->setText( 1 , data->getValueName().toUpper() );

				if( data->type == CJsonKeyvalueData::structure )
					createTree( data->value.toString() , item );
				else
					item->setData( 2 , Qt::DisplayRole , data->value );
			}
		}
	}
}
