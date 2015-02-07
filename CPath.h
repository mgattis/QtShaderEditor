#ifndef CPATH_H
#define CPATH_H

#include <qtil.h>

#include <QMessageBox>
#include <QString>

/*
Example	: /media/Stuff/home/user/Projects/QtSE/release/QtSE.tar.gz
path	: /media/Stuff/home/user/Projects/QtSE/release
basename: QtSE
name	: QtSE.tar
fullExt	: tar.gz
ext		: gz
*/

class CPath
{
public:
	CPath( const QString &path = QString() , bool lastTokenIsName = true );

public:
	inline bool isDirectory( void ) const { return !path.isEmpty() && name.isEmpty(); }
	inline bool isEmpty( void ) const { return path.isEmpty() && name.isEmpty() && fullExtension.isEmpty(); }
	inline bool isFile( void ) const { return !name.isEmpty(); }

	QString getFullPath( void ) const;

	QString getPath( bool addTrailingSlash ) const;
	//QString getPathRelativeTo( const QString &relativePath ) const;
	void setPath( const QString &path , bool lastTokenIsName = false );

	QString getName( bool addFullExtension ) const;
	void setName( const QString &name );

	inline QString getFullExtension( void ) const { return fullExtension; }
	void setFullExtension( const QString &fullExtension );

	inline QString getExtension( void ) const { return extension; }
	void setExtension( const QString &extension );

	inline void clear( void ) { path = name = fullExtension = extension = ""; }

	CPath toCaseSensitivePath( const QString &basePath = QString() , bool includeBasePath = true ) const;

protected:
	QString path , name , fullExtension , extension;
};

#endif // CPATH_H
