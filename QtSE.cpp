#include "QtSE.h"

QtSE::QtSE( QWidget *parent ) : QMainWindow( parent )
{
	menuFile = this->menuBar()->addMenu( "File" );
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
	connect( projectTree , SIGNAL(customContextMenuRequested(QPoint)) , this , SLOT(treeContextMenu(QPoint)) );

	viewWidget = new VGLView( NULL );

	CProjectTreeItem *dummyStage = new CProjectTreeItem( CProjectTreeItem::stage , projectTree , QStringList( "Stage 1" ) );
	CProjectTreeItem *dummyFB = new CProjectTreeItem( CProjectTreeItem::framebuffer , dummyStage , QStringList( "Framebuffer 1" ) );
	CProjectTreeItem *dummyShader = new CProjectTreeItem( CProjectTreeItem::shader , dummyFB , QStringList( "Shader 1" ) );
	CProjectTreeItem *dummyModel = new CProjectTreeItem( CProjectTreeItem::model , dummyShader  , QStringList( "Model 1" ) );
	CProjectTreeItem *dummyTexture = new CProjectTreeItem( CProjectTreeItem::texture , dummyModel  , QStringList( "Texture 1" ) );
	projectTree->expandAll();

	itemsTab = new QTabWidget( NULL );
	itemsTab->addTab( projectTree , "Project" );
	itemsTab->setTabPosition( QTabWidget::East );
	itemsTab->setTabsClosable( false );

	viewSplitter = new QSplitter( NULL );
	viewSplitter->setOrientation( Qt::Vertical );
	viewSplitter->addWidget( viewWidget );
	viewSplitter->setStretchFactor( 0 , 1 );
	viewSplitter->addWidget( itemsTab );
	viewSplitter->setStretchFactor( 1 , 4 );

#if 0
	editorArea = new QMdiArea( NULL );

	QTextEdit *testEdit = new QTextEdit( NULL );
	editorArea->addSubWindow( testEdit );
#endif

	VJsonForm *form = new VJsonForm( NULL );
	VJsonForm *form2 = new VJsonForm( NULL );

	QTabWidget *initialTab = new QTabWidget( NULL );
	initialTab->setTabsClosable( true );
	initialTab->setMovable( true );
	initialTab->addTab( form , "Shader 1" );
	initialTab->addTab( form2 , "Shader 2" );

	windowSplitter = new QSplitter( NULL );
	windowSplitter->addWidget( viewSplitter );
	windowSplitter->setStretchFactor( 0 , 1 );
	windowSplitter->addWidget( initialTab );
	windowSplitter->setStretchFactor( 1 , 4 );

	splitterMap.insert( initialTab , windowSplitter );
	tabMap.insert( form , initialTab );
	tabMap.insert( form2 , initialTab );

	this->setCentralWidget( windowSplitter );
	form->setFocus();
}

QtSE::~QtSE()
{
	// Nothing to do
}

void QtSE::treeContextMenu( QPoint point )
{
	CProjectTreeItem *item = (CProjectTreeItem*)projectTree->itemAt( point );
	QMenu *menu = new QMenu();
	menu->setAttribute( Qt::WA_DeleteOnClose , true );

	if( item )
	{
		switch( item->getPartType() )
		{
			case CProjectTreeItem::stage:
				menu->addAction( "Add Framebuffer" , this , SLOT(addFramebuffer()) );
				break;
			case CProjectTreeItem::framebuffer:
				menu->addAction( "Add Shader" , this , SLOT(addShader()) );
				break;
			case CProjectTreeItem::shader:
				menu->addAction( "Add Model" , this , SLOT(addModel()) );
				menu->addAction( "Add Texture" , this , SLOT(addTexture()) );
				break;
			case CProjectTreeItem::texture:
				//menu->addAction( "Add Stage" );
				return;
				break;
			case CProjectTreeItem::model:
				menu->addAction( "Add Texture" , this , SLOT(addTexture()) );
				break;
			default: {}
		}
	}
	else
	{
		menu->addAction( "Add Stage" , this , SLOT(addStage()) );
	}

	menu->popup( QCursor::pos() , NULL );
}

void QtSE::split( Qt::Orientation orientation )
{
	QTabWidget *tabWidget = tabMap.value( this->focusWidget() , NULL );

	if( tabWidget && tabWidget->count() > 1 )
	{
		QSplitter *tabSplitter = splitterMap.value( tabWidget , NULL );
		VJsonForm *form = tabWidget->currentWidget();

		QTabWidget *newTabWidget = new QTabWidget( NULL );
		newTabWidget->addTab( form , "ey bb" );

		QSplitter *newSplitter = new QSplitter( orientation , NULL );
		newSplitter->addWidget( tabWidget );
		newSplitter->addWidget( newTabWidget );

		tabSplitter->addWidget( newSplitter );

		splitterMap[ tabWidget ] = newSplitter;
		splitterMap[ newTabWidget ] = newSplitter;
		tabMap[ form ] = newTabWidget;
		form->setFocus();
	}
}

void QtSE::splitCollapse( void )
{
	QTabWidget *tabWidget = tabMap.value( this->focusWidget() , NULL );

	if( tabWidget )
	{
		QSplitter *parentSplitter = splitterMap[ tabWidget ];
		QTabWidget *otherTab = NULL;

		// We have an active editor. Find the other tab widget owned by the splitter
		for( QMap< QTabWidget* , QSplitter* >::iterator it = splitterMap.begin() ; it != splitterMap.end() ; it++ )
		{
			if( it.value() == parentSplitter && it.key() != tabWidget )
			{
				otherTab = it.key();
				break;
			}
		}

		if( otherTab )
		{
			// We have the other tab. Add the current tabs to this one
			while( tabWidget->currentWidget() )
			{
				//std::cout << (int)tabMap.value( tabWidget->currentWidget() , NULL ) << std::endl;
				tabMap[ tabWidget->currentWidget() ] = otherTab;
				otherTab->addTab( tabWidget->currentWidget() , "lol" );
			}

			delete tabWidget;
			tabWidget = NULL;
		}

		return;

		/*
		for( int index = 0 ; index < splitterMap.size() ; index++ )
		{
			VJsonForm *form = dynamic_cast< VJsonForm* >( list.at( index ) );

			if( form )
				formList.append( form );

			std::cout << (int)form << std::endl;
		}
		*/
#if 0
		QSplitter *tabSplitter = splitterMap.value( tabWidget , NULL );
		VJsonForm *form = tabWidget->currentWidget();

		QTabWidget *newTabWidget = new QTabWidget( NULL );
		newTabWidget->addTab( tabWidget->currentWidget() , "ey bb" );

		QSplitter *newSplitter = new QSplitter( orientation , NULL );
		newSplitter->addWidget( tabWidget );
		newSplitter->addWidget( newTabWidget );

		tabSplitter->addWidget( newSplitter );

		splitterMap[ tabWidget ] = newSplitter;
		splitterMap[ newTabWidget ] = newSplitter;
		tabMap[ form ] = newTabWidget;
#endif
	}
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
