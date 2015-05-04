#include "VNewProjectDialog.h"

VNewProjectDialog::VNewProjectDialog( QWidget *parent /* = NULL */ )
{
	QSettings settings( QSettings::IniFormat , QSettings::UserScope , QCoreApplication::organizationName() , QCoreApplication::applicationName() );

	projectNameLabel = new QLabel( "Project Name:" , NULL );
	projectNameEdit = new QLineEdit( NULL );
	projectPathLabel = new QLabel( "Project Path:" , NULL );
	projectPathEdit = new QLineEdit( settings.value( "settings/defaultProjectPath" , qApp->applicationDirPath() ).toString() , NULL );
	projectPathButton = new QPushButton( "Browse..." , NULL );
	projectDefaultPathCheck = new QCheckBox( "Use as default project location" , NULL );
	projectDefaultPathCheck->setChecked( settings.contains( "settings/defaultProjectPath" ) );
	connect( projectPathButton , SIGNAL(clicked()) , this , SLOT(browse()) );

	infoLayout = new QGridLayout;
	infoLayout->addWidget( projectNameLabel , 0 , 0 );
	infoLayout->addWidget( projectNameEdit , 0 , 1 , 1 , 2 );
	infoLayout->addWidget( projectPathLabel , 1 , 0 );
	infoLayout->addWidget( projectPathEdit , 1 , 1 );
	infoLayout->addWidget( projectPathButton , 1 , 2 );
	infoLayout->addWidget( projectDefaultPathCheck , 2 , 1 );

	infoBox = new QGroupBox( "New Project" , NULL );
	delete infoBox->layout();
	infoBox->setLayout( infoLayout );

	buttonBox = new QDialogButtonBox( QDialogButtonBox::Ok | QDialogButtonBox::Cancel );
	connect( buttonBox , SIGNAL(accepted()) , this , SLOT(wasAccepted()) );
	connect( buttonBox , SIGNAL(rejected()) , this , SLOT(reject()) );

	windowLayout = new QVBoxLayout( NULL );
	windowLayout->addWidget( infoBox , 0 , Qt::AlignTop );
	windowLayout->addStretch();
	windowLayout->addWidget( buttonBox , 0 , Qt::AlignBottom );

	this->setLayout( windowLayout );

	this->setWindowTitle( "New Project - QtSE" );
	this->setModal( true );
	//this->setAttribute( Qt::WA_DeleteOnClose , true );
	this->adjustSize();
	this->setFixedSize( 800 , this->height() );
}

VNewProjectDialog::~VNewProjectDialog()
{
	//
}

QString VNewProjectDialog::getProjectPath( void )
{
	QDir dir;

	if( dir.cd( projectPathEdit->text() ) && dir.mkdir( projectNameEdit->text() ) )
		return dir.absolutePath();

	return "";
}

void VNewProjectDialog::browse( void )
{
	QString path = QFileDialog::getExistingDirectory( this , "Browse" , projectPathEdit->text() );

	if( !path.isEmpty() )
		projectPathEdit->setText( path );
}

void VNewProjectDialog::wasAccepted( void )
{
	QDir dir;

	if( !projectNameEdit->text().isEmpty() )
	{
		if( !projectPathEdit->text().isEmpty() && dir.cd( projectPathEdit->text() ) )
		{
			if( !dir.exists( projectNameEdit->text() ) )
			{
				if( projectDefaultPathCheck->isChecked() )
				{
					QSettings settings( QSettings::IniFormat , QSettings::UserScope , QCoreApplication::organizationName() , QCoreApplication::applicationName() );
					settings.setValue( "settings/defaultProjectPath" , projectPathEdit->text() );
				}

				this->accept();
			}
			else
				QMessageBox::warning( this , this->windowTitle() , "Project already exists." );
		}
		else
			QMessageBox::warning( this , this->windowTitle() , "Invalid project path." );
	}
	else
		QMessageBox::warning( this , this->windowTitle() , "No project name given." );
}
