#include "VJsonForm.h"

VJsonForm::VJsonForm( QWidget *parent /* = NULL */ ) : QTreeWidget( parent )
{
	this->setColumnCount( 2 );
	this->setHeaderLabels( QStringList( "Key" ) << "Value" );

	QJsonObject obj = CJsonTemplate::get()->createTree( "shader" , true );
	generateChildren( this->invisibleRootItem() , obj );

	this->expandAll();
	this->resizeColumnToContents( 0 );
}

VJsonForm::~VJsonForm()
{
	//
}

void VJsonForm::generateValue( QTreeWidgetItem *parent , const QString &name , QJsonValue &value , bool useParent )
{
	QTreeWidgetItem *item = NULL;

	if( !useParent )
	{
		item = new QTreeWidgetItem( parent );
		item->setText( 0 , name );
	}
	else
		item = parent;

	if( value.isArray() )
	{
		QJsonArray array = value.toArray();

		for( int index = 0 ; index < array.count() ; index++ )
		{
			QTreeWidgetItem *indexItem = new QTreeWidgetItem( item );
			indexItem->setText( 0 , QString( "[%1]" ).arg( index ) );

			QJsonValue value2 = array.at( index );
			generateValue( indexItem , "" , value2 , true );
		}
	}
	else if( value.isBool() )
		item->setText( 1 , value.toBool() ? "true" : "false" );
	else if( value.isDouble() )
		item->setText( 1 , QString( "%1" ).arg( value.toDouble() ) );
#if 0
	else if( value.isInt() )
	{
		QTreeWidgetItem *item = new QTreeWidgetItem( parent );
		item->setText( 0 , keys.at( keyIndex ) );
		item->setText( 1 , QString( "%1" ).arg( value.toInt() ) );
	}
#endif
	else if( value.isObject() )
	{
		QJsonObject object = value.toObject();
		generateChildren( parent , object );
	}
	else if( value.isString() )
		item->setText( 1 , value.toString() );
}

void VJsonForm::generateChildren( QTreeWidgetItem *parent , QJsonObject &object )
{
	if( parent )
	{
		QStringList keys = object.keys();

		for( int index = 0 ; index < keys.length() ; index++ )
		{
			QJsonValue value = object.value( keys.at( index ) );

			generateValue( parent , keys.at( index ) , value );
		}
	}
}
