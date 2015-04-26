#include "VJsonForm.h"

ComboBoxDelegate::ComboBoxDelegate( QObject *parent /* = NULL */ ) : QItemDelegate( parent )
{
	//
}

QWidget* ComboBoxDelegate::createEditor( QWidget *parent , const QStyleOptionViewItem &option , const QModelIndex &index ) const
{
	// Is ghetto a technical term? Can't find a way to get the view of a model
	VJsonForm *treeWidget = (VJsonForm*)this->parent();
	VJsonFormItem *item = (VJsonFormItem*)treeWidget->itemFromIndex( index );

	if( item->valueList )
	{
		QComboBox* editor = new QComboBox( parent );
		for( int index = 0 ; index < item->valueList->size() ; index++ )
			editor->addItem( item->valueList->at( index ).toString() );

		return editor;
	}

	return QItemDelegate::createEditor( parent , option , index );
}

void ComboBoxDelegate::setEditorData( QWidget *editor , const QModelIndex &index ) const
{
	VJsonForm *treeWidget = (VJsonForm*)this->parent();
	VJsonFormItem *item = (VJsonFormItem*)treeWidget->itemFromIndex( index );

	if( item->valueList )
	{
		QComboBox *comboBox = (QComboBox*)editor;

		QVariant value = index.model()->data( index , Qt::EditRole );
		int curIndex = comboBox->findData( value , Qt::EditRole );

		if( curIndex != -1 )
			comboBox->setCurrentIndex( curIndex );
	}
	else
		QItemDelegate::setEditorData( editor , index );
}

VJsonForm::VJsonForm( QWidget *parent /* = NULL */ ) : QTreeWidget( parent )
{
	delegate = new ComboBoxDelegate( this );
	this->setItemDelegateForColumn( 2 , delegate );

	this->setColumnCount( 2 );
	this->setHeaderLabels( QStringList( "Key" ) << "Type" << "Value" );
	this->setEditTriggers( QAbstractItemView::NoEditTriggers );

	this->setContextMenuPolicy( Qt::CustomContextMenu );
	connect( this , SIGNAL(customContextMenuRequested(QPoint)) , this , SLOT(showContextMenu(QPoint)) );
	connect( this , SIGNAL(itemClicked(QTreeWidgetItem*,int)) , this , SLOT(editTreeItem(QTreeWidgetItem*,int)) );
	connect( this , SIGNAL(itemChanged(QTreeWidgetItem*,int)) , this , SLOT(itemTextChanged(QTreeWidgetItem*,int)) );

	this->expandAll();
	this->resizeColumnToContents( 0 );
}

VJsonForm::~VJsonForm()
{
	delete delegate;
}

void VJsonForm::closeEvent( QCloseEvent *event )
{
	if( this->isWindowModified() )
	{
		switch( QMessageBox::question( this , this->windowTitle() , "Save changes?" , QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel , QMessageBox::Cancel ) )
		{
			case QMessageBox::Yes:
				save();
			case QMessageBox::No:
				break;
			default:
				event->ignore();
				return;
		}
	}

	event->accept();
}

void VJsonForm::load( const QString &path )
{
	//QString type;
	QJsonObject obj = CJsonTemplate::get()->loadUserJson( path , type );

	if( !type.isEmpty() )
	{
		CJsonTemplate::get()->createTree( type , obj , this->invisibleRootItem() );
		this->setWindowModified( false );
		UTIL_expandTreeItems( this , this->invisibleRootItem() );

		filePath = path;
		setUnmodified();
		this->setWindowTitle( path.mid( path.lastIndexOf( "/" ) + 1 ) + "[*]" );
	}
}

void VJsonForm::save( void )
{
	if( filePath.isEmpty() )
	{
		filePath = QFileDialog::getSaveFileName( this , "Save File As" , "." , "JSON File (*.json)" );

		if( filePath.isEmpty() )
			return;
		else
			this->setWindowTitle( filePath.mid( filePath.lastIndexOf( '/' ) + 1 ) + "[*]" );
	}

	QJsonObject obj = toObject();
	//obj.insert( "itemType"  , type );

	QJsonDocument doc( obj );
	std::cout << doc.toJson().data() << std::endl;

	setUnmodified();
	return;

	QFile file( filePath );

	if( file.open( QIODevice::WriteOnly | QIODevice::Truncate ) )
	{
		//QJsonObject obj = toObject();

		//QJsonDocument doc( obj );
		//std::cout << doc.toJson().data() << std::endl;
		QByteArray contents;

		if( file.write( contents ) != -1 )
			setUnmodified();
		else
			slog::log( "Unable to write file" , file.errorString() );
	}
	else
		slog::log( "Unable to open file" , file.errorString() );
}

#if 0
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
#endif

QJsonObject VJsonForm::toObject( void )
{
	QJsonObject obj;

	const QTreeWidgetItem *rootItem = this->invisibleRootItem();

	for( int index = 0 ; index < rootItem->childCount() ; index++ )
	{
		VJsonFormItem *item = (VJsonFormItem*)rootItem->child( index );
		item->toObject( obj );
	}

	return obj;
}

void VJsonForm::showContextMenu( QPoint point )
{
	VJsonFormItem *item = lastContextItem = (VJsonFormItem*)this->itemAt( point );
	QMenu *menu = new QMenu( this );
	menu->setAttribute( Qt::WA_DeleteOnClose );

	if( item )
	{
		contextItemType = item->lastValue.toString();

		if( item->isArray )
			menu->addAction( "Add" , this , SLOT(addArrayItem()) );

		if( !item->isArray && item->type == CJsonKeyvalueData::structure )
		{
			QStringList itemKeys , keys = CJsonTemplate::get()->getKeysForStructure( item->lastValue.toString() );

			if( menu->actions().size() && keys.size() )
				menu->addSeparator();

			for( int index = 0 ; index < item->childCount() ; index++ )
				itemKeys.append( item->child( index )->text( 0 ) );

			for( int index = 0 ; index < keys.size() ; index++ )
			{
				QAction *action = menu->addAction( keys.at( index ) , this , SLOT(toggleStructureItem()) );
				action->setCheckable( true );

				if( itemKeys.contains( keys.at( index ) ) )
					action->setChecked( true );
			}
		}

		if( item->parent() && item->parent()->text( 1 ) == "ARRAY" )
		{
			if( menu->actions().size() )
				menu->addSeparator();
			menu->addAction( "Delete" , this , SLOT(removeArrayItem()) );
		}

		if( !menu->actions().size() )
		{
			delete menu;
			return;
		}
	}
	else
	{
		contextItemType = type;
		QTreeWidgetItem *item = lastContextItem = invisibleRootItem();
		QStringList itemKeys , keys = CJsonTemplate::get()->getKeysForStructure( type );

		for( int index = 0 ; index < item->childCount() ; index++ )
			itemKeys.append( item->child( index )->text( 0 ) );

		for( int index = 0 ; index < keys.size() ; index++ )
		{
			QAction *action = menu->addAction( keys.at( index ) , this , SLOT(toggleStructureItem()) );
			action->setCheckable( true );

			if( itemKeys.contains( keys.at( index ) ) )
				action->setChecked( true );
		}
	}

	menu->popup( this->mapToGlobal( point ) );
}

void VJsonForm::addArrayItem( void )
{
	VJsonFormItem *parent = (VJsonFormItem*)this->currentItem();

	//return;

	if( parent && parent->isArray )
	{
		//std::cout << CJsonKeyvalueData::getValueName( parent->type ).toLatin1().data() << std::endl;
		//std::cout << parent->defaultValue.toString().toLatin1().data() << std::endl;

		if( parent->type == CJsonKeyvalueData::structure )
		{
			VJsonFormItem *item = new VJsonFormItem( parent , parent->type , false , parent->defaultValue );
			item->setText( 0 , QString( "[%1]" ).arg( parent->childCount() - 1 ) );
			item->setText( 1 , parent->defaultValue.toString() );

			CJsonTemplate::get()->createTree( parent->defaultValue.toString() , item );
			UTIL_expandTreeItems( this , item );
		}
		else
		{
			VJsonFormItem *item = new VJsonFormItem( parent , parent->type , false , parent->defaultValue );
			item->setText( 0 , QString( "[%1]" ).arg( parent->childCount() - 1 ) );
			item->setText( 1 , CJsonKeyvalueData::getValueName( parent->type ).toUpper() );
			item->setData( 2 , Qt::DisplayRole , parent->defaultValue );
		}
	}
}

void VJsonForm::removeArrayItem( void )
{
	VJsonFormItem *item = (VJsonFormItem*)this->currentItem();

	if( item )
	{
		VJsonFormItem *parent = (VJsonFormItem*)item->parent();
		delete item;

		for( int index = 0 ; index < parent->childCount() ; index++ )
			parent->child( index )->setText( 0 , QString( "[%1]" ).arg( index ) );
	}
}

void VJsonForm::toggleStructureItem( void )
{
	QAction *action = (QAction*)QObject::sender();
	QString key = action->text();
	bool checked = action->isChecked();

	if( checked )
	{
		QTreeWidgetItem *parentItem = new QTreeWidgetItem( (QTreeWidgetItem*)NULL );
		CJsonTemplate::get()->createTree( contextItemType , parentItem , true );

		for( int index = 0 ; index < parentItem->childCount() ; index++ )
			if( !key.compare( parentItem->child( index )->text( 0 ) , Qt::CaseInsensitive ) )
			{
				int pos = -1;
				for( int parentIndex = 0 ; parentIndex < parentItem->childCount() ; parentIndex++ )
					for( int targetIndex = 0 ; targetIndex < lastContextItem->childCount() ; targetIndex++ )
						if( !lastContextItem->child( targetIndex )->text( 0 ).compare( parentItem->child( parentIndex )->text( 0 ) , Qt::CaseInsensitive ) )
						{
							pos++;
							break;
						}
						else if( !key.compare( parentItem->child( parentIndex )->text( 0 ) , Qt::CaseInsensitive ) )
						{
							// Don't continue past the key we're trying to add
							parentIndex = parentItem->childCount();
							targetIndex = lastContextItem->childCount();
							break;
						}

				lastContextItem->insertChild( pos + 1 , parentItem->takeChild( index ) );
				setModified();
				break;
			}

		delete parentItem;
	}
	else
	{
		for( int index = 0 ; index < lastContextItem->childCount() ; index++ )
			if( !key.compare( lastContextItem->child( index )->text( 0 ) , Qt::CaseInsensitive ) )
			{
				delete lastContextItem->takeChild( index );
				setModified();
				return;
			}
	}
}

void VJsonForm::editTreeItem( QTreeWidgetItem *item , int column )
{
	if( item && column == 2 )
	{
		VJsonFormItem *formItem = (VJsonFormItem*)item;

		if( !formItem->isArray && !( formItem->type == CJsonKeyvalueData::structure && !formItem->valueList ) )
			this->editItem( item , 2 );
	}
}

void VJsonForm::itemTextChanged( QTreeWidgetItem *item , int column )
{
	VJsonFormItem *formItem = (VJsonFormItem*)item;

	// WARNING: Column is important here. This methods gets called even on programmatic
	// changes. Beware infinite loops on corrupted data. If initial values must be
	// garbage, set the data on the item before adding it to the tree

	if( formItem )
	{
		if( column == 2 )
		{
			bool ok = false;

			switch( formItem->type )
			{
				case CJsonKeyvalueData::structure:
				{
					QString structureType = formItem->text( 2 );

					// setText in this block indirectly calls this function again. Avoid
					if( !( structureType.isEmpty() || structureType == formItem->lastValue.toString() ) )
					{
						// Make new structure
						QTreeWidgetItem *parentItem = new QTreeWidgetItem( (QTreeWidgetItem*)NULL );
						CJsonTemplate::get()->createTree( formItem->text( 2 ) , parentItem );

						// Clean up old structure
						while( formItem->child( 0 ) )
							delete formItem->takeChild( 0 );

						// Add new structure
						while( parentItem->child( 0 ) )
							formItem->addChild( parentItem->takeChild( 0 ) );

						// Clean up
						delete parentItem;
						formItem->lastValue = structureType;
						// Perhaps modify formItem's valueList pointer
						formItem->setText( 0 , structureType );
						formItem->setText( 1 , structureType.toUpper() );
						formItem->setText( 2 , "" );

						setModified();
					}

					break;
				}
				case CJsonKeyvalueData::boolean:
				{
					bool value = formItem->text( column ).toInt( &ok );

					if( !ok )
					{
						if( !formItem->text( 2 ).compare( "true" , Qt::CaseInsensitive ) )
						{
							value = true;
							ok = true;
						}
						else if( !formItem->text( 2 ).compare( "false" , Qt::CaseInsensitive ) )
						{
							value = false;
							ok = true;
						}

						// Don't default to one or the other. Use the last accepted value
					}

					if( ok )
					{
						formItem->setText( column , value ? "true" : "false" );
						formItem->lastValue = value;
						setModified();
					}
					else
						formItem->setText( column , formItem->lastValue.toBool() ? "true" : "false" );

					break;
				}
				case CJsonKeyvalueData::integer:
				{
					int value = formItem->text( column ).toInt( &ok );

					if( ok )
					{
						formItem->setText( column , QString::number( value ) );
						formItem->lastValue = value;
						setModified();
					}
					else
						formItem->setText( column , QString::number( formItem->lastValue.toInt() ) );

					break;
				}
				case CJsonKeyvalueData::floating:
				{
					float value = formItem->text( column ).toFloat( &ok );

					if( ok )
					{
						formItem->setText( column , QString::number( value ) );
						formItem->lastValue = value;
						setModified();
					}
					else
						formItem->setText( column , QString::number( formItem->lastValue.toFloat() ) );
					break;
				}
				case CJsonKeyvalueData::string:
					//if( formItem->lastValue.formItem->text( column ) )
					formItem->lastValue = formItem->text( column );
					setModified();
					break;
				default: {}
			}
		}
		else // NOTE: Expensive? This doesn't always work either
			resizeColumnToContents( column );
	}
}
