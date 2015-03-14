#include "QtSE.h"

void QtSE::CProjectTreeItem::getRelativePath( QString &relativePath ) const
{
	if( partType == dir )
		relativePath.prepend( QString( "%1/" ).arg( this->text( 0 ) ) );
	else
		relativePath.append( this->text( 0 ) );

	if( this->parent() && !this->parent()->text( 0 ).isEmpty() )
		( (CProjectTreeItem*)this->parent() )->getRelativePath( relativePath );
}

QString QtSE::CProjectTreeItem::getFullPath( const QString &basePath ) const
{
	QString relativePath;
	getRelativePath( relativePath );
	return relativePath.prepend( basePath );
}

QtSE::QtSE( QWidget *parent ) : QMainWindow( parent )
{
	activeProjectItem = NULL;
	activeTabWidget = NULL;
	activeForm = NULL;

	connect( qApp , SIGNAL(focusChanged(QWidget*,QWidget*)) , this , SLOT(focusChanged(QWidget*,QWidget*)) );

	menuFile = this->menuBar()->addMenu( "File" );
		actionSave = menuFile->addAction( "Open" , this , SLOT(open()) , QKeySequence( Qt::CTRL | Qt::Key_O ) );
		actionSave = menuFile->addAction( "Save" , this , SLOT(save()) , QKeySequence( Qt::CTRL | Qt::Key_S ) );
		menuFile->addSeparator();
		actionQuit = menuFile->addAction( "Quit" , this , SLOT(close()) );

	menuView = this->menuBar()->addMenu( "View" );
		actionSplitHorizontally = menuView->addAction( "Split Horizontal" , this , SLOT(splitHorizontally()) );
		actionSplitVertically = menuView->addAction( "Split Vertical" , this , SLOT(splitVertically()) );
		actionSplitCollapse = menuView->addAction( "Collapse" , this , SLOT(splitCollapse()) );

	menuHelp = this->menuBar()->addMenu( "Help" );
		actionAbout = menuHelp->addAction( "About" , this , SLOT(close()) );

	projectTree = new QTreeWidget( NULL );
	projectTree->header()->setStretchLastSection( true );
	projectTree->header()->setHidden( true );
	projectTree->setContextMenuPolicy( Qt::CustomContextMenu );
	connect( projectTree , SIGNAL(customContextMenuRequested(QPoint)) , this , SLOT(projectTreeContextMenu(QPoint)) );

	fsProjectTree = new QTreeWidget( NULL );
	fsProjectTree->header()->setStretchLastSection( true );
	fsProjectTree->header()->setHidden( true );
	fsProjectTree->setContextMenuPolicy( Qt::CustomContextMenu );
	connect( fsProjectTree , SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)) , this , SLOT(fsProjectTreeItemDoubleClicked(QTreeWidgetItem*,int)) );
	connect( fsProjectTree , SIGNAL(customContextMenuRequested(QPoint)) , this , SLOT(fsProjectTreeContextMenu(QPoint)) );

	viewWidget = new VGLView( NULL );

	//CProjectTreeItem *dummyProject = new CProjectTreeItem( CProjectTreeItem::invalid , projectTree , QStringList( "testProject" ) );
	QTreeWidget *dummyProject = projectTree;
	CProjectTreeItem *dummyStage = new CProjectTreeItem( CProjectTreeItem::stage , dummyProject , QStringList( "Stages" ) );
	CProjectTreeItem *dummyStage1 = new CProjectTreeItem( CProjectTreeItem::invalid , dummyStage , QStringList( "Stage 1" ) );
	CProjectTreeItem *dummyFB = new CProjectTreeItem( CProjectTreeItem::framebuffer , dummyProject , QStringList( "Framebuffers" ) );
	CProjectTreeItem *dummyFB1 = new CProjectTreeItem( CProjectTreeItem::invalid , dummyFB , QStringList( "Framebuffer 1" ) );
	CProjectTreeItem *dummyShader = new CProjectTreeItem( CProjectTreeItem::shader , dummyProject , QStringList( "Shaders" ) );
	CProjectTreeItem *dummyShader1 = new CProjectTreeItem( CProjectTreeItem::invalid , dummyShader , QStringList( "Shader 1" ) );
	CProjectTreeItem *dummyModel = new CProjectTreeItem( CProjectTreeItem::model , dummyProject  , QStringList( "Models" ) );
	CProjectTreeItem *dummyModel1 = new CProjectTreeItem( CProjectTreeItem::invalid , dummyModel  , QStringList( "Model 1" ) );
	CProjectTreeItem *dummyTexture = new CProjectTreeItem( CProjectTreeItem::texture , dummyProject  , QStringList( "Textures" ) );
	CProjectTreeItem *dummyTexture1 = new CProjectTreeItem( CProjectTreeItem::invalid , dummyTexture  , QStringList( "Texture 1" ) );
	projectTree->expandAll();

	itemsTab = new QTabWidget( NULL );
	itemsTab->addTab( projectTree , "Project" );
	itemsTab->addTab( fsProjectTree , "Filesystem" );
	itemsTab->setTabPosition( QTabWidget::East );
	itemsTab->setTabsClosable( false );
	itemsTab->setContextMenuPolicy( Qt::CustomContextMenu );
	connect( itemsTab , SIGNAL(customContextMenuRequested(QPoint)) , this , SLOT(itemsTabTreeContextMenu(QPoint)) );

	viewSplitter = new QSplitter( NULL );
	viewSplitter->setOrientation( Qt::Vertical );
	viewSplitter->addWidget( viewWidget );
	viewSplitter->setStretchFactor( 0 , 1 );
	viewSplitter->addWidget( itemsTab );
	viewSplitter->setStretchFactor( 1 , 4 );

	VDraggableTabWidget *initialTab = makeVDraggableTabWidget();

	// Make some test stuff
	for( int index = 0 ; index < 3 ; index++ )
		initialTab->addTab( makeVJsonForm() , QString( "Shader %1" ).arg( index + 1 ) );

	windowSplitter = new QSplitter( NULL );
	windowSplitter->addWidget( viewSplitter );
	windowSplitter->setStretchFactor( 0 , 1 );
	windowSplitter->addWidget( initialTab );
	windowSplitter->setStretchFactor( 1 , 4 );

	this->setCentralWidget( windowSplitter );
	initialTab->widget( 0 )->setFocus();

	// Test
	open();
}

QtSE::~QtSE()
{
	// Nothing to do
}

VJsonForm* QtSE::makeVJsonForm( void )
{
	VJsonForm *form = new VJsonForm( NULL );
	connect( form , SIGNAL(destroyed(QObject*)) , this , SLOT(tabWidgetTabDestroyed(QObject*)) );
	connect( form , SIGNAL(modified()) , this , SLOT(formModified()) );
	connect( form , SIGNAL(unmodified()) , this , SLOT(formUnmodified()) );

	return form;
}

VDraggableTabWidget* QtSE::makeVDraggableTabWidget( void )
{
	VDraggableTabWidget *newTabWidget = new VDraggableTabWidget();
	connect( newTabWidget , SIGNAL(widgetAdded(QWidget*)) , this , SLOT(tabWidgetTabAdded(QWidget*)) );
	connect( newTabWidget , SIGNAL(tabCountChanged(int)) , this , SLOT(tabWidgetCountChanged(int)) );
	connect( newTabWidget , SIGNAL(destroyed(QObject*)) , this , SLOT(removeTabWidgetFromLayout(QObject*)) );
	connect( newTabWidget , SIGNAL(tabCloseRequested(int)) , this , SLOT(tabCloseRequested(int)) );

	return newTabWidget;
}

VDraggableTabWidget* QtSE::getFirstTabWidget( QSplitter *splitter ) const
{
	if( splitter )
	{
		for( int index = 0 ; index < splitter->count() ; index++ )
		{
			VDraggableTabWidget *tabWidget = dynamic_cast< VDraggableTabWidget* >( splitter->widget( index ) );

			if( !tabWidget )
			{
				tabWidget = getFirstTabWidget( dynamic_cast< QSplitter* >( splitter->widget( index ) ) );

				if( tabWidget )
					return tabWidget;
			}
			else
				return tabWidget;
		}
	}

	return NULL;
}

void QtSE::removeTabWidgetFromLayout( VDraggableTabWidget *tabWidget )
{
	std::cout << QString( "removeTabWidgetFromLayout( %1 )" ).arg( (int)tabWidget ).toLatin1().data() << std::endl;

	if( tabWidget )
	{
		QSplitter *parentSplitter = dynamic_cast< QSplitter* >( tabWidget->parentWidget() );

		// Don't close top splitter
		if( !parentSplitter || parentSplitter == windowSplitter )
			return;

		VJsonForm *form = tabWidget->currentWidget();
		VDraggableTabWidget* otherTabWidget = dynamic_cast< VDraggableTabWidget* >( parentSplitter->widget( !parentSplitter->indexOf( tabWidget ) ) );
		QSplitter *otherSplitter = dynamic_cast< QSplitter* >( parentSplitter->widget( !parentSplitter->indexOf( tabWidget ) ) );

		if( !otherTabWidget && otherSplitter )
			otherTabWidget = getFirstTabWidget( otherSplitter );

		if( otherTabWidget && parentSplitter->count() > 1 )
		{
			tabWidget->disconnect(); // Stop indirect recursion

			// Move remaining tabs from this tab to the next one
			while( tabWidget->count() )
				otherTabWidget->addTab( tabWidget->widget( 0 ) , tabWidget->tabText( 0 ) );

			//std::cout << QString( "parentSplitter->count() == %1" ).arg( parentSplitter->count() ).toLatin1().data() << std::endl;

			if( parentSplitter->count() == 2 )
			{
				QSplitter *parentParentSplitter = (QSplitter*)( parentSplitter->parentWidget() );

				// Add the other widget to the parent layout
				if( otherSplitter )
				{
					// windowSplitter should contain all the tab widgets in a single, child splitter
					// It may have other splits for different components
					//if( parentParentSplitter != windowSplitter && parentSplitter->orientation() == parentParentSplitter->orientation() )
					if( parentParentSplitter != windowSplitter && otherSplitter->orientation() == parentParentSplitter->orientation() )
					{
						// Add all of the other splitter's widgets to the splitter above that
						while( otherSplitter->count() )
							parentParentSplitter->addWidget( otherSplitter->widget( 0 ) );
					}
					else
						parentParentSplitter->addWidget( otherSplitter );
				}
				else
					parentParentSplitter->addWidget( otherTabWidget );

				parentSplitter->deleteLater();
			}
			else // Multiple splits. Simply remove one
				tabWidget->deleteLater();

			if( form )
				form->setFocus();
		}
	}
}

void QtSE::open( void )
{
	//loadProject( QFileDialog::getOpenFileName( this , "Open File" , "." , "JSON File (*.json)" ) );
	loadProject( "/home/terrenteller/Projects/QtShaderEditor/QtSEProjects/demo/testProject.project.json" );
}

void QtSE::save( void )
{
	QWidget *focus = this->focusWidget();

	if( focus )
	{
		VJsonForm *jsonForm = dynamic_cast< VJsonForm* >( focus );

		if( jsonForm )
		{
			jsonForm->save();
		}
	}
}

void QtSE::focusChanged( QWidget *previous , QWidget *current )
{
	std::cout << (int)previous << " -> " << (int)current << std::endl;

	if( current )
	{
		VDraggableTabWidget *tabWidget = dynamic_cast< VDraggableTabWidget* >( current );

		if( tabWidget )
		{
			std::cout << "VDraggableTabWidget*" << std::endl;
			activeTabWidget = tabWidget;
		}

		VJsonForm *jsonForm = dynamic_cast< VJsonForm* >( current );

		if( jsonForm )
		{
			std::cout << "VJsonForm*" << std::endl;
			activeForm = jsonForm;
			activeTabWidget = tabMap.value( activeForm , NULL );
		}
	}
}

void QtSE::projectTreeContextMenu( QPoint point )
{
	CProjectTreeItem *item = (CProjectTreeItem*)projectTree->itemAt( point );

	if( item )
	{
		QMenu *menu = new QMenu();
		menu->setAttribute( Qt::WA_DeleteOnClose , true );

		switch( item->getPartType() )
		{
			case CProjectTreeItem::stage:
				menu->addAction( "Add Stage" , this , SLOT(addStage()) );
				break;
			case CProjectTreeItem::framebuffer:
				menu->addAction( "Add Framebuffer" , this , SLOT(addFramebuffer()) );
				break;
			case CProjectTreeItem::shader:
				menu->addAction( "Add Shader" , this , SLOT(addShader()) );
				break;
			case CProjectTreeItem::texture:
				menu->addAction( "Add Texture" , this , SLOT(addTexture()) );
				break;
			case CProjectTreeItem::model:
				menu->addAction( "Add Model" , this , SLOT(addModel()) );
				break;
			default:
				delete menu;
				return;
		}

		menu->popup( QCursor::pos() , NULL );
	}
}

void QtSE::fsProjectTreeContextMenu( QPoint point )
{
	activeProjectItem = (CProjectTreeItem*)fsProjectTree->itemAt( point );

	if( activeProjectItem )
	{
		QMenu *menu = new QMenu();
		menu->setAttribute( Qt::WA_DeleteOnClose , true );

		switch( activeProjectItem->getPartType() )
		{
			case CProjectTreeItem::dir:
			{
				//activeFilePath.clear();
				//activeProjectItem->getRelativePath( activeFilePath );
				//activeFilePath.prepend( projectPath.getPath( true ) );

				menu->addAction( "Add Folder" , this , SLOT(addFolder()) );
				menu->addSeparator();
				menu->addAction( "Add Stage" , this , SLOT(addStage()) );
				menu->addAction( "Add Framebuffer" , this , SLOT(addFramebuffer()) );
				menu->addAction( "Add Shader" , this , SLOT(addShader()) );
				menu->addAction( "Add Model" , this , SLOT(addModel()) );
				menu->addAction( "Add Texture" , this , SLOT(addTexture()) );
				menu->addSeparator();
				menu->addAction( "Delete Folder" , this , SLOT(deleteItem()) );
				break;
			}
			default:
				delete menu;
				return;
		}

		menu->popup( QCursor::pos() , NULL );
	}
}

void QtSE::itemsTabTreeContextMenu( QPoint point )
{
	std::cout << "lol" << std::endl;

	int index = itemsTab->tabBar()->tabAt( itemsTab->tabBar()->mapFromGlobal( QCursor::pos() ) );

	if( index >= 0 )
	{
		QMenu *menu = new QMenu();
		menu->setAttribute( Qt::WA_DeleteOnClose , true );

		/*
		if( itemsTab->tabText( index ) == "Project" )
		{
			//
		}
		else
		*/
		if( itemsTab->tabText( index ) == "Filesystem" )
		{
			menu->addAction( "Refresh" , this , SLOT(fsRefresh()) );
		}
		else
		{
			delete menu;
			return;
		}

		menu->popup( QCursor::pos() , NULL );
	}
}

void QtSE::fsProjectTreeItemDoubleClicked( QTreeWidgetItem *item , int column )
{
	CProjectTreeItem *projectItem = dynamic_cast< CProjectTreeItem* >( item );

	if( projectItem && !projectItem->isDir() )
	{
		//QString filePath = projectItem->getFullPath( projectPath.getPath( true ) );

		if( activeTabWidget )
		{
			VJsonForm *form = makeVJsonForm();
			activeTabWidget->addTab( form , "test" );
		}
	}
}

void QtSE::split( Qt::Orientation orientation )
{
	QTabWidget *tabWidget = tabMap.value( this->focusWidget() , NULL );

	if( tabWidget && tabWidget->count() > 1 )
	{
		QSplitter *tabSplitter = tabWidget->parentWidget();
		VJsonForm *form = tabWidget->currentWidget();

		VDraggableTabWidget *newTabWidget = makeVDraggableTabWidget();
		newTabWidget->addTab( form , tabWidget->tabText( tabWidget->currentIndex() ) );
		tabMap[ form ] = newTabWidget;

		if( tabSplitter != windowSplitter && tabSplitter->orientation() == orientation )
			tabSplitter->addWidget( newTabWidget );
		else
		{
			QSplitter *newSplitter = new QSplitter( orientation , NULL );
			newSplitter->addWidget( tabWidget );
			newSplitter->addWidget( newTabWidget );

			tabSplitter->addWidget( newSplitter );
		}

		form->setFocus();
	}
}

void QtSE::splitCollapse( void )
{
	QTabWidget *tabWidget = tabMap.value( this->focusWidget() , NULL );

	removeTabWidgetFromLayout( tabWidget );
}

void QtSE::tabWidgetTabAdded( QWidget *widget )
{
	VJsonForm *form = dynamic_cast< VJsonForm* >( widget );
	VDraggableTabWidget *tabWidget = dynamic_cast< VDraggableTabWidget* >( QObject::sender() );

	if( form && tabWidget )
	{
		//std::cout << QString( "%1 added to %2" ).arg( (int)form ).arg( (int)tabWidget ).toLatin1().data() << std::endl;
		tabMap[ form ] = tabWidget;
	}
}

void QtSE::tabWidgetTabDestroyed( QObject *object )
{
	VJsonForm *form = dynamic_cast< VJsonForm* >( object );

	if( form )
	{
		//std::cout << QString( "%1 removed" ).arg( (int)form ).toLatin1().data() << std::endl;
		tabMap.remove( form );
	}
}

void QtSE::tabWidgetCountChanged( int count )
{
	//std::cout << QString( "tabWidgetCountChanged( %1 )" ).arg( count ).toLatin1().data() << std::endl;

	if( !count )
	{
		// Delete the tab widget that had it's last tab removed
		VDraggableTabWidget *caller = dynamic_cast< VDraggableTabWidget* >( QObject::sender() );

		if( caller && caller->parentWidget() != windowSplitter )
			caller->deleteLater();
	}
}

void QtSE::tabCloseRequested( int index )
{
	VDraggableTabWidget *widget = dynamic_cast< VDraggableTabWidget* >( QObject::sender() );

	if( widget )
	{
		VJsonForm *form = dynamic_cast< VJsonForm* >( widget->widget( index ) );

		if( widget->tabText( index ).endsWith( '*' , Qt::CaseInsensitive ) )
		{
			switch( QMessageBox::question( this , this->windowTitle() , "Save changes?" , QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel , QMessageBox::Cancel ) )
			{
				case QMessageBox::Yes:
					form->save();
				case QMessageBox::No:
					break;
				default:
					return;
			}
		}

		form->deleteLater();
	}
}

void QtSE::formModified( void )
{
	VJsonForm *form = QObject::sender();

	if( form )
	{
		VDraggableTabWidget *widget = tabMap.value( form , NULL );

		if( widget )
			widget->setTabModified( form );
	}
}

void QtSE::formUnmodified( void )
{
	VJsonForm *form = QObject::sender();

	if( form )
	{
		VDraggableTabWidget *widget = tabMap.value( form , NULL );

		if( widget )
			widget->setTabUnmodified( form );
	}
}

void QtSE::fsRefresh( void )
{
	loadProject();
}

void QtSE::loadProject( const QString &path /* = QString() */ )
{
	if( !path.isEmpty() )
		projectPath.setPath( path , true );

	QString basePath( projectPath.getPath( true ) );

	//projectTree->clear();
	fsProjectTree->clear();
	generateProjectTree( basePath , fsProjectTree->invisibleRootItem() );
}

void QtSE::generateProjectTree( const QString &path , QTreeWidgetItem *dirItem )
{
	QDir dir( path );
	dir.setFilter( QDir::AllEntries | QDir::NoDotAndDotDot );

	if( dir.exists() )
	{
		QDirIterator it( dir );

		do
		{
			it.next();

			if( !it.filePath().isEmpty() )
			{
				std::cout << it.fileName().toLatin1().data() << std::endl;
				// CPath fileName( it.fileName() , !it.fileInfo().isDir() );

				QString fileName = it.fileName();
				//if( fileName.endsWith( ".json" , Qt::CaseInsensitive ) )
					//fileName.chop( 5 );

				CProjectTreeItem *item = new CProjectTreeItem( it.fileInfo().isDir() ? CProjectTreeItem::dir : CProjectTreeItem::invalid , dirItem , QStringList( fileName ) );

				if( it.fileInfo().isDir() )
					generateProjectTree( it.filePath() , item ); // Recurse
			}
		}
		while( it.hasNext() );

		// Group dirs and move to end
		QList< QTreeWidgetItem* > dirItems;

		for( int index = dirItem->childCount() - 1 ; index >= 0 ; index-- )
			if( ( (CProjectTreeItem*)dirItem->child( index ) )->getPartType() == CProjectTreeItem::dir )
				dirItems.append( dirItem->takeChild( index ) );

		dirItem->addChildren( dirItems );
	}
}

void QtSE::addFolder( void )
{
	QString activeFilePath = activeProjectItem->getFullPath( projectPath.getPath( true ) );
	QDir dir( activeFilePath );

	if( dir.exists() )
	{
		bool ok = false;
		QString name = QInputDialog::getText( this , this->windowTitle() , "Directory Name:" , QLineEdit::Normal , "" , &ok );

		if( ok )
		{
			if( UTIL_validateFileName( name , false ) )
			{
				if( dir.mkdir( name ) )
				{
					CProjectTreeItem *item = new CProjectTreeItem( CProjectTreeItem::dir , activeProjectItem , QStringList( name ) );
					activeProjectItem->setExpanded( true );
				}
				else
					QMessageBox::warning( this , this->windowTitle() , "Unable to add directory." );
			}
			else
				QMessageBox::warning( this , this->windowTitle() , "Invalid directory name." );
		}
	}
	else
		QMessageBox::warning( this , this->windowTitle() , "Base path does not exist." );
}

void QtSE::addStage( void )
{
	//
}

void QtSE::addFramebuffer( void )
{
	//
}

void QtSE::addShader( void )
{
	//
}

void QtSE::addModel( void )
{
	//
}

void QtSE::addTexture( void )
{
	//
}

void QtSE::deleteItem( void )
{
	if( activeProjectItem )
		if( !deleteItem( activeProjectItem ) )
			QMessageBox::warning( this , this->windowTitle() , "Unable to delete one or more files or directories." );
}

bool QtSE::deleteItem( CProjectTreeItem *curItem /* = NULL */ )
{
	if( curItem )
	{
		// Delete children first
		while( curItem->childCount() )
			deleteItem( curItem->child( 0 ) );

		// Get file path
		QString activeFilePath = curItem->getFullPath( projectPath.getPath( true ) );

		// Delete depending on type
		QDir dir( activeFilePath );
		if( curItem->isDir() ? dir.rmdir( activeFilePath ) : dir.remove( activeFilePath ) )
		{
			delete curItem;
			return true;
		}
	}

	return false;
}
