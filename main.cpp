#include "CJsonTemplate.h"
#include "QtSE.h"

#include <QApplication>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <QStringList>
#include <QSettings>

#if 0

#include <cstdio>
#include <fstream>
#include <unistd.h>
#include <stdio.h>
#include <poll.h>
#include <QTextStream>
#define _GNU_SOURCE             /* See feature_test_macros(7) */
#include <fcntl.h>              /* Obtain O_* constant definitions */
#include <ext/stdio_filebuf.h>
#include <sstream>
#include <iostream>
#include <fstream>
#include "StdRedirector.h"

char buf2[ 1024 ] = { 0 };

static void dummyPrint( const char* ptr , std::streamsize count , void *target )
{
	std::cerr << ptr << std::endl;
	//memcpy( buf2 , ptr , count );
}

int main( int argc , char *argv[] )
{
	int    fd;
	fpos_t pos;
	char buf[ 1024 ] = { 0 };
	std::string string;
	int tube[ 2 ];

	pipe( &tube[ 0 ] );

	printf("stdout, ");

	// Override stdout
	fflush(stdout);
	fgetpos(stdout, &pos);
	fd = dup(fileno(stdout));
	dup2( tube[ 1 ] , fileno( stdout ) );

	__gnu_cxx::stdio_filebuf< char > filebuf( tube[ 0 ] , std::ios::in );
	//std::istream is( &filebuf );
	std::stringstream is( string );
	( (std::istream*)( &is ) )->rdbuf( &filebuf );
	StdRedirector<> redir( is , dummyPrint , NULL );

	printf("testout1\n");
	fflush( stdout );
	//is.write( "testout2\n" , 8 );
	//is.flush();
	//read( tube[ 0 ] , &buf[ 0 ] , 1023 );
	//is.read( &buf2[ 0 ] , 8 );

	struct pollfd poller;
	poller.fd = tube[ 0 ];
	poller.events = POLLIN;

	if( poll( &poller , 1 , 0 ) == 1 )
	{
		read( tube[ 0 ] , &buf[ 0 ] , 1023 );
		/* data available */
	}

	// Restore stdout
	fflush(stdout);
	dup2(fd, fileno(stdout));
	close(fd);
	clearerr(stdout);
	fsetpos(stdout, &pos);        /* for C9X */

	//printf( "Error: %d\n" , error );
	printf( "\nWritten1: %s\n" , buf );
	printf( "Written2: %s\n" , buf2 );
	//printf( "\nWritten: %s\n" , string.data() );
}

#else

int main( int argc , char *argv[] )
{
	QApplication a( argc , argv );

	QCoreApplication::setOrganizationName( "Riintouge" );
	QCoreApplication::setApplicationName( "QtSE" );

    QtSE w;
	w.show();

    return a.exec();
}

#endif
