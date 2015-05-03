#include "VLogger.h"

VLogger::VLogger( QWidget *parent ) : QTextEdit( parent )
{
	QSettings settings( QSettings::IniFormat , QSettings::UserScope , QCoreApplication::organizationName() , QCoreApplication::applicationName() );

	clearLogfileOnStart = false;
	logToFile = false;
	alwaysScrollToEndOnAppend = false;

	QFont font;
	font.setFamily( "monospace" );
	font.setFixedPitch( true );
	font.setPointSize( 9 );

	this->setFont( font );
	this->setTabStopWidth( 4 * UTIL_getFontWidth( " " , font ) );

	logToFile = true;
	logFile.setFileName( QCoreApplication::applicationDirPath() + "/dia.log" );
	logFile.open( QIODevice::WriteOnly | ( settings.value( "logger/clearLogFileOnStart" , true ).toBool() ?  QIODevice::Truncate : QIODevice::NotOpen ) );
	logStream.setDevice( &logFile );

	this->setReadOnly( true );
	this->setWordWrapMode( QTextOption::NoWrap );

	// Override stdout
	pipe( &outTube[ 0 ] );
	fflush( stdout );
	fgetpos( stdout , &oldStdoutPos );
	oldStdoutFd = dup( fileno( stdout ) );
	dup2( outTube[ 1 ] , fileno( stdout ) );

	// Override stderr
	pipe( &errTube[ 0 ] );
	fflush( stderr );
	fgetpos( stderr , &oldStderrPos );
	oldStderrFd = dup( fileno( stderr ) );
	dup2( errTube[ 1 ] , fileno( stderr ) );

	// Override C++ streams
	coutRedirector = new StdRedirector<>( std::cout , coutCallback , this );
	cerrRedirector = new StdRedirector<>( std::cerr , cerrCallback , this );
	clogRedirector = new StdRedirector<>( std::clog , clogCallback , this );
}

VLogger::~VLogger()
{
	// Restore stdout
	fflush( stdout );
	dup2( oldStdoutFd , fileno( stdout ) );
	::close( oldStdoutFd );
	clearerr( stdout );
	fsetpos( stdout , &oldStdoutPos ); /* for C9X */
	::close( outTube[ 1 ] );

	// Restore stderr
	fflush( stderr );
	dup2( oldStderrFd , fileno( stderr ) );
	::close( oldStderrFd );
	clearerr( stderr );
	fsetpos( stderr , &oldStderrPos ); /* for C9X */
	::close( errTube[ 1 ] );

	// Restore the streams
	//delete coutRedirector;
	//delete cerrRedirector;
	//delete clogRedirector;
}

void VLogger::resizeEvent( QResizeEvent *event )
{
	scrollToLatest();

	QTextEdit::resizeEvent( event );
}

void VLogger::stdAppend( const QString &string , stream type )
{
	// Log to file

#if 0
	if( logToFile && ( logFile.isOpen() || logFile.open( QIODevice::Truncate | QIODevice::WriteOnly ) ) )
	{
		switch( type )
		{
			case cout:
				logFile.write( "Output: " );
				break;
			case cerr:
				logFile.write( "Error: " );
				break;
			case clog:
				logFile.write( "Log: " );
				break;
			default: {}
		}

		logFile.write( QByteArray( string.data() , string.size() ) );
		logFile.flush();
	}
#endif

	// Log to self

	QString info( string );
	info = info.trimmed();

	if( !info.isEmpty() )
	{
		QString prefix;

		switch( type )
		{
			case cout:
				this->setTextColor( Qt::black );
				prefix = "[OUT] ";
				break;
			case cerr:
				this->setTextColor( Qt::red );
				prefix = "[ERR] ";
				break;
			case clog:
				this->setTextColor( Qt::blue );
				prefix = "[LOG] ";
				break;
			default: {}
		}

		this->append( QString( info ) );
		scrollToLatest();

		if( logToFile && ( logFile.isOpen() || logFile.open( QIODevice::WriteOnly ) ) )
		{
			QStringList strings = info.split( '\n' );

			for( int index = 0 ; index < strings.size() ; index++ )
				logStream << prefix << strings.at( index ) << "\n";
		}

		logStream.flush();
	}
}

void VLogger::flushStandardStreams( void )
{
	// Buffer that keeps data until newline
	static QByteArray outData;
	static QByteArray errData;
	static char buf[ 1024 ] = { 0 };

	int count = 0;
	struct pollfd poller;

	// stdout
	poller.fd = outTube[ 0 ];
	poller.events = POLLIN;

	// Gather data
	fflush( stdout );
	while( poll( &poller , 1 , 0 ) == 1 )
	{
		count = read( outTube[ 0 ] , &buf[ 0 ] , 1023 );

		if( count > 0 )
			outData.append( buf, count );
	}

	// Print data
	count = outData.lastIndexOf( '\n' );
	if( count != -1 )
	{
		count++;
		coutAppend( outData.mid( 0 , count ) );
		outData = outData.mid( count );
	}

	// stderr
	poller.fd = errTube[ 0 ];
	poller.events = POLLIN;

	// Gather data
	fflush( stderr );
	while( poll( &poller , 1 , 0 ) == 1 )
	{
		count = read( errTube[ 0 ] , &buf[ 0 ] , 1023 );

		if( count > 0 )
			errData.append( buf , count );
	}

	// Print data
	count = errData.lastIndexOf( '\n' );
	if( count != -1 )
	{
		count++;
		cerrAppend( errData.mid( 0 , count ) );
		errData = errData.mid( count );
	}
}

void VLogger::scrollToLatest( void )
{
	if( alwaysScrollToEndOnAppend || ( this->verticalScrollBar()->sliderPosition() == this->verticalScrollBar()->maximum() ) )
	{
		QScrollBar *bar = this->horizontalScrollBar();
		bar->setSliderPosition( bar->minimum() );

		bar = this->verticalScrollBar();
		bar->setSliderPosition( bar->maximum() );
	}
}
