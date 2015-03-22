#include "qtil.h"

QString UTIL_trimFilePath( const QString &constPath )
{
	QString path = constPath;
	UTIL_trimFilePath( path );
	return path;
}

void UTIL_trimFilePath( QString &fixedPath )
{
	if( fixedPath.length() > 1 )
	{
		unsigned short slashIndex = 0 , slashEndex = fixedPath.length() - 1;
		for( ; slashIndex < fixedPath.length() && ( fixedPath.at( slashIndex ) == '/' || fixedPath.at( slashIndex ) == '\\' || fixedPath.at( slashIndex ).isSpace() ) ; slashIndex++ );
		for( ; slashEndex > slashIndex && ( fixedPath.at( slashEndex ) == '/' || fixedPath.at( slashEndex ) == '\\' || fixedPath.at( slashEndex ).isSpace() ) ; slashEndex-- );
		fixedPath = slashIndex >= slashEndex ? "" : fixedPath.mid( slashIndex , ( slashEndex + 1 ) - slashIndex );

#ifdef linux
		//fixedPath.prepend( '/' );
#endif
	}
}

// Convenience function where '/' and '\\' are considered the same and letter case is ignored
bool UTIL_compareFilePath( const QString &pathA , const QString &pathB )
{
	if( pathA.length() == pathB.length() )
	{
		for( unsigned short index = 0 ; index < pathA.length() ; index++ )
			if( pathA.at( index ).toLower() != pathB.at( index ).toLower() && !( ( pathA.at( index ) == '/' || pathA.at( index ) == '\\' ) && ( pathB.at( index ) == '/' || pathB.at( index ) == '\\' ) ) )
				return false;
	}
	else
		return false;

	return true;
}

// Handles both windows and unix style terminators
QByteArray UTIL_readLineFromQIODevice( QIODevice &device )
{
	QByteArray string;

	if( device.isOpen() && device.isReadable() && !device.atEnd() )
	{
		// Init
		static unsigned char newData[128] = { 0 };
		static unsigned char bytes;
		bytes = 0;

		do
		{
			if( bytes == 128 )
			{
				string.append( &newData[0] , 128 );
				bytes = 0;
			}

			device.getChar( &newData[ bytes ] );
			bytes++; // Last so bytes - 1 never results in -1
		}
		while( newData[ bytes - 1 ] != '\0' && newData[ bytes - 1 ] != 0x0A && newData[ bytes - 1 ] != 0x0D && !device.atEnd() );

		string.append( &newData[0] , bytes );

		// Can't check !device.atEnd() since a line end may be the last character
		if( string.endsWith( '\0' ) || string.endsWith( 0x0A ) || string.endsWith( 0x0D ) )
		{
			// This isn't meant to be here, but if moved below
			// next if, causes crashes. (TODO: Fix)
			string.chop( 1 );

			// Sometimes 0x0A follows 0x0D
			if( !device.isSequential() && string.endsWith( 0x0D ) )
			{
				if( !device.atEnd() )
				{
					device.getChar( bytes );

					// If so, eat it
					if( bytes != 0x0A )
						device.ungetChar( bytes );
				}
			}
		}
	}

	return string;
}

QString UTIL_readCStringFromQDataStream( QDataStream &stream , char delimiter /* = '\0' */ , unsigned short reserveLength /* = 128 */ )
{
#if 0
	// Can't enable this since it acts different enough from
	// UTIL_readLineFromQIODevice to potentially break other things
	//return stream.device() ? UTIL_readLineFromQIODevice( *stream.device() ) : "";
#else
	// Init
	unsigned char bufferChar = 0;
	QString string;

	if( reserveLength )
		string.reserve( reserveLength );

	// Get the string
	stream >> bufferChar;
	for( unsigned short charIndex = 0 ; !stream.atEnd() && bufferChar != delimiter ; charIndex++ )
	{
		string.append( bufferChar );

		stream >> bufferChar;
	}

	return string;
#endif
}

QString UTIL_standardNotationFloat( double value , int convertPrecision = -1 )
{
	QString string = QString( "%1" ).arg( value , 0 , 'f' , convertPrecision );

	if( string.lastIndexOf( '.' ) )
	{
		while( string.at( string.length() - 1 ) == '0' )
		{
			string.chop( 1 );

			if( string.at( string.length() - 1 ) == '.' )
			{
				string.chop( 1 );
				break;
			}
		}
	}

	return string;
}

// Returns true if key and value
bool UTIL_getPart( const QString &line , QString &key , QString &value )
{
	// TODO: Chck that line is not key or value

	unsigned short startPos = 0 , endPos = 0;

	for( ; startPos < line.length() && line[startPos].isSpace() ; startPos++ );

	if( startPos == line.length() || ( startPos + 1 < line.length() && line[startPos] == '/' && line[startPos + 1] == '/' ) )
	{
		// Line empty or is comment
		key.clear();
		value.clear();
		return false;
	}

	// If not empty, read key
	for( endPos = startPos + 1 ; endPos < line.length() && !line[endPos].isSpace() ; endPos++ );
	key = line.mid( startPos , endPos - startPos );

	// Remove quotes if wrapped with
	if( key.length() > 1 )
		key = key.mid( key[0] == '\"' , key.length() - ( key[0] == '\"' ) - ( key[key.length() - 1] == '\"' ) );

	// Locate start of value
	for( startPos = endPos ; startPos < line.length() && line[startPos].isSpace() ; startPos++ );

	if( startPos == line.length() || ( startPos + 1 < line.length() && line[startPos] == '/' && line[startPos + 1] == '/' ) )
	{
		// Key only. Value not present or is comment
		value.clear();
		return false;
	}

	// If not end of line, read value
	for( endPos = startPos + 1 ; endPos < line.length() && ( line[startPos] == '\"' ? line[endPos] != '\"' : !line[endPos].isSpace() ) ; endPos++ );

	// WARNING: This probably doesn't work properly
	// TODO: Test and fix if necessary
	if( endPos != line.length() )
	{
		value = line.mid( startPos , endPos - ( startPos - 1 ) );
		if( value.length() > 1 )
			value = value.mid( value[0] == '\"' , value.length() - ( value[0] == '\"' ) - ( value[value.length() - 1] == '\"' ) );
		return true;
	}

	value.clear();
	return false;
}

QStringList UTIL_getParts( const QString &line )
{
	// Outer parentheses for capturing
	// Slashes for C++ escaping and regex escaping
	// That is, in order to match a '"', QRegExp needs \"
	// This expands to \\\"
	// That is, \\ and \" before C++ escaping
	QRegExp regex( "(\\\"[^\\\"]+\\\"|//.*$|(\\S)+)" , Qt::CaseInsensitive );
	// Unescaped:          \"[^\"]+\"|//.*$|(\S)+
	//QRegExp regex( "\".*?\"|//.*?$|(\\S)+" , Qt::CaseInsensitive );
	//QRegExp regex( "(\"[^\"]+\")|(//.*$)|((\\S)+)" , Qt::CaseInsensitive );

	int index = 0;
	QStringList parts;

	while( ( index = regex.indexIn( line , index ) ) != -1 )
	{
		//parts << regex.cap( 0 );
		parts << regex.cap( 1 );
		//parts << regex.cap( 2 );
		index += regex.matchedLength();

		if( parts.last().endsWith( '\"' , Qt::CaseInsensitive ) )
			parts.last().chop( 1 );

		if( parts.last().startsWith( '\"' , Qt::CaseInsensitive ) )
			parts.last() = parts.last().mid( 1 );

		parts.last() = parts.last().trimmed();
	}

#if 0
	static unsigned int counter = 0;
	counter++;

	if( !( counter % 40 ) && parts.length() )
	{
		QString string = QString::number( parts.length() );
		//string.append( parts.first() );

		for( unsigned int index = 0 ; index < parts.length() ; index++ )
		{
			string.append( "\n" );
			string.append( parts.at( index ) );
		}

		QMessageBox::information( NULL , "Notice" , string );
	}
#endif

	return parts;
}

void UTIL_getParts( const QString &line , QString &key , QString &value , QString &comment )
{
	// Init
	QStringList parts = UTIL_getParts( line );

	key.clear();
	value.clear();
	comment.clear();

	for( unsigned int index = 0 ; index < parts.length() ; index++ )
	{
		if( parts.at( index ).startsWith( "//" , Qt::CaseInsensitive ) )
			comment = parts.at( index ).trimmed();
		else if( key.isNull() )
			key = parts.at( index ); //.mid( parts.at( index )[0] == '\"' , ( parts.at( index ).length() - ( parts.at( index )[0] == '\"' ) ) - ( parts.at( index )[ parts.at( index ).length() - 1 ] == '\"' ) ).trimmed();
		else if( value.isNull() )
			value = parts.at( index ); //.mid( parts.at( index )[0] == '\"' , ( parts.at( index ).length() - ( parts.at( index )[0] == '\"' ) ) - ( parts.at( index )[ parts.at( index ).length() - 1 ] == '\"' ) ).trimmed();
	}
}

void UTIL_getParts( const QString &line , QString &key , QString &value )
{
	static QString comment;

	UTIL_getParts( line , key , value , comment );
}

void UTIL_getParts( const QByteArray &line , QByteArray &key , QByteArray &value )
{
	static QString stringLine , stringKey , stringValue;

	stringLine = line;
	stringKey = key;
	stringValue = value;

	UTIL_getParts( stringLine , stringKey , stringValue );

	key = stringKey.toLatin1();
	value = stringValue.toLatin1();

	// In the conversion from unicode to ascii,
	// empty, but not null, strings become empty AND null byte arrays
	if( key.isNull() && !stringKey.isNull() )
		key = "";
	if( value.isNull() && !stringValue.isNull() )
		value = "";
}

void UTIL_centerOverWidget( QWidget *topWidget , const QWidget *bottomWidget )
{
	if( topWidget && bottomWidget )
	{
		topWidget->adjustSize();
		QPoint showPos = bottomWidget->mapToGlobal( QPoint( 0 , 0 ) );
		topWidget->move( showPos.x() + ( ( bottomWidget->width() - topWidget->width() ) / 2 ) , showPos.y() + ( ( bottomWidget->height() - topWidget->height() ) / 2 ) );
	}
}

bool UTIL_deviceCopy( QIODevice &from , QIODevice &to , quint64 bytes , QString *lastError )
{
	if( bytes == (quint64)-1 )
		bytes = from.size() - from.pos();

	static unsigned char newData[1024] = { 0 };
	static quint64 totalBytes = bytes;

	for( quint64 curLength = 0 ; bytes > 0 ; bytes -= curLength )
	{
		curLength = UTIL_upperLimit( bytes , (quint64)1024 );

		if( curLength == from.read( &newData[0] ,  curLength ) )
		{
			if( curLength == to.write( &newData[0] ,  curLength ) )
				continue;
			else if( lastError )
				*lastError = QString( "Write Error: Unable to write %1 of %2 bytes" ).arg( bytes ).arg( totalBytes );
		}
		else if( lastError )
			*lastError = QString( "Read Error: Unable to read %1 of %2 bytes" ).arg( bytes ).arg( totalBytes );

		return false;
	}

	return true;
}

QIcon UTIL_shellIconForExtension( const QString &extension )
{
	// Static stuff for optimization
	static QTemporaryFile iconSource;
	static QString fileName;
	static QFileIconProvider iconProvider;
	static QFileInfo fileInfo;

	// If the fileName is empty, the temporary file hasn't been made yet
	if( fileName.isEmpty() && iconSource.open() )
	{
		fileInfo.setCaching( false ); // Put here so it's not called every time
		fileName = iconSource.fileName();
		iconSource.close(); // The file doesn't need to stay open. Renaming closes it anyways
	}

	if( !fileName.isEmpty() )
	{
		iconSource.unsetError(); // Rename will continue to fail until the last error is unset
		fileInfo.setFile( fileName + ( extension.isEmpty() ? "" : ( "." + extension.toLower() ) ) );
		if( iconSource.rename( fileInfo.absoluteFilePath() ) || QFile::exists( fileInfo.absoluteFilePath() ) )
			return iconProvider.icon( fileInfo );
	}

	return QIcon( ":/resources/icons/empty" );
}

// QFile can't create directories that don't exist
// Path is taken to be absolute
bool UTIL_makeDirectoriesForPath( const QString &path , bool lastSplitIsFile )
{
#if 1
	static QDir pathfinder;
	pathfinder.mkpath( lastSplitIsFile ? path.left( path.lastIndexOf( REGEX_SLASH_PATTERN ) ) : path );
#else
	static QStringList pathTokens;

	pathTokens = path.split( REGEX_SLASH_PATTERN , QString::SkipEmptyParts );

	if( lastSplitIsFile )
		pathTokens.removeLast();

	for( int index = 0 ; index < pathTokens.length() ; index++ )
	{
		//QDir::mkdir()
		//QDir::mkpath()
	}
#endif

	return true;
}

int UTIL_getFontWidth( const QString &string , const QFont &font /* = QFont() */ )
{
	static QFontMetrics *fontMetrics = new QFontMetrics( font );

	return fontMetrics->width( string );
}

void UTIL_cleanFilePath( QByteArray &path )
{
	static QString stringPath;
	stringPath = path;
	UTIL_cleanFilePath( stringPath );
	path = stringPath.toLatin1();
}

void UTIL_cleanFilePath( QString &path )
{
	if( path.length() )
	{
		unsigned int index = path.length() - 1;

		// Remove trailing slashes
		for( ; index < path.length() && ( path.at( index ) == '/' || path.at( index ) == '\\' || path.at( index ).isSpace() ) ; index-- );

		if( index > path.length() )
			path.clear();
		else
			path.chop( ( path.length() - 1 ) - index );

		if( path.length() )
		{
			// Remove leading slashes
			for( index = 0 ; index < path.length() && ( path.at( index ) == '/' || path.at( index ) == '\\' || path.at( index ).isSpace() ) ; index++  );
			path = path.mid( index );

			if( path.length() )
			{
				for( index = 0 ; index < path.length() ; index++ )
				{
					// Change backslashes into forward slashes
					if( path.at( index ) == '\\' )
						path[ index ] = '/';

					// Remove multiple slashes
					if( path.at( index ) == '/' && index + 1 < path.length() && ( path.at( index + 1 ) == '/' || path.at( index + 1 ) == '\\' ) )
					{
						path.remove( index , 1 );
						index--;
					}
#if 1
					// TODO: Add any characters not allowed in file paths
					// Windows seems to be the most strict
#endif
				}
			}
		}
	}
}

bool UTIL_writeBytes( const QByteArray &data , QIODevice &device , unsigned int bytes )
{
	if( data.length() && device.isOpen() && device.isWritable() && bytes > 0 )
	{
		unsigned int index = 0;

		while( bytes )
		{
			device.putChar( data[ index ] );
			bytes--;

			index++;
			if( index == data.length() )
				index = 0;
		}
	}

	return true;
}

QString UTIL_groupDigits( quint64 number )
{
	QString digits = QString::number( number );

	for( int index = digits.length() % 3 ; index < digits.length() ; index += 4 )
	{
		if( index )
			digits.insert( index , ',' );
		else
			index--;
	}

	return digits;
}

QString UTIL_extensionForFileDialogFilter( const QString &filter )
{
	QString extension;

	int index = filter.indexOf( "*." , Qt::CaseInsensitive );

	if( index != -1 )
	{
		extension = filter.mid( index + 2 );
		extension = extension.mid( 0 , extension.indexOf( ' ' , Qt::CaseInsensitive ) );
		extension = extension.mid( 0 , extension.indexOf( ')' , Qt::CaseInsensitive ) );
	}

	return extension;
}

void UTIL_expandTreeItems( QTreeWidget *tree , QTreeWidgetItem *item )
{
	if( tree && item )
	{
		tree->expandItem( item );

		// Expand children as well
		for( int index = 0 ; index < item->childCount() ; index++ )
			UTIL_expandTreeItems( tree , item->child( index ) );
	}
}

//#define SLASHES "/\\"

bool UTIL_validateFileName( const QString &path , bool allowDirectories )
{
#ifdef WIN32
	static const char notAllowedCharsSubDir[]   = "\/?:*<>|";
#else
	//static const char notAllowedCharsSubDir[]   = ",^@=+{}[]~!?:&*\"|#%<>$\"'();`' ";
	static const char notAllowedCharsSubDir[]   = "";
	//static const char notAllowedCharsNoSubDir[] = ",^@={}[]~!?:&*\"|#%<>$\"'();`' " SLASHES;
	static const char notAllowedCharsNoSubDir[] = "/";
	//static const char *notAllowedSubStrings[] = {".."};
	//static const char *notAllowedSubStrings[] = {".."};
#endif

	if( path.isEmpty() || !path.compare( "/" , Qt::CaseInsensitive ) )
		return false;

	// Characters
	const char *notAllowedChars = allowDirectories ? notAllowedCharsSubDir : notAllowedCharsNoSubDir;

	do
	{
		if( path.contains( QLatin1Char( *notAllowedChars ) , Qt::CaseInsensitive ) )
			return false;

		notAllowedChars++;
	}
	while( *notAllowedChars );

	/*
	for( const char *c = notAllowedChars ; *c ; c++ )
		if( path.contains( QLatin1Char(*c) , Qt::CaseInsensitive ) )
			return false;
	*/

#if 0
	// Substrings
	const int notAllowedSubStringCount = sizeof( notAllowedSubStrings ) / sizeof( const char* );

	for( int s = 0 ; s < notAllowedSubStringCount ; s++ )
	{
		const QLatin1String notAllowedSubString( notAllowedSubStrings[ s ] );

		if( name.contains( notAllowedSubString ) )
			return false;
	}
#endif

#if 0
	// Windows devices
	bool matchesWinDevice = windowsDeviceNoSubDirPattern().exactMatch(name);

	if( !matchesWinDevice && allowDirectories )
		matchesWinDevice = windowsDeviceSubDirPattern().exactMatch(name);

	if( matchesWinDevice )
		return false;
#endif

	return true;
}
