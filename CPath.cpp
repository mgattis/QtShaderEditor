#include "CPath.h"

CPath::CPath( const QString &path /* = QString() */ , bool lastTokenIsName /* = true */ )
{
    setPath( path , lastTokenIsName );

    //QMessageBox::information( NULL , "" , this->path + "\n" + name + "\n" + fullExtension + "\n" + extension );
}

QString CPath::getFullPath( void ) const
{
    QString fullPath;

    if( !path.isEmpty() )
    {
        fullPath += path;

        if( !name.isEmpty() )
            fullPath += "/";
    }

    if( !name.isEmpty() )
    {
        fullPath += name;

        if( !fullExtension.isEmpty() )
            fullPath += "." + fullExtension;
    }

    return fullPath;
}

QString CPath::getPath( bool addTrailingSlash ) const
{
    if( !path.isEmpty() )
    {
        if( addTrailingSlash )
            return path + "/";
        else
            return path;
    }

    return QString();
}

#if 0
QString CPath::getPathRelativeTo( const QString &relativePath ) const
{
    QStringList relativeTokens = UTIL_filePathSlashToForward( relativePath ).split( '/' , QString::SkipEmptyParts );
    QStringList tokens = UTIL_filePathSlashToForward( (const QString)( path ) ).split( '/' , QString::SkipEmptyParts );

    while( relativeTokens.length() && tokens.length() && !relativeTokens.first().compare( tokens.first() , Qt::CaseInsensitive ) )
    {
        relativeTokens.removeFirst();
        tokens.removeFirst();
    }

    return "";
}
#endif

void CPath::setPath( const QString &path , bool lastTokenIsName /* = false */ )
{
    QString string = UTIL_filePathSlashToForward( path );

    if( string.endsWith( '/' ) )
    {
        if( string.length() > 1 )
            string.chop( 1 );

        this->path = string;
    }
    else if( lastTokenIsName )
    {
        int index = string.lastIndexOf( '/' );

        if( index != -1 )
        {
            this->path = string.left( index );
            setName( string.mid( index + 1 ) );
        }
        else
            setName( string );
    }
    else
        this->path = string;
}

QString CPath::getName( bool addFullExtension ) const
{
    if( !name.isEmpty() )
    {
        if( addFullExtension )
            return name + "." + fullExtension;
        else
            return name;
    }

    return QString();
}

void CPath::setName( const QString &name )
{
    QString string = UTIL_filePathSlashToForward( name );
    int index = string.lastIndexOf( '/' );

    if( index != -1 )
        string = string.mid( index + 1 );

    index = name.indexOf( '.' , 1 );

    if( index != -1 )
    {
        this->name = string.left( index );
        setFullExtension( string.mid( index + 1 ) );
    }
    else
        this->name = string;
}

void CPath::setFullExtension( const QString &fullExtension )
{
    if( !name.isEmpty() )
    {
        this->fullExtension = fullExtension;

        int index = fullExtension.lastIndexOf( '.' );

        extension = index != -1 ? fullExtension.mid( index + 1 ) : fullExtension;
    }
}

void CPath::setExtension( const QString &extension )
{
    fullExtension.chop( this->extension.length() );
    fullExtension.append( this->extension = extension );
}

CPath CPath::toCaseSensitivePath( const QString &basePath /* = QString() */ , bool includeBasePath /* = true */ ) const
{
    //if( basePath.length() ? QDir( basePath ).isAbsolute() : ( path.length() && QDir( path ).isAbsolute() ) )
    QDir pathfinder( "/" );
    pathfinder.setFilter( QDir::Dirs | QDir::NoSymLinks | QDir::NoDotAndDotDot | QDir::Readable );
    QString newPath , searchPath;// = UTIL_filePathSlashToForward( basePath );

#ifdef linux
    newPath = "/";
#else
//#error "FIXME"
#endif

    if( !basePath.isEmpty() )
    {
        if( pathfinder.exists( basePath ) )
        {
            pathfinder.setPath( basePath );

            if( includeBasePath )
                newPath = basePath;
        }
        else
            searchPath = basePath + "/";
    }

    searchPath += path;

    UTIL_filePathSlashToForward( searchPath );
    QStringList parts = searchPath.split( QRegExp( "/|\\\\" , Qt::CaseInsensitive ) , QString::SkipEmptyParts );

    if( !name.isEmpty() )
        parts.append( getName( true ) );

    for( int partsIndex = 0 ; partsIndex < parts.length() ; partsIndex++ )
    {
        // Search for optional file name
        if( partsIndex + 1 == parts.length() && !name.isEmpty() )
            pathfinder.setFilter( QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot | QDir::Readable );

        QStringList entries = pathfinder.entryList();
        QStringList results;

        // Find directories with a matching name
        for( int entryIndex = 0 ; entryIndex < entries.length() ; entryIndex++ )
            if( !entries.at( entryIndex ).compare( parts.at( partsIndex ) , Qt::CaseInsensitive ) )
                results.append( entries.at( entryIndex ) );

        if( results.length() > 1 )
        {
            int resultIndex = 0;

            for( ; resultIndex < results.length() ; resultIndex++ )
                if( !results.at( resultIndex ).compare( parts.at( partsIndex ) , Qt::CaseSensitive ) )
                    break;

            if( resultIndex == results.length() )
            {
                // Multiple matching directory names with no exact match
                // For now, fail

                return CPath();
            }
            else
            {
                newPath += results.at( resultIndex ) + "/";
                pathfinder.cd( results.at( resultIndex ) );
            }
        }
        else if( results.length() == 1 )
        {
            newPath += results.at( 0 ) + "/";
            pathfinder.cd( results.at( 0 ) );
        }
        else
            return CPath();
    }

    return CPath( newPath , false );
}
