#include "QtSE.h"

#if 0
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
#endif

QtSE::QtSE( QWidget *parent ) : QMainWindow( parent )
{
	// Make this first so we can start intercepting messages ASAP
	coutEdit = new VLogger( NULL );
	//std::cout << "test cout" << std::endl;
	//std::cerr << "test cerr" << std::endl;
	//std::clog << "test clog" << std::endl;

	//g_coutList = new QListWidget( NULL );

	//activeProjectItem = NULL;

	//connect( qApp , SIGNAL(focusChanged(QWidget*,QWidget*)) , this , SLOT(focusChanged(QWidget*,QWidget*)) );

	menuFile = this->menuBar()->addMenu( "File" );
		actionNew = menuFile->addAction( QIcon( ":/qrc/icons/world_add.png" ) , "New..." , this , SLOT(newProject()) );
		menuFile->addSeparator();
		actionSave = menuFile->addAction( QIcon( ":/qrc/icons/folder_explore.png" ) , "Open..." , this , SLOT(open()) , QKeySequence( Qt::CTRL | Qt::Key_O ) );
		menuRecentProjects = menuFile->addMenu( QIcon( ":/qrc/icons/timeline_marker.png" ) , "Recent Projects" );
		updateRecentProjects();
		menuFile->addSeparator();
		actionSave = menuFile->addAction( QIcon( ":/qrc/icons/disk.png" ) , "Save" , this , SLOT(save()) , QKeySequence( Qt::CTRL | Qt::Key_S ) );
		actionSaveAll = menuFile->addAction( QIcon( ":/qrc/icons/disk_multiple.png" ) , "Save All" , this , SLOT(saveAll()) , QKeySequence( Qt::CTRL | Qt::SHIFT | Qt::Key_S ) );
		menuFile->addSeparator();
		actionQuit = menuFile->addAction( QIcon( ":/qrc/icons/cross.png" ) , "Quit" , this , SLOT(close()) );

	menuView = this->menuBar()->addMenu( "View" );
		actionReloadProject = menuView->addAction( QIcon( ":/qrc/icons/arrow_refresh.png" ) , "Reload" , this , SLOT(reloadProject()) , QKeySequence( Qt::CTRL | Qt::Key_R ) );
		menuView->addSeparator();
		actionSplitVertically = menuView->addAction( QIcon( ":/qrc/icons/application_tile_horizontal.png" ) , "Split Vertical" , this , SLOT(splitVertically()) );
		actionSplitHorizontally = menuView->addAction( QIcon( ":/qrc/icons/application_tile_vertical.png" ) , "Split Horizontal" , this , SLOT(splitHorizontally()) );
		actionSplitCollapse = menuView->addAction( QIcon( ":/qrc/icons/arrow_join.png" ) , "Collapse" , this , SLOT(splitCollapse()) );

	menuSettings = this->menuBar()->addMenu( "Settings" );
		actionPreferences = menuSettings->addAction( QIcon( ":/qrc/icons/cog.png" ) , "Preferences..." , this , SLOT(about()) );

	menuHelp = this->menuBar()->addMenu( "Help" );
		actionAbout = menuHelp->addAction( QIcon( ":/qrc/icons/question_book.png" ) , "About" , this , SLOT(about()) );
		actionAboutQt = menuHelp->addAction( QIcon( ":/qrc/icons/qtcreator_logo_16.png" ) , "About Qt" , qApp , SLOT(aboutQt()) );

	fsProjectTree = new QTreeView( NULL );
	fsProjectTree->header()->setStretchLastSection( true );
	fsProjectTree->header()->setHidden( true );
	fsProjectTree->setContextMenuPolicy( Qt::CustomContextMenu );
	fsModel = new QFileSystemModel;
	connect( fsProjectTree , SIGNAL(clicked(QModelIndex)) , this , SLOT(fsProjectTreeItemClicked(QModelIndex)) );
	connect( fsProjectTree , SIGNAL(customContextMenuRequested(QPoint)) , this , SLOT(fsProjectTreeContextMenu(QPoint)) );

	projectTree = new QTreeWidget( NULL );
	projectTree->header()->setStretchLastSection( true );
	projectTree->header()->setHidden( true );
	projectTree->setContextMenuPolicy( Qt::CustomContextMenu );
	connect( projectTree , SIGNAL(customContextMenuRequested(QPoint)) , this , SLOT(projectTreeContextMenu(QPoint)) );

	//viewWidget = new VGLView( NULL );

#if 0
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
#endif

	itemsTab = new QTabWidget( NULL );
	itemsTab->addTab( fsProjectTree , "Filesystem" );
	//itemsTab->addTab( projectTree , "Project" );
	itemsTab->setTabPosition( QTabWidget::East );
	itemsTab->setTabsClosable( false );
	itemsTab->setContextMenuPolicy( Qt::CustomContextMenu );
	connect( itemsTab , SIGNAL(customContextMenuRequested(QPoint)) , this , SLOT(itemsTabTreeContextMenu(QPoint)) );

	viewSplitter = new QSplitter( Qt::Vertical , NULL );
	//viewSplitter->addWidget( viewWidget );
	//viewSplitter->setStretchFactor( 0 , 1 );
	viewSplitter->addWidget( itemsTab );
	//viewSplitter->setStretchFactor( 1 , 4 );

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

	// Restore settings
	QSettings settings( QSettings::IniFormat , QSettings::UserScope , QCoreApplication::organizationName() , QCoreApplication::applicationName() );
	windowSplitter->restoreState( settings.value( "gui/mainwindow/windowsplitter" ).toByteArray() );
	viewSplitter->restoreState( settings.value( "gui/mainwindow/viewsplitter" ).toByteArray() );
	editSplitter->restoreState( settings.value( "gui/mainwindow/editsplitter" ).toByteArray() );

	// Test
	open();
}

QtSE::~QtSE()
{
	// Nothing to do
}

void QtSE::closeEvent( QCloseEvent *event )
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

	QSettings settings( QSettings::IniFormat , QSettings::UserScope , QCoreApplication::organizationName() , QCoreApplication::applicationName() );
	settings.setValue( "gui/mainwindow/windowsplitter" , windowSplitter->saveState() );
	settings.setValue( "gui/mainwindow/viewsplitter" , viewSplitter->saveState() );
	settings.setValue( "gui/mainwindow/editsplitter" , editSplitter->saveState() );

	event->accept();
}

void QtSE::updateRecentProjects( const QString &path /* = QString() */ )
{
	QSettings settings( QSettings::IniFormat , QSettings::UserScope , QCoreApplication::organizationName() , QCoreApplication::applicationName() );
	QStringList recentProjectList = settings.value( "settings/recentprojects" ).toStringList();

	// Prepend path
	if( !path.isEmpty() )
	{
		// Remove to move
		if( recentProjectList.contains( path , Qt::CaseSensitive ) )
			recentProjectList.removeOne( path );

		// Only store five projects
		if( recentProjectList.size() == 5 )
			recentProjectList.removeLast();

		recentProjectList.prepend( path );
		settings.setValue( "settings/recentprojects" , recentProjectList );
	}

	// Rebuild menu
	menuRecentProjects->clear();
	menuRecentProjects->setDisabled( !recentProjectList.size() );

	for( int index = 0 ; index < recentProjectList.size() ; index++ )
		menuRecentProjects->addAction( recentProjectList.at( index ) , this , SLOT(openRecent()) );
}

void QtSE::newProject( void )
{
	// QDir, cd to QCoreApplication::applicationDirPath() + "/QtSEProjects",
	// make folder with name, create project json, save, load
}

void QtSE::open( void )
{
	//loadProject( QFileDialog::getOpenFileName( this , "Open File" , "." , "JSON Project File (*.project.json)" ) );
	//return;

	loadProject( QCoreApplication::applicationDirPath() + "/QtSEProjects/experimental/testProject.project.json" );
}

void QtSE::openRecent( void )
{
	QAction *sender = (QAction*)( QObject::sender() );
	std::cout << sender->text().toLatin1().data() << std::endl;
}

void QtSE::save( void )
{
	if( save( this->focusWidget() ) )
		reloadProject();
}

bool QtSE::save( QWidget *widget )
{
	if( widget && widget->isWindowModified() )
	{
		if( VJsonForm *jsonForm = dynamic_cast< VJsonForm* >( widget ) )
			jsonForm->save();
		else if( VGLSLEdit *glslEdit = dynamic_cast< VGLSLEdit* >( widget ) )
			glslEdit->save();

		return true;
	}

	return false;
}

void QtSE::saveAll( void )
{
	QList< QWidget* > widgets = openFiles.values();
	bool reload = false;

	for( int index = 0 ; index < widgets.size() ; index++ )
		if( save( widgets.at( index ) ) )
			reload = true;

	if( reload )
		reloadProject();
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
			VJsonForm *form = new VJsonForm( NULL );
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

void QtSE::reloadProject( void )
{
	viewWidget->openProject( QString( "%1/%2" ).arg( QDir::currentPath() ).arg( jsonProjectName ) );
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

void QtSE::itemsTabTreeContextMenu( QPoint point )
{
	// TODO: Is this even important anymore?
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
			//menu->addAction( "Refresh" , this , SLOT(fsRefresh()) );
		}
		else
		{
			delete menu;
			return;
		}

		menu->popup( QCursor::pos() , NULL );
	}
}

void QtSE::fsProjectTreeContextMenu( QPoint point )
{
	fsContextIndex = fsProjectTree->indexAt( point );

	if( fsContextIndex.isValid() )
	{
		QFileInfo info = fsModel->fileInfo( fsContextIndex );

		QMenu *menu = new QMenu();
		menu->setAttribute( Qt::WA_DeleteOnClose , true );

		if( info.isDir() )
		{
			menu->addAction( QIcon( ":/qrc/icons/folder_add.png" ) , "Add Folder" , this , SLOT(addFolder()) );
			menu->addSeparator();
			menu->addAction( QIcon( ":/qrc/icons/plugin_add.png" ) , "Add Stage" , this , SLOT(addStage()) );
			menu->addAction( QIcon( ":/qrc/icons/film_add.png" ) , "Add Framebuffer" , this , SLOT(addFramebuffer()) );
			menu->addAction( QIcon( ":/qrc/icons/script_add.png" ) , "Add Shader" , this , SLOT(addShader()) );
			menu->addAction( QIcon( ":/qrc/icons/vvd_add.png" ) , "Add Model" , this , SLOT(addModel()) );
			menu->addAction( QIcon( ":/qrc/icons/image_add.png" ) , "Add Texture" , this , SLOT(addTexture()) );
			menu->addSeparator();
			menu->addAction( QIcon( ":/qrc/icons/folder_delete.png" ) , "Delete Folder" , this , SLOT(deleteItem()) );
		}
		else if( info.isFile() )
		{
			menu->addAction( QIcon( ":/qrc/icons/page_white_delete.png" ) , "Delete File" , this , SLOT(deleteItem()) );
		}

		menu->popup( QCursor::pos() , NULL );
	}
}

void QtSE::fsProjectTreeItemClicked( QModelIndex index )
{
	QFileInfo info = fsModel->fileInfo( index );

	if( info.isFile() )
	{
		openPath( info.filePath() );
	}
	else if( info.isDir() )
	{
		if( fsProjectTree->isExpanded( index ) )
			fsProjectTree->collapse( index );
		else
			fsProjectTree->expand( index );
	}
}

void QtSE::tabWidgetDeleted( QWidget *widget )
{
	QString key = openFiles.key( widget , "" );
	std::cout << key.toLatin1().data() << std::endl;
	openFiles.remove( key );
}

void QtSE::loadProject( const QString &path /* = QString() */ )
{
	if( !path.isEmpty() )
	{
		QString projectValue;
		QJsonObject obj = CJsonTemplate::get()->loadUserJson( path , projectValue );

		if( !obj.isEmpty() && projectValue == "project" )
		{
			// File is valid
			CPath pathParts( path , true );
			jsonProjectName = pathParts.getName( true );
			QDir::setCurrent( pathParts.getPath( false ) );

			fsModel->setRootPath( QDir::currentPath() );

			fsProjectTree->setModel( fsModel );
			fsProjectTree->setRootIndex( fsModel->index( QDir::currentPath() ) );

			// Hide all sections except for file name
			for( int index = 1 ; index < fsProjectTree->header()->count() ; index++ )
				fsProjectTree->header()->setSectionHidden( index , true );

			// TODO: Do
			//projectTree->clear();

			updateRecentProjects( path );
		}
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
				obj.insert( "itemName" , name );
				obj.insert( "itemType" , typeName );
				QJsonDocument doc( obj );
				file.write( doc.toJson() );

				openPath( path );
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
