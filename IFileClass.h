// ============================================================================
// IFileClass & CFileClass
// ============================================================================
// Purpose:   Neatly create classes from various sources
// Example:
//
// class CFileXYZ : public IFileClass< CFileXYZ > ... {}
// IFileClass contains file-type agnostic code to create classes
// CFileClass exists due to QObject not supporting template classes directly
//*****************************************************************************

#ifndef IFILECLASS_H
#define IFILECLASS_H

#include "CPath.h"

#include <QBuffer>
#include <QDataStream>
#include <QFile>
#include <QString>

class CFileClass : public QObject , public CPath
{
	Q_OBJECT

public:
	CFileClass( void ) { title = "untitled"; }

public:
	//inline QString getPath( void ) const { return path; }
	//inline void setPath( const QString &path , bool emitSignal = false ) { emitSignal ? emit pathChanged( this->path , this->path = path ) : (void)( this->path = path ); }
	//inline void setPathEmit( const QString &path ) { emit pathChanged( this->path , this->path = path ); }

	//inline QString getName( void ) const { return name; }
	//inline void setName( const QString &name , bool emitSignal = true ) { emitSignal ? emit nameChanged( this->name , this->name = name ) : (void)( this->name = name ); }
	//inline void setNameEmit( const QString &name ) { emit nameChanged( this->name , this->name = name ); }

	inline QString getTitle( void ) const { return title; }
	inline void setTitle( const QString &title ) { this->title = title; }

protected:
	QString title;

	// Path should include name and extension // File may have non-standard extension
	// Name should never be empty, unless it actually is
	//QString path , name;

signals:
	//void pathChanged( QString pathOld , QString pathNew );
	//void nameChanged( QString pathOld , QString pathNew );
};

template <class T> class IFileClass : public CFileClass
{
protected:
	IFileClass( void ) { /* Nothing to do */ }
	virtual ~IFileClass() { /* Nothing to do */ }

public:
	inline QString errorString( void ) const { return lastError; }

	virtual bool serialize( QDataStream &stream ) = 0;

	virtual bool serialize( QIODevice &device )
	{
		if( device.isOpen() || device.open( QIODevice::WriteOnly ) )
		{
			QDataStream deviceStream( &device );
			return serialize( deviceStream );
		}
		else
			lastError = "Write Error: Unable to open device";

		return false;
	}

	virtual bool serialize( QByteArray &data )
	{
		QBuffer buffer( &data );

		if( buffer.open( QIODevice::WriteOnly ) )
			return serialize( buffer );
		else
			lastError = "Write Error: Unable to open device";

		return false;
	}

	virtual bool serialize( const QString &path )
	{
		if( !path.isEmpty() )
		{
			QFile file( path );

			if( file.open( QIODevice::WriteOnly ) )
				return serialize( file );
			else
				lastError = "Write Error: Unable to open device";
		}
		else
			lastError = "Write Error: Invalid path";

		return false;
	}

protected:
	virtual bool createFromStream( QDataStream &stream ) = 0;

protected:
	mutable QString lastError;

public:
	static T* createFromStream( QDataStream &stream , QString &error )
	{
		T* object = new T;

		if( reinterpret_cast< IFileClass* >( object )->createFromStream( stream ) )
		{
			error = object->lastError;
			return object;
		}
		else
		{
			error = object->lastError;
			delete object;
		}

		return NULL;
	}

	static T* createFromDevice( QIODevice &device , QString &error )
	{
		if( device.isOpen() || device.open( QIODevice::ReadOnly ) )
		{
			QDataStream deviceStream( &device );
			deviceStream.setByteOrder( QDataStream::LittleEndian );
			deviceStream.setFloatingPointPrecision( QDataStream::SinglePrecision );

			return IFileClass::createFromStream( deviceStream , error );
		}
		else
			error = "Read Error: Unable to open device";

		return NULL;
	}

	static T* createFromDevice( QIODevice &device ) { QString dummy; return IFileClass::createFromDevice( device , dummy ); }

	static T* createFromData( QByteArray &data , QString &error )
	{
		QBuffer buffer( &data );

		if( buffer.open( QIODevice::ReadOnly ) )
			return IFileClass::createFromDevice( buffer , error );
		else
			error = "Read Error: Unable to open device";

		return NULL;
	}

	static T* createFromData( QByteArray &data ) { QString dummy; return IFileClass::createFromData( data , dummy ); }

	static T* createFromFile( const QString &path , QString &error )
	{
		QFile file( path );

		if( file.exists() )
		{
			if( file.open( QIODevice::ReadOnly ) )
			{
				T *object = IFileClass::createFromDevice( file , error );

				if( object )
				{
					reinterpret_cast< IFileClass* >( object )->setPath( path , true );
					reinterpret_cast< IFileClass* >( object )->title = reinterpret_cast< IFileClass* >( object )->getName( false );
				}

				return object;
			}
			else
				error = "Read Error: Unable to open device";
		}
		else
			error = "Read Error: File does not exist";

		return NULL;
	}

	static T* createFromFile( const QString &path ) { QString dummy; return IFileClass::createFromFile( path , dummy ); }
};

#endif // IFILECLASS_H
