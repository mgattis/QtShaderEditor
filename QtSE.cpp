#include "QtSE.h"

QtSE::QtSE( QWidget *parent ) : QMainWindow( parent )
{
	menuFile = this->menuBar()->addMenu( "File" );
		actionQuit = menuFile->addAction( "Quit" , this , SLOT(close()) );

	menuHelp = this->menuBar()->addMenu( "Help" );
		actionAbout = menuHelp->addAction( "About" , this , SLOT(close()) );

	projectTree = new QTreeWidget( NULL );
	projectTree->header()->setStretchLastSection( true );
	projectTree->header()->setHidden( true );

	viewWidget = new VGLView( NULL );

	QTreeWidgetItem *dummyFB = new QTreeWidgetItem( projectTree , QStringList( "Framebuffer 1" ) );
	QTreeWidgetItem *dummyModel = new QTreeWidgetItem( dummyFB  , QStringList( "Model 1" ) );
	QTreeWidgetItem *dummyShader = new QTreeWidgetItem( dummyModel , QStringList( "Shader 1" ) );
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

#if 0
	QWidget *window = new QWidget( NULL );
	window->setAttribute( Qt::WA_DeleteOnClose );
	window->show();
#endif

#if 1
	QTextEdit *textEdit = new QTextEdit( NULL );

	initialTab = new QTabWidget( NULL );
	initialTab->setTabsClosable( true );
	initialTab->setMovable( true );
	initialTab->addTab( textEdit , "Shader 1" );
#endif

	windowSplitter = new QSplitter( NULL );
	windowSplitter->addWidget( viewSplitter );
	windowSplitter->setStretchFactor( 0 , 1 );
	windowSplitter->addWidget( initialTab );
	windowSplitter->setStretchFactor( 1 , 4 );

	this->setCentralWidget( windowSplitter );
	initialTab->setFocus();
}

QtSE::~QtSE()
{
	// Nothing to do
}
