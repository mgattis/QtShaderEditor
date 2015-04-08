#ifndef VLOGGER_H
#define VLOGGER_H

#include "StdRedirector.h"
#include "qtil.h"

#include <QCoreApplication>
#include <QFile>
#include <QScrollBar>
#include <QTextEdit>

class VLogger : public QTextEdit
{
public:
	enum stream
	{
		cout,
		cerr,
		clog
	};

public:
	VLogger( QWidget *parent );
	~VLogger();

public:
	inline void coutAppend( const char *data , std::streamsize count = -1 ) { coutAppend( QByteArray(  data , count ) ); }
	inline void coutAppend( const QByteArray &data ) { coutAppend( QString( data ) ); }
	inline void coutAppend( const QString &string ) { stdAppend( string , cout ); }

	inline void cerrAppend( const char *data , std::streamsize count = -1 ) { cerrAppend( QByteArray(  data , count ) ); }
	inline void cerrAppend( const QByteArray &data ) { cerrAppend( QString( data ) ); }
	inline void cerrAppend( const QString &string ) { stdAppend( string , cerr ); }

	inline void clogAppend( const char *data , std::streamsize count = -1 ) { clogAppend( QByteArray(  data , count ) ); }
	inline void clogAppend( const QByteArray &data ) { clogAppend( QString( data ) ); }
	inline void clogAppend( const QString &string ) { stdAppend( string , clog ); }

	void stdAppend( const QString &string , stream type );

protected:
	void resizeEvent( QResizeEvent *event );

protected:
	StdRedirector<> *coutRedirector;
	StdRedirector<> *cerrRedirector;
	StdRedirector<> *clogRedirector;

	bool logToFile;
	QFile logFile;

public slots:
	void scrollToLatest( void );

protected:
	static void callbackHelper( const char* ptr , std::streamsize count , void *target , stream type )
	{
		// Buffer that keeps data until newline
		static QByteArray coutData;
		static QByteArray cerrData;
		static QByteArray clogData;

		VLogger *logger = (VLogger*)target;
		QByteArray *data = NULL;
		void (VLogger::*append)( const QByteArray& ) = NULL;

		// Set what to use
		switch( type )
		{
			case cout:
				data = &coutData;
				append = &VLogger::coutAppend;
				break;
			case cerr:
				data = &cerrData;
				append = &VLogger::cerrAppend;
				break;
			case clog:
				data = &clogData;
				append = &VLogger::clogAppend;
				break;
			default:
				return;
		}

		data->append( ptr , count );

		int index = -1;
		while( ( index = data->lastIndexOf( '\n' ) ) != -1 )
		{
			index++;
			(logger->*append)( data->mid( 0 , index ) ); // I love C++
			*data = data->mid( index );
		}
	}

	static void coutCallback( const char* ptr , std::streamsize count , void *target )
	{
		callbackHelper( ptr , count , target , cout );
	}

	static void cerrCallback( const char* ptr , std::streamsize count , void *target )
	{
		callbackHelper( ptr , count , target , cerr );
	}

	static void clogCallback( const char* ptr , std::streamsize count , void *target )
	{
		callbackHelper( ptr , count , target , clog );
	}
};

#endif // VLOGGER_H
