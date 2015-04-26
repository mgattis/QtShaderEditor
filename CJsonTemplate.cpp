#include "CJsonTemplate.h"

VJsonFormItem::VJsonFormItem( QTreeWidgetItem *parent , CJsonKeyvalueData::Type type , bool isArray , const QVariant &defaultValue ) : QTreeWidgetItem( parent )
{
	this->type = type;
	this->isArray = isArray;
	this->defaultValue = defaultValue;
	this->lastValue = defaultValue;
	valueList = NULL;

	setFlags( this->flags() | Qt::ItemIsEditable );
}

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
				array.append( QJsonValue( child->text( 2 ) ) );
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
			break;
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
			obj.insert( this->text( 0 ) , QJsonValue( this->text( 2 ) ) );
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
	QList< CJsonStructure* > list = structMap.values();
	UTIL_deleteListAsPointerArray( list );
}

void CJsonTemplate::parseDefaults( const QString &path /* = QString() */ )
{
	QFile file;

	if( path.isNull() )
		file.setFileName( QCoreApplication::applicationDirPath() + "/QtSEProjects/template.template.json" );
	else
		file.setFileName( path );

	file.open( QIODevice::ReadOnly );

	QByteArray data = file.readAll();

	QJsonParseError error;
	QJsonDocument doc = QJsonDocument::fromJson( data , &error );

	std::cerr << error.errorString().toLatin1().data();

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
							else if( key == "valueList" )
							{
								// TODO:
								data->valueList = new QVariantList;
								QStringList values = keyvalue.value( key ).toString().split( ':' , QString::SkipEmptyParts );

								bool ok = false;
								for( int index = 0 ; index < values.size() ; index++ )
								{
									switch( data->type )
									{
										case CJsonKeyvalueData::floating:
										{
											float temp = values.at( index ).toDouble( &ok );

											if( ok )
												data->valueList->append( QVariant( temp ) );
											//std::cout << data->value.toFloat();
											break;
										}
										case CJsonKeyvalueData::integer:
										{
											int temp = values.at( index ).toInt( &ok );

											if( ok )
												data->valueList->append( QVariant( temp ) );
											//std::cout << data->value.toInt();
											break;
										}
										case CJsonKeyvalueData::string:
										case CJsonKeyvalueData::structure:
										{
											data->valueList->append( QVariant( values.at( index ) ) );
											break;
										}
										//case CJsonKeyvalueData::boolean
										default: {}
									}

									ok = false;
								}

								// If the list has less than two valid values, it's not a valid list
								if( data->valueList->size() < 2 )
								{
									delete data->valueList;
									data->valueList = NULL;
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

void CJsonTemplate::validate( QJsonArray &cmp, const QJsonArray &ref )
{
	QJsonValue cmpValue;
	QJsonValue refValue;

	// Ignore if our reference array is empty
	if( ref.size() && ref.at( 0 ).type() != QJsonValue::Null )
	{
		refValue = ref.at( 0 );

		// Compare everything to the reference type
		for( int index = cmp.size() - 1 ; index >= 0 ; index-- )
		{
			cmpValue = cmp.at( index );

			// We don't use mixed type arrays. Remove anything that doesn't
			// match the type of the first value of the reference array.
			// Validate as normal for matching types
			if( refValue.type() == cmpValue.type() )
			{
				if( refValue.isArray() )
				{
					QJsonArray cmpArray = cmpValue.toArray();
					QJsonArray refArray = refValue.toArray();

					validate( cmpArray , refArray );
					cmp.replace( index , QJsonValue( cmpArray ) );
				}
				else if( refValue.isObject() )
				{
					QJsonObject cmpObject = cmpValue.toObject();
					QJsonObject refObject = refValue.toObject();

					validate( cmpObject , refObject );
					cmp.replace( index , QJsonValue( cmpObject ) );
				}
			}
			else
			{
				//std::cout << "removing " << index << std::endl;

				cmp.removeAt( index );
			}
		}

	}
}

void CJsonTemplate::validate( QJsonObject &cmp , const QJsonObject &ref )
{
	QStringList cmpKeys = cmp.keys();
	QStringList refKeys = ref.keys();

	// Iterate over known keys
	for( int index = 0 ; index < refKeys.size() ; index++ )
	{
		//QString refKey = refKeys.at( index );
		//std::cout << "refKey: " << refKey.toLatin1().data() << std::endl;

		QJsonValue cmpValue = cmp.value( refKeys.at( index ) );
		QJsonValue refValue = ref.value( refKeys.at( index ) );

		//std::cout << refValue.type() << " = " << cmpValue.type() << std::endl;

		if( refValue.type() == cmpValue.type() )
		{
			if( refValue.isArray() )
			{
				//std::cout << refKeys.at( index ).toLatin1().data() << " is array" << std::endl;

				QJsonArray cmpArray = cmpValue.toArray();
				QJsonArray refArray = refValue.toArray();

				//std::cout << "Array Size: " << refArray.size() << std::endl;

				validate( cmpArray , refArray );
				cmp.insert( refKeys.at( index ) , QJsonValue( cmpArray ) );
			}
			else if( refValue.isObject() )
			{
				//std::cout << refKeys.at( index ).toLatin1().data() << " is object" << std::endl;

				QJsonObject cmpObject = cmpValue.toObject();
				QJsonObject refObject = refValue.toObject();

				//std::cout << "isObject: " << std::endl;

				validate( cmpObject , refObject );
				cmp.insert( refKeys.at( index ) , QJsonValue( cmpObject ) );
			}
		}
		else if( cmpValue.type() != QJsonValue::Undefined )
		{
			// If cmp doesn't contain a value, don't add it
			//std::cout << "replacing " << refKeys.at( index ).toLatin1().data() << std::endl;

			cmp.insert( refKeys.at( index ) , ref[ refKeys.at( index ) ] );
		}

		cmpKeys.removeOne( refKeys.at( index ) );
	}

	// Remove unknown keys
	for( int index = 0 ; index < cmpKeys.size() ; index++ )
		cmp.remove( cmpKeys.at( index ) );
}

QJsonObject CJsonTemplate::createTree( const QString &name , bool gui ) const
{
	QJsonObject obj;
	QVector< CJsonKeyvalueData* > *keyvalues = ( QVector< CJsonKeyvalueData* >* )structMap.value( name , NULL );

	if( keyvalues )
	{
		for( int index = 0 ; index < keyvalues->length() ; index++ )
		{
			CJsonKeyvalueData *data = keyvalues->at( index );

			// TODO: What was the difference again?
			// This is important for validation purposes. Ask mgattis
			if( gui ? data->guiInsert : true )
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
	}

	return obj;
}

void CJsonTemplate::createTree( const QString &name , QTreeWidgetItem *parent , bool guiOnly ) const
{
	QVector< CJsonKeyvalueData* > *keyvalues = ( QVector< CJsonKeyvalueData* >* )structMap.value( name , NULL );

	if( keyvalues )
	{
		for( int index = 0 ; index < keyvalues->length() ; index++ )
		{
			const CJsonKeyvalueData *data = keyvalues->at( index );
			//std::cout << data->key.toLatin1().data() << std::endl;

			if( guiOnly ? data->guiInsert : true )
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

						createTree( data->value.toString() , item , guiOnly );
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
						createTree( data->value.toString() , item , guiOnly );
					else
						item->setData( 2 , Qt::DisplayRole , data->value );
				}
			}
		}
	}
}

void CJsonTemplate::createTree( const QString &name , const QJsonObject &obj , QTreeWidgetItem *parent ) const
{
	QVector< CJsonKeyvalueData* > *keyvalues = ( QVector< CJsonKeyvalueData* >* )structMap.value( name , NULL );

	if( keyvalues )
	{
		for( int index = 0 ; index < keyvalues->length() ; index++ )
		{
			const CJsonKeyvalueData *data = keyvalues->at( index );
			//std::cout << "Working on: " << data->key.toLatin1().data() << std::endl;

			// Some keys are optional or mutually exclusive with others
			// Don't attempt to parse something we don't have
			//if( guiOnly ? data->guiInsert : true )
			{
				if( obj.contains( data->key ) )
				{
					if( data->indexable )
					{
						// NOTE: Iterating over the array does not return items in alphabetical order
						QJsonArray objArray = obj.value( data->key ).toArray();

						if( data->type == CJsonKeyvalueData::structure )
						{
							VJsonFormItem *array = new VJsonFormItem( parent , data->type , true , data->value );
							array->setText( 0 , data->key );
							array->setText( 1 , "ARRAY" );

							for( int arrayIndex = 0 ; arrayIndex < objArray.size() ; arrayIndex++ )
							{
								VJsonFormItem *item = new VJsonFormItem( array , data->type , false , data->value );
								item->setText( 0 , QString( "[%1]" ).arg( arrayIndex ) );
								item->setText( 1 , data->getValueName() );

								QJsonObject arrayObject = objArray.at( arrayIndex ).toObject();
								createTree( data->value.toString() , arrayObject , item );
							}
						}
						else
						{
							VJsonFormItem *array = new VJsonFormItem( parent , data->type , true , data->value );
							array->setText( 0 , data->key );
							array->setText( 1 , "ARRAY" );

							for( int arrayIndex = 0 ; arrayIndex < objArray.size() ; arrayIndex++ )
							{
								VJsonFormItem *item = new VJsonFormItem( array , data->type , false , data->value );
								item->setText( 0 , QString( "[%1]" ).arg( arrayIndex ) );
								item->setText( 1 , data->getValueName().toUpper() );
								item->setData( 2 , Qt::DisplayRole , objArray.at( arrayIndex ).toVariant() );
							}
						}
					}
					else // data not indexable
					{
						VJsonFormItem *item = new VJsonFormItem( parent , data->type , false , data->value );
						item->setText( 0 , data->key );
						item->setText( 1 , data->getValueName().toUpper() );
						item->valueList = data->valueList;

						if( data->type == CJsonKeyvalueData::structure )
						{
							//std::cout << "toString() " << data->key.toLatin1().data() << " = " << data->value.toString().toLatin1().data() << "" << std::endl;

							QJsonValue strValue = obj.value( data->key );

							//if( strValue.isObject() )
								//std::cout << "Type: " << (int)strValue.type() << std::endl;

							QJsonObject strObject = strValue.toObject();
							//QStringList keys = strObject.keys();
							//for( int index2 = 0 ; index2 < keys.size() ; index2++ )
								//std::cout << "Key: " << keys.at( index2 ).toLatin1().data() << std::endl;
							createTree( data->value.toString() , strObject , item );
						}
						else
						{
							switch( data->type )
							{
								case CJsonKeyvalueData::boolean:
									item->setData( 2 , Qt::EditRole , obj.value( data->key ).toVariant() );
									//item->setText( 2 , obj.value( data->key ).toString( "false" ) == "true" ? "true" : "false" );
									break;
								case CJsonKeyvalueData::floating:
									item->setText( 2 , QString::number( obj.value( data->key ).toDouble() ) );
									break;
								case CJsonKeyvalueData::integer:
									item->setText( 2 , QString::number( obj.value( data->key ).toInt() ) );
									break;
								case CJsonKeyvalueData::string:
									if( item->valueList )
										item->setData( 2 , Qt::DisplayRole , obj.value( data->key ).toString() );
									else
										item->setText( 2 , obj.value( data->key ).toString() );
									break;
								default: {}
							}

							//item->setData( 2 , Qt::EditRole , obj.value( data->key ).toVariant() );
						}
					}
				}
			}
		}
	}
}

QStringList CJsonTemplate::getKeysForStructure( const QString name )
{
	QStringList keys;
	CJsonStructure *data = structMap.value( name , NULL );

	if( data )
		for( int index = 0 ; index < data->keyvalues.size() ; index++ )
			keys.append( data->keyvalues.at( index )->key );

	return keys;
}

QJsonObject CJsonTemplate::loadUserJson( const QString &path , QString &type ) const
{
	QJsonObject userObject;
	QFile file( path );

	if( file.exists() && ( file.isOpen() || file.open( QFile::ReadOnly ) ) )
	{
		QJsonParseError error;
		QJsonDocument doc = QJsonDocument::fromJson( file.readAll() , &error );

		if( error.error == QJsonParseError::NoError && doc.isObject() )
		{
			userObject = doc.object();
			type = userObject.value( "itemType" ).toString();
			//std::cout << userObject.value( "itemType" ).toString().toLatin1().data() << std::endl;
			//std::cout << userObject.value( "itemName" ).toString().toLatin1().data() << std::endl;

			// Create a template with all possible values
			QJsonObject templateObject = createTree( type , false );

#if 0
			std::cout << "===============================" << std::endl;

			std::cout << "templateObject" << std::endl;
			QJsonDocument doc2( templateObject );
			std::cout << doc2.toJson().data() << std::endl;

			std::cout << "userObject" << std::endl;
			QJsonDocument doc3( userObject );
			std::cout << doc3.toJson().data() << std::endl;
#endif

			// Validate the user object against the template
			validate( userObject , templateObject );

#if 0
			std::cout << "validated" << std::endl;
			QJsonDocument doc4( userObject );
			std::cout << doc4.toJson().data() << std::endl;

			std::cout << "===============================" << std::endl;
#endif
		}
		else
			std::cout << error.errorString().toLatin1().data() << std::endl;
	}

	return userObject;
}
