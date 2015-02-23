#include "VTabEditor.h"

VTabEditor::VTabEditor( QWidget *parent /* = NULL */ ) : QWidget( parent )
{
	tabBar = new QTabBar( NULL );
	tabBar->setSizePolicy( QSizePolicy::Ignored , QSizePolicy::Minimum );
	tabBar->addTab( "lol" );
	tabBar->setMovable( true );

	buttonSplit = new QPushButton( "[]<->[]" );
	buttonCloseSplit = new QPushButton( "->[]<-" );
	buttonClose = new QPushButton( "X" );

	buttonLayout = new QHBoxLayout( NULL );
	buttonLayout->setSpacing( 0 );
	buttonLayout->setMargin( 0 );
	//buttonLayout->addWidget( buttonSplit );
	//buttonLayout->addWidget( buttonCloseSplit );
	//buttonLayout->addWidget( buttonClose );

	QTextEdit *widget = new QTextEdit( NULL );
	stackedWidget = new QStackedWidget( NULL );
	stackedWidget->setSizePolicy( QSizePolicy::Ignored , QSizePolicy::Ignored );
	stackedWidget->addWidget( widget );
	stackedWidget->setCurrentIndex( 0 );

	windowLayout = new QGridLayout( NULL );
	windowLayout->setSpacing( 0 );
	windowLayout->setMargin( 0 );
	windowLayout->addWidget( tabBar , 0 , 0 );
	windowLayout->addLayout( buttonLayout , 0 , 1 );
	windowLayout->addWidget( stackedWidget , 1 , 0 , 1 , 2 );

	delete this->layout();
	this->setLayout( windowLayout );
}

VTabEditor::~VTabEditor()
{
	// Nothing to do
}

void VTabEditor::addTab( QWindow *window )
{
	//
}
