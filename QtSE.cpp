#include "QtSE.h"

void QtSE::CProjectTreeItem::getRelativePath( QString &relativePath ) const
{
	// Recurse down, then append going up
	if( this->parent() && !this->parent()->text( 0 ).isEmpty() )
		( (CProjectTreeItem*)this->parent() )->getRelativePath( relativePath );

	relativePath.append( QString( "/%1" ).arg( this->text( 0 ) ) );
}

QString QtSE::CProjectTreeItem::getFullPath( const QString &basePath ) const
{
	QString relativePath = basePath;
	getRelativePath( relativePath );
	return relativePath;
}

QtSE::QtSE( QWidget *parent ) : QMainWindow( parent )
{
	// This doesn't work from main
	//glewInit();

	// Make this first so we can start intercepting messages ASAP
	coutEdit = new VLogger( NULL );
	std::cout << "test cout" << std::endl;
	std::cerr << "test cerr" << std::endl;
	std::clog << "test clog" << std::endl;

	//g_coutList = new QListWidget( NULL );

	activeProjectItem = NULL;

	//connect( qApp , SIGNAL(focusChanged(QWidget*,QWidget*)) , this , SLOT(focusChanged(QWidget*,QWidget*)) );

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
		actionAbout = menuHelp->addAction( "About" , this , SLOT(about()) );

	projectTree = new QTreeWidget( NULL );
	projectTree->header()->setStretchLastSection( true );
	projectTree->header()->setHidden( true );
	projectTree->setContextMenuPolicy( Qt::CustomContextMenu );
	connect( projectTree , SIGNAL(customContextMenuRequested(QPoint)) , this , SLOT(projectTreeContextMenu(QPoint)) );

#if 0
	fsProjectTree = new QTreeWidget( NULL );
	fsProjectTree->header()->setStretchLastSection( true );
	fsProjectTree->header()->setHidden( true );
	fsProjectTree->setContextMenuPolicy( Qt::CustomContextMenu );
	connect( fsProjectTree , SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)) , this , SLOT(fsProjectTreeItemDoubleClicked(QTreeWidgetItem*,int)) );
	connect( fsProjectTree , SIGNAL(customContextMenuRequested(QPoint)) , this , SLOT(fsProjectTreeContextMenu(QPoint)) );
#endif

	fsProjectTree2 = new QTreeView( NULL );
	fsProjectTree2->header()->setStretchLastSection( true );
	fsProjectTree2->header()->setHidden( true );
	fsProjectTree2->setContextMenuPolicy( Qt::CustomContextMenu );
	fsModel = new QFileSystemModel;
	connect( fsProjectTree2 , SIGNAL(clicked(QModelIndex)) , this , SLOT(fsProjectTree2ItemClicked(QModelIndex)) );
	connect( fsProjectTree2 , SIGNAL(customContextMenuRequested(QPoint)) , this , SLOT(fsProjectTree2ContextMenu(QPoint)) );

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
	itemsTab->addTab( fsProjectTree2 , "Filesystem" );
	itemsTab->addTab( projectTree , "Project" );
	//itemsTab->addTab( fsProjectTree , "Filesystem" );
	itemsTab->setTabPosition( QTabWidget::East );
	itemsTab->setTabsClosable( false );
	itemsTab->setContextMenuPolicy( Qt::CustomContextMenu );
	connect( itemsTab , SIGNAL(customContextMenuRequested(QPoint)) , this , SLOT(itemsTabTreeContextMenu(QPoint)) );

	viewSplitter = new QSplitter( Qt::Vertical , NULL );
	viewSplitter->addWidget( viewWidget );
	viewSplitter->setStretchFactor( 0 , 1 );
	viewSplitter->addWidget( itemsTab );
	viewSplitter->setStretchFactor( 1 , 4 );

	tabArea = new VTabWidgetArea();
	connect( tabArea , SIGNAL(widgetDeleted(QWidget*)) , this , SLOT(tabWidgetDeleted(QWidget*)) );

	editSplitter = new QSplitter( Qt::Vertical , NULL );
	editSplitter->addWidget( tabArea );
	editSplitter->setStretchFactor( 0 , 4 );
	editSplitter->addWidget( coutEdit );
	editSplitter->setStretchFactor( 1 , 1 );

	windowSplitter = new QSplitter( NULL );
	windowSplitter->addWidget( viewSplitter );
	windowSplitter->setStretchFactor( 0 , 4 );
	windowSplitter->addWidget( editSplitter );
	windowSplitter->setStretchFactor( 1 , 1 );

	this->setCentralWidget( windowSplitter );

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
	form->setAttribute( Qt::WA_DeleteOnClose );

	return form;
}

void QtSE::open( void )
{
	//loadProject( QFileDialog::getOpenFileName( this , "Open File" , "." , "JSON Project File (*.project.json)" ) );
	//return;

	// TODO: Load json and verify that it is a project
	//QDir::setCurrent( QCoreApplication::applicationDirPath() + "/QtSEProjects/experimental/" );
	QDir::setCurrent( QCoreApplication::applicationDirPath() + "/QtSEProjects/testProject/" );
	//QDir::setCurrent( QDir::homePath() + "/Projects/QtShaderEditor/QtSEProjects/experimental/" );

	fsModel->setRootPath( QDir::currentPath() );

	fsProjectTree2->setModel( fsModel );
	fsProjectTree2->setRootIndex( fsModel->index( QDir::currentPath() ) );

	// Hide all sections except for file name
	for( int index = 1 ; index < fsProjectTree2->header()->count() ; index++ )
		fsProjectTree2->header()->setSectionHidden( index , true );

	loadProject( QDir::currentPath() + "/testProject.project.json" );
}

void QtSE::save( void )
{
	QWidget *focus = this->focusWidget();

	if( focus )
	{
		if( VJsonForm *jsonForm = dynamic_cast< VJsonForm* >( focus ) )
			jsonForm->save();
		else if( VGLSLEdit *glslEdit = dynamic_cast< VGLSLEdit* >( focus ) )
			glslEdit->save();
	}
}

void QtSE::about( void )
{
	// Nothing to do yet
}

void QtSE::openPath( const QString path )
{
	QString relPath = path;
	std::cout << path.toLatin1().data() << std::endl;

	if( path.startsWith( '/' ) )
	{
		relPath = path.mid( QDir::currentPath().size() + 1 );
	}

	std::cout << relPath.toLatin1().data() << std::endl;
	relPath.prepend( '/' );

	if( !openFiles.contains( relPath ) )
	{
		QWidget *addWidget = NULL;
		QString filePath = QDir::currentPath().append( relPath );

		if( relPath.endsWith( ".json" , Qt::CaseInsensitive ) )
		{
			VJsonForm *form = makeVJsonForm();
			form->load( filePath );
			addWidget = form;
		}
		else if( relPath.endsWith( ".glsl" , Qt::CaseInsensitive ) )
		{
			VGLSLEdit *edit = new VGLSLEdit( NULL );
			edit->load( filePath );
			addWidget = edit;
		}

		if( addWidget )
		{
			openFiles.insert( relPath , addWidget );
			tabArea->addWidgetToArea( addWidget );
		}
	}
	else
		tabArea->showWidget( openFiles.value( relPath , NULL ) );
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

#if 0
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
#endif

void QtSE::itemsTabTreeContextMenu( QPoint point )
{
	//std::cout << "lol" << std::endl;

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

#if 0
void QtSE::fsProjectTreeItemDoubleClicked( QTreeWidgetItem *item , int column )
{
	CProjectTreeItem *projectItem = dynamic_cast< CProjectTreeItem* >( item );

	if( projectItem && !projectItem->isDir() )
	{
		QString relPath;
		projectItem->getRelativePath( relPath );
		relPath = relPath.mid( 1 );
		std::cout << "fsProjectTreeItemDoubleClicked: " << relPath.toLatin1().data() << std::endl;

		openPath( relPath );
	}
}
#endif

void QtSE::fsProjectTree2ContextMenu( QPoint point )
{
	fsContextIndex = fsProjectTree2->indexAt( point );

	if( fsContextIndex.isValid() )
	{
		QFileInfo info = fsModel->fileInfo( fsContextIndex );

		QMenu *menu = new QMenu();
		menu->setAttribute( Qt::WA_DeleteOnClose , true );

		if( info.isDir() )
		{
			menu->addAction( "Add Folder" , this , SLOT(addFolder()) );
			menu->addSeparator();
			menu->addAction( "Add Stage" , this , SLOT(addStage()) );
			menu->addAction( "Add Framebuffer" , this , SLOT(addFramebuffer()) );
			menu->addAction( "Add Shader" , this , SLOT(addShader()) );
			menu->addAction( "Add Model" , this , SLOT(addModel()) );
			menu->addAction( "Add Texture" , this , SLOT(addTexture()) );
			menu->addSeparator();
			menu->addAction( "Delete Folder" , this , SLOT(deleteItem()) );
		}
		else
		{
			delete menu;
			return;
		}

		menu->popup( QCursor::pos() , NULL );
	}
}

void QtSE::fsProjectTree2ItemClicked( QModelIndex index )
{
	QFileInfo info = fsModel->fileInfo( index );

	if( info.isFile() )
	{
		openPath( info.filePath() );
	}
	else if( info.isDir() )
	{
		if( fsProjectTree2->isExpanded( index ) )
			fsProjectTree2->collapse( index );
		else
			fsProjectTree2->expand( index );
	}
}

void QtSE::tabWidgetDeleted( QWidget *widget )
{
	QString key = openFiles.key( widget , "" );
	std::cout << key.toLatin1().data() << std::endl;
	openFiles.remove( key );
}

void QtSE::fsRefresh( void )
{
	loadProject();
}

void QtSE::loadProject( const QString &path /* = QString() */ )
{
	if( !path.isEmpty() )
	{
		QString projectValue;
		QJsonObject obj = CJsonTemplate::get()->loadUserJson( path , projectValue );

		if( !obj.isEmpty() && projectValue == "project" )
		{
			CPath pathParts( path , true );
			jsonProjectName = pathParts.getName( true );
			QDir::setCurrent( pathParts.getPath( false ) );

			//projectTree->clear();
			//fsProjectTree->clear();
			//generateProjectTree( QDir::current().absolutePath() , fsProjectTree->invisibleRootItem() );
		}
	}
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
	QFileInfo info = fsModel->fileInfo( fsContextIndex );

	if( info.isDir() )
	{
		bool ok = false;
		QString dirName = QInputDialog::getText( this , this->windowTitle() , "Folder Name:" , QLineEdit::Normal , "" , &ok );

		if( ok )
		{
			//std::cout << info.absoluteFilePath().toLatin1().data() << std::endl;
			QDir dir( info.absoluteFilePath() );

			if( !dir.exists( dirName ) )
				dir.mkdir( dirName );
		}
	}

#if 0
	QString activeFilePath = activeProjectItem->getFullPath( CPath( QDir::currentPath() ).getPath( true ) );
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
#endif
}

void QtSE::addProjectFile( QFileInfo &info , const QString &itemName , const QString &typeName )
{
	bool ok = false;
	QString name = QInputDialog::getText( this , this->windowTitle() , itemName + " Name:" , QLineEdit::Normal , "" , &ok );

	if( ok )
	{
		//std::cout << info.absoluteFilePath().toLatin1().data() << std::endl;
		QString path = info.absoluteFilePath() + "/" + name + ".json";
		QDir dir( path );
		std::cout << path.toLatin1().data() << std::endl;

		if( !dir.exists( path ) )
		{
			QFile file( path );

			if( file.open( QIODevice::WriteOnly ) )
			{
				QJsonObject obj = CJsonTemplate::get()->createTree( typeName , true );
				obj.insert( "itemName" , "itemname" );
				obj.insert( "itemType" , typeName );
				QJsonDocument doc( obj );
				file.write( doc.toJson() );
			}
		}
	}
}

void QtSE::addStage( void )
{
	QFileInfo info = fsModel->fileInfo( fsContextIndex );

	if( info.isDir() )
		addProjectFile( info , "Stage" , "stage" );
}

void QtSE::addFramebuffer( void )
{
	QFileInfo info = fsModel->fileInfo( fsContextIndex );

	if( info.isDir() )
		addProjectFile( info , "Framebuffer" , "framebuffer" );
}

void QtSE::addShader( void )
{
	QFileInfo info = fsModel->fileInfo( fsContextIndex );

	if( info.isDir() )
		addProjectFile( info , "Shader" , "shader" );
}

void QtSE::addModel( void )
{
	QFileInfo info = fsModel->fileInfo( fsContextIndex );

	if( info.isDir() )
		addProjectFile( info , "Model" , "model" );
}

void QtSE::addTexture( void )
{
	QFileInfo info = fsModel->fileInfo( fsContextIndex );

	if( info.isDir() )
		addProjectFile( info , "Texture" , "texture" );
}

void QtSE::deleteItem( void )
{
	QFileInfo info = fsModel->fileInfo( fsContextIndex );

	if( QMessageBox::Yes == QMessageBox::question( this , this->windowTitle() , "Are you sure?\nThis action cannot be undone." ) )
	{
		if( info.isDir() )
		{
			QDir dir( info.absoluteFilePath() );
			QString rmName = dir.dirName();
			dir.cdUp();
			dir.rmdir( rmName );
		}
		else
		{
			QDir dir( info.absolutePath() );
			dir.remove( info.fileName() );
		}
	}

	//if( activeProjectItem )
		//if( !deleteItem( activeProjectItem ) )
			//QMessageBox::warning( this , this->windowTitle() , "Unable to delete one or more files or directories." );
}

#if 0
bool QtSE::deleteItem( CProjectTreeItem *curItem /* = NULL */ )
{
	if( curItem )
	{
		// Delete children first
		while( curItem->childCount() )
			deleteItem( curItem->child( 0 ) );

		// Get file path
		QString activeFilePath = curItem->getFullPath( CPath( QDir::currentPath() ).getPath( true ) );

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
#endif
