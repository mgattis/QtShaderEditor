// ============================================================================
// Qt Utility Code
// ============================================================================
// Purpose: Collection of randomly useful code
//*****************************************************************************

#ifndef QTIL_H
#define QTIL_H

#include "util.h"

#include <QDataStream>
#include <QDir>
#include <QFile>
#include <QFileIconProvider>
#include <QFileInfo>
#include <QFontMetrics>
#include <QIcon>
#include <QString>
#include <QTemporaryFile>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QVector>
#include <QWidget>

#include <QMessageBox>

#define REGEX_SLASH_PATTERN	"\\\\|/"

// Poor man's debugger

#define DEBUGGER( fileName ) \
	QFile debugFile( fileName ); \
	debugFile.open( QFile::WriteOnly | QFile::Append | QFile::Text );

#define WRITE( message ) \
	if( debugFile.isOpen() ) \
	{ \
		debugFile.write( message ); \
		debugFile.flush(); \
	}

// Removes slashes and whitespace from both ends of a string
QString UTIL_trimFilePath( const QString &constPath );
void UTIL_trimFilePath( QString &fixedPath );

#ifndef QTIL2_H
#define QTIL2_H

inline QString UTIL_pathSanitize( const QString &path ) { return QString( path ).replace( QRegExp( "[\\\\|/]+" ) , "/" ); }

// Converts back slashes to forward slashes
inline QString UTIL_filePathSlashToForward( const QString &constPath ) { return UTIL_pathSanitize( constPath ).replace( '\\' , '/' ); }
inline void UTIL_filePathSlashToForward( QString &path ) { path = UTIL_pathSanitize( path ).replace( '\\' , '/' ); }

// Converts forward slashes to back slashes
inline QString UTIL_filePathSlashToBackwards( const QString &constPath ) { return UTIL_pathSanitize( constPath ).replace( '/' , '\\' ); }
inline void UTIL_filePathSlashToBackwards( QString &path ) { UTIL_pathSanitize( path ).replace( '/' , '\\' ); }

#endif

// Convenience function where '/' and '\\' are considered the same and letter case is ignored
bool UTIL_compareFilePath( const QString &pathA , const QString &pathB );

// Reads a line of text
QByteArray UTIL_readLineFromQIODevice( QIODevice &device );

// Reads a null-terminated character array from a QDataStream
QString UTIL_readCStringFromQDataStream( QDataStream &stream , char delimiter = '\0' , unsigned short reserveLength = 128 );

// Floating point value to QString // Removes trailing zeros
QString UTIL_standardNotationFloat( double value , int convertPrecision /* = -1 */ );

// Splits a keyvalue string into component parts
// Returns false if line is empty, or only a key is present
bool UTIL_getPart( const QString &line , QString &key , QString &value );

// Similar to UTIL_getPart
// Using the magical regex pattern ".*?"|//.*?$|(\S)+
// Should only return three strings
// NOTE: Since using ".*?"|//.*?$|(\S)+ instead of ".*?"|(\S)+
// beware of non quote enclosed keys or values that start with //
QStringList UTIL_getParts( const QString &line );

// Nullifies input strings for later differentiation
// between empty and non-existant
void UTIL_getParts( const QString &line , QString &key , QString &value , QString &comment /* = QString() */ );

// To bypass comment requirement when unnecessary
void UTIL_getParts( const QString &line , QString &key , QString &value );
void UTIL_getParts( const QByteArray &line , QByteArray &key , QByteArray &value );

// Centers a window over something else
void UTIL_centerOverWidget( QWidget *topWidget , const QWidget *bottomWidget );
inline void UTIL_centerOverParent( QWidget *widget ) { UTIL_centerOverWidget( widget , widget ? widget->parentWidget() : NULL ); } // ?: for inline

// Copy a number of bytes from one device to another
bool UTIL_deviceCopy( QIODevice &from , QIODevice &to , quint64 bytes , QString *lastError );

#if 0
bool UTIL_validateDevice( const QIODevice *device , QIODevice::OpenMode openMode )
{
	if( device )
	{
		if( device->openMode() == openMode )
		{
			//
		}
	}

	return NULL;
}
#endif

// Hack to get around QFileIconProvider's inability to get
// icons (by extension) of files not existing on disk
// DEPRECIATED: Use QIconProvider
QIcon UTIL_shellIconForExtension( const QString &extension );

// QFile can't create directories that don't exist
// Path is taken to be absolute
bool UTIL_makeDirectoriesForPath( const QString &path , bool lastSplitIsFile );

int UTIL_getFontWidth( const QString &string );

void UTIL_cleanFilePath( QByteArray &path );
void UTIL_cleanFilePath( QString &path );

bool UTIL_writeBytes( const QByteArray &data , QIODevice &device , unsigned int bytes );

QString UTIL_groupDigits( quint64 number );

QString UTIL_extensionForFileDialogFilter( const QString &filter );

// QTreeWidget can't expand all children of a specific item alone
void UTIL_expandTreeItems( QTreeWidget *tree , QTreeWidgetItem *item );

// Taken from:
// https://qt.gitorious.org/qt-creator/qt-creator/source/4df7656394bc63088f67a0bae8733f400671d1b6%3asrc/libs/utils/filenamevalidatinglineedit.cpp
bool UTIL_validateFileName( const QString &path , bool allowDirectories );


#ifndef UTIL_REMOVEDUPLICATESQLIST_H
#define UTIL_REMOVEDUPLICATESQLIST_H

// Items must have an implementation of operator==
template <class T> void UTIL_removeDuplicates( QList<T> &list )
{
	for( int baseIndex = 0 ; baseIndex < list.length() ; baseIndex++ )
		for( int cmpIndex = list.length() - 1 ; cmpIndex > baseIndex ; cmpIndex-- )
			if( list.at( cmpIndex ) == list.at( baseIndex ) )
				list.removeAt( cmpIndex );
}

#endif // UTIL_REMOVEDUPLICATESQLIST_H

#ifndef UTIL_REMOVEDUPLICATESQVECTOR_H
#define UTIL_REMOVEDUPLICATESQVECTOR_H

// Items must have an implementation of operator==
template <class T> void UTIL_removeDuplicates( QVector<T> &vector )
{
	for( int baseIndex = 0 ; baseIndex < vector.size() ; baseIndex++ )
		for( int cmpIndex = vector.size() - 1 ; cmpIndex > baseIndex ; cmpIndex-- )
			if( vector.at( cmpIndex ) == vector.at( baseIndex ) )
				vector.remove( cmpIndex );
}

#endif // UTIL_REMOVEDUPLICATESQVECTOR_H

#ifndef UTIL_DELETEPOINTERQLIST_H
#define UTIL_DELETEPOINTERQLIST_H

// List must contain pointers
// If list is dereferenced before passing, check if NULL
// This function assumes otherwise
template <class T> void UTIL_deleteListAsPointerArray( QList<T> &list )
{
	UTIL_removeDuplicates( list );

	for( int index = 0 ; index < list.length() ; index++ )
		delete list.at( index );
}

#endif // UTIL_DELETEPOINTERQLIST_H

#ifndef UTIL_DELETEPOINTERQVECTOR_H
#define UTIL_DELETEPOINTERQVECTOR_H

// List must contain pointers
// If list is dereferenced before passing, check if NULL
// This function assumes otherwise
template <class T> void UTIL_deleteVectorAsPointerArray( QVector<T> &vector )
{
	UTIL_removeDuplicates( vector );

	for( int index = 0 ; index < vector.size() ; index++ )
		delete vector.at( index );
}

#endif // UTIL_DELETEPOINTERQVECTOR_H

#endif // QTIL_H
