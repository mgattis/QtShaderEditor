#include "VJsonForm.h"

ColorListEditor::ColorListEditor(QWidget *widget) : QComboBox(widget)
{
	populateList();
}

QColor ColorListEditor::color() const
{
	return qvariant_cast<QColor>(itemData(currentIndex(), Qt::DecorationRole));
}

void ColorListEditor::setColor(QColor color)
{
	setCurrentIndex(findData(color, int(Qt::DecorationRole)));
}

void ColorListEditor::populateList()
{
	QStringList colorNames = QColor::colorNames();

	for (int i = 0; i < colorNames.size(); ++i) {
		QColor color(colorNames[i]);

		insertItem(i, colorNames[i]);
		setItemData(i, color, Qt::DecorationRole);
	}
}

// ====================================

VStringListEdit::VStringListEdit( QWidget *widget = NULL ) : QComboBox( widget )
{
	// Nothing to do
	populateList();
}

QString VStringListEdit::getString( void ) const
{
	return this->currentText();
	return "freeCamera";
	return qvariant_cast< QString >( itemData( currentIndex() , Qt::DecorationRole ) );
}

void VStringListEdit::setString( QString str )
{
	setCurrentIndex( findData( str , int( Qt::DecorationRole ) ) );
}

QStringList VStringListEdit::getStringList( void ) const
{
	QStringList list;
	for( int index = 0 ; index < this->count() ; index++ )
		list.append( this->itemText( index ) );
	return qvariant_cast< QStringList >( list );
	//return qvariant_cast< QStringList >( itemData( currentIndex() , Qt::DecorationRole ) );
}

void VStringListEdit::setStringList( QStringList list )
{
	if( list.size() )
	{
		//stringList = list;
		//std::cout << list.at( 0 ).toLatin1().data() << std::endl;
		this->clear();
		for( int index = 0 ; index < list.size() ; index++ )
		{
			insertItem( index , list.at( index ) );
			setItemData( index , list[ index ] , Qt::DecorationRole );
		}
	}
	//setCurrentIndex( findData( list , int( Qt::DecorationRole ) ) );
}

void VStringListEdit::populateList( void )
{
	QStringList cameraTypes( "orbitCamera" );
	cameraTypes << "freeCamera";

	for( int i = 0 ; i < cameraTypes.size() ; ++i )
	{
		insertItem( i , cameraTypes[ i ] );
		//setItemData( i , color , Qt::DecorationRole );
		//setItemData( i , colorNames[ i ] , Qt::DecorationRole );
	}

	return;


	QStringList colorNames = QColor::colorNames();

	for( int i = 0 ; i < colorNames.size() ; ++i )
	{
		QColor color( colorNames[ i ] );

		insertItem( i , colorNames[ i ] );
		//setItemData( i , color , Qt::DecorationRole );
		setItemData( i , colorNames[ i ] , Qt::DecorationRole );
	}
}

VJsonForm::VJsonForm( QWidget *parent /* = NULL */ ) : QTreeWidget( parent )
{
#if 0
	QItemEditorFactory *editorFactory = new QItemEditorFactory;
	QItemEditorCreatorBase *creator = new QStandardItemEditorCreator< VStringListEdit >();
	//editorFactory->registerEditor( QVariant::StringList , creator );
	editorFactory->registerEditor( QVariant::String , creator );
	QItemEditorFactory::setDefaultFactory( editorFactory );
	//this->setItemEditorFactory( editorFactory );
#endif

#if 0
	QItemEditorFactory *editorFactory = new QItemEditorFactory;
	QItemEditorCreatorBase *creator = new QStandardItemEditorCreator< ColorListEditor >();
	editorFactory->registerEditor( QVariant::Color , creator );
	QItemEditorFactory::setDefaultFactory( editorFactory );
#endif

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
	//
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

void VJsonForm::save( void )
{
	QJsonObject obj = toObject();

	QJsonDocument doc( obj );
	std::cout << doc.toJson().data() << std::endl;

	setUnmodified();
}

void VJsonForm::showContextMenu( QPoint point )
{
	VJsonFormItem *item = (VJsonFormItem*)this->itemAt( point );

	if( item )
	{
		if( item->valueList )
			std::cout << "valueList" << std::endl;

		QMenu *menu = new QMenu( this );
		menu->setAttribute( Qt::WA_DeleteOnClose );

		if( item->isArray )
			menu->addAction( "Add" , this , SLOT(addArrayItem()) );
		else if( item->parent() && item->parent()->text( 1 ) == "ARRAY" )
			menu->addAction( "Delete" , this , SLOT(removeArrayItem()) );
		else
		{
			delete menu;
			return;
		}

		menu->popup( this->mapToGlobal( point ) );
	}
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

void VJsonForm::editTreeItem( QTreeWidgetItem *item , int column )
{
	if( item && column == 2 )
	{
		VJsonFormItem *formItem = (VJsonFormItem*)item;

		if( !formItem->isArray && !formItem->type == CJsonKeyvalueData::structure )
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
					formItem->setText( column , formItem->defaultValue.toString() );
					break;
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
						formItem->setText( column , formItem->lastValue.toInt() );

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

void VJsonForm::setModified( void )
{
	//std::cout << QString( "void VJsonForm::setModified()" ).toLatin1().data() << std::endl;

	if( !this->windowTitle().endsWith( "*" ) )
		this->setWindowTitle( this->windowTitle().append( "*" ) );

	if( !isWindowModified() )
		setWindowModified( true );

	emit modified();
}

void VJsonForm::setUnmodified( void )
{
	//std::cout << QString( "void VJsonForm::setUnmodified()" ).toLatin1().data() << std::endl;

	if( this->windowTitle().endsWith( "*" ) )
	{
		QString title = this->windowTitle();
		title.chop( 1 );
		this->setWindowTitle( title );
	}

	if( isWindowModified() )
		setWindowModified( false );

	emit unmodified();
}
