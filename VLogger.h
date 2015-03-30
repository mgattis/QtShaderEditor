#ifndef VLOGGER_H
#define VLOGGER_H

#include "StdRedirector.h"

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
	static void coutCallback( const char* ptr , std::streamsize count , void *target )
	{
		VLogger *logger = (VLogger*)target;
		logger->coutAppend( ptr , count );
	}

	static void cerrCallback( const char* ptr , std::streamsize count , void *target )
	{
		VLogger *logger = (VLogger*)target;
		logger->cerrAppend( ptr , count );
	}

	static void clogCallback( const char* ptr , std::streamsize count , void *target )
	{
		VLogger *logger = (VLogger*)target;
		logger->clogAppend( ptr , count );
	}
};

#endif // VLOGGER_H
