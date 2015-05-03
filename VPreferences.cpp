#include "VPreferences.h"

VPreferences::VPreferences( QWidget *parent /* = NULL */ )
{
	QSettings settings( QSettings::IniFormat , QSettings::UserScope , QCoreApplication::organizationName() , QCoreApplication::applicationName() );

	loadLastProjectOnStart = new QCheckBox( "Load last project on start" , NULL );
	loadLastProjectOnStart->setChecked( settings.value( "settings/loadLastProjectOnStart" , true ).toBool() );
	refreshSceneOnSave = new QCheckBox( "Refresh scene on save" , NULL );
	refreshSceneOnSave->setChecked( settings.value( "settings/refreshSceneOnSave" , false ).toBool() );

	generalLayout = new QVBoxLayout( NULL );
	generalLayout->addWidget( loadLastProjectOnStart , 0 , Qt::AlignTop );
	generalLayout->addWidget( refreshSceneOnSave , 0 , Qt::AlignTop );

	generalBox = new QGroupBox( "General Settings" , NULL );
	delete generalBox->layout();
	generalBox->setLayout( generalLayout );

	logToFile = new QCheckBox( "Log to file" , NULL );
	logToFile->setChecked( settings.value( "logger/logToFile" , true ).toBool() );

	clearLogFileOnStart = new QCheckBox( "Clear log file on start" , NULL );
	clearLogFileOnStart->setChecked( settings.value( "logger/clearLogFileOnStart" , true ).toBool() );

	alwaysScrollToEndOnAppend = new QCheckBox( "Always scroll to end on append" , NULL );
	alwaysScrollToEndOnAppend->setChecked( settings.value( "logger/alwaysScrollToEndOnAppend" , true ).toBool() );

	logLayout = new QVBoxLayout( NULL );
	logLayout->addWidget( logToFile , 0 , Qt::AlignTop );
	logLayout->addWidget( clearLogFileOnStart , 0 , Qt::AlignTop );
	logLayout->addWidget( alwaysScrollToEndOnAppend , 0 , Qt::AlignTop );

	logBox = new QGroupBox( "Log Settings" , NULL );
	delete logBox->layout();
	logBox->setLayout( logLayout );

	buttonBox = new QDialogButtonBox( QDialogButtonBox::Save | QDialogButtonBox::Cancel );

	connect( buttonBox , SIGNAL(accepted()) , this , SLOT(wasAccepted()) );
	connect( buttonBox , SIGNAL(rejected()) , this , SLOT(reject()) );

	windowLayout = new QVBoxLayout( NULL );
	windowLayout->addWidget( generalBox , 0 , Qt::AlignTop );
	windowLayout->addWidget( logBox , 0 , Qt::AlignTop );
	windowLayout->addStretch();
	windowLayout->addWidget( buttonBox , 0 , Qt::AlignBottom );

	//this->setLayout();
	this->setLayout( windowLayout );

	this->setWindowTitle( "Preferences" );
	this->setModal( true );
	this->setAttribute( Qt::WA_DeleteOnClose , true );
	//this->setFixedSize( this->width() , this->height() );
	this->setFixedSize( 400 , 500 );
}

VPreferences::~VPreferences()
{
	// Nothing to do
}

void VPreferences::wasAccepted( void )
{
	QSettings settings( QSettings::IniFormat , QSettings::UserScope , QCoreApplication::organizationName() , QCoreApplication::applicationName() );

	settings.setValue( "settings/loadLastProjectOnStart" , loadLastProjectOnStart->isChecked() );
	settings.setValue( "settings/refreshSceneOnSave" , refreshSceneOnSave->isChecked() );

	settings.setValue( "logger/logToFile" , logToFile->isChecked() );
	settings.setValue( "logger/clearLogFileOnStart" , clearLogFileOnStart->isChecked() );
	settings.setValue( "logger/alwaysScrollToEndOnAppend" , alwaysScrollToEndOnAppend->isChecked() );

	this->accept();
}
