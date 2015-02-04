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

	QTreeWidgetItem *dummyFB = new QTreeWidgetItem( projectTree , QStringList( "Framebuffer 1" ) );
	QTreeWidgetItem *dummyModel = new QTreeWidgetItem( dummyFB  , QStringList( "Model 1" ) );
	QTreeWidgetItem *dummyShader = new QTreeWidgetItem( dummyModel , QStringList( "Shader 1" ) );
	projectTree->expandAll();

	itemsTab = new QTabWidget( NULL );
	itemsTab->addTab( projectTree , "Project" );
	itemsTab->setTabPosition( QTabWidget::East );
	itemsTab->setTabsClosable( false );

	QTextEdit *textEdit= new QTextEdit( NULL );
	textTab = new QTabWidget( NULL );
	textTab->addTab( textEdit , "File 1" );

	windowSplitter = new QSplitter( NULL );
	windowSplitter->addWidget( itemsTab );
	windowSplitter->addWidget( textTab );

	this->setCentralWidget( windowSplitter );
	textTab->setFocus();
}

QtSE::~QtSE()
{
	//
}
