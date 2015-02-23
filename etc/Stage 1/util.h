// =======================================================
// Generic Utility Code
// =======================================================
// Purpose: Collection of randomly useful code
//********************************************************

#include <string>
#include <vector>

#include <QMessageBox>

bool UTIL_getPart( const std::string &line , std::string &key , std::string &value );

#ifndef UTIL_DECONST_H
#define UTIL_DECONST_H

template <class T> inline T DECONST( const T object ) { return const_cast<T>( object ); }

#endif

#ifndef UTIL_DECONSTPTR_H
#define UTIL_DECONSTPTR_H

template <class T> inline T* DECONST( const T* object ) { return const_cast<T*>( object ); }

#endif

#ifndef UTIL_SPLIT_H
#define UTIL_SPLIT_H

template <class T> std::vector<std::vector<T>*> UTIL_split( const std::vector<T> &data , T delimiter , bool keepEmptyResults , unsigned int extraReserve = 0 )
{
	std::vector<std::vector<T>*> tokenVector;

	for( unsigned int dataStart = 0 , dataEnd = 0 ; dataEnd < data.size() ; dataEnd++ )
	{
		for( dataStart = dataEnd ; dataEnd < data.size() && data[dataEnd] != delimiter ; dataEnd++ );
		if( dataStart + 1 != dataEnd || keepEmptyResults )
		{
			std::vector<T> *newVec = new std::vector<T>;
			newVec->reserve( ( dataEnd - ( dataStart - 1 ) ) + extraReserve );
			for( unsigned int copyIndex = dataStart ; copyIndex < dataEnd ; copyIndex++ )
				newVec->push_back( data[copyIndex] );
			tokenVector.push_back( newVec );
		}
			//tokenVector.push_back( new std::vector<T>( data.begin() + dataStart , data.begin() + ( dataEnd - 1 ) ) );
	}

	return tokenVector;
}

#endif

#ifndef UTIL_REMOVEDUPLICATESVECTOR_H
#define UTIL_REMOVEDUPLICATESVECTOR_H

// Items must have an implementation of operator==
template <class T> void UTIL_removeDuplicates( std::vector<T> &list )
{
	for( unsigned int baseIndex = 0 ; baseIndex < list.size() ; baseIndex++ )
		for( unsigned int cmpIndex = list.size() - 1 ; cmpIndex > baseIndex ; cmpIndex-- )
			if( list.at( cmpIndex ) == list.at( baseIndex ) )
				list.erase( list.begin() + cmpIndex );
}

#endif

#ifndef UTIL_DELETEPOINTERVECTOR_H
#define UTIL_DELETEPOINTERVECTOR_H

// Items must be pointers and have implementation of operator==
template <class T> void UTIL_deleteVectorAsPointerArray( std::vector<T> &list )
{
	UTIL_removeDuplicates( list );

	unsigned int index = 0 , lastSize = list.size();

	while( index < list.size() )
	{
		delete list.at( index );

		// Only increment index if deleted item did not remove itself from the list
		if( lastSize == list.size() )
			index++;
	}
}

#endif

#ifndef UTIL_CLAMP_H
#define UTIL_CLAMP_H

// Items must have an implementation of operator>
template <class T> T UTIL_clamp( T cmp , T min , T max )
{
	return min > cmp ? min : ( cmp > max ? max : cmp );
}

#endif

#ifndef UTIL_LOWERLIMIT_H
#define UTIL_LOWERLIMIT_H

// Items must have an implementation of operator>
template <class T> T UTIL_lowerLimit( T cmp , T min )
{
	return cmp > min ? cmp : min;
}

#endif

#ifndef UTIL_UPPERLIMIT_H
#define UTIL_UPPERLIMIT_H

// Items must have an implementation of operator>
template <class T> T UTIL_upperLimit( T cmp , T max )
{
	return cmp > max ? max : cmp;
}

#endif

#ifndef UTIL_MIN_H
#define UTIL_MIN_H

// Items must have an implementation of operator>
template <class T> T UTIL_min( T a , T b )
{
	return a > b ? b : a;
}

#endif

#ifndef UTIL_MAX_H
#define UTIL_MAX_H

// Items must have an implementation of operator>
template <class T> T UTIL_max( T a , T b )
{
	return a > b ? a : b;
}

#endif

#ifndef CSTACKPTR_H
#define CSTACKPTR_H

// Personal implementation of auto_ptr
// Allows pointers returned by functions to be used
// as arguments without an intermediate variable
#if 0
template <class T> class stackPtr
{
public:
	stackPtr<T>( T *object )
	{
		this->object = object;
	}

	~stackPtr()
	{
		delete object;
	}

	T* operator*( void ) const
	{
		return object;
	}

private:
	T* const object;
};
#endif

#endif

#ifndef CPAIR_H
#define CPAIR_H

// Simple helper class to work with two related items as one
// Easily used in conjunction with QList
template <class T,class U> class CPair
{
public:
	CPair<T,U>( void )
	{
		//
	}

	CPair<T,U>( T first , U second )
    {
		primary = first;
		secondary = second;
    }

	T primary;
	U secondary;

	CPair<T,U> operator+( const CPair<T,U> &other )
	{
		return CPair<T,U>( this->primary + other.primary , this->secondary + other.secondary );
	}

	CPair<T,U> operator+=( const CPair<T,U> &other )
	{
		this->primary += other.primary;
		this->secondary += other.secondary;
		return *this;
	}
};

#endif // CPAIR_H

#ifndef CTRIPLET_H
#define CTRIPLET_H

// Simple helper class to work with three related items as one
// Easily used in conjunction with QList
template <class T,class U,class V> class CTriplet
{
public:
	CTriplet<T,U,V>( void )
	{
		//
	}

	CTriplet<T,U,V>( T first , U second , V third )
    {
		primary = first;
		secondary = second;
		tertiary = third;
    }

	T primary;
	U secondary;
	V tertiary;
};

#endif // CTRIPLET_H

#ifndef UTIL_UTIL_H
#define UTIL_UTIL_H

inline bool UTIL_isPowerOfTwo( const quint64 &value ) {	return value && !( value & ( value - 1 ) ); }

// 8 = UTIL_nextPowerOfTwo( 5 , false );
// 8 = UTIL_nextPowerOfTwo( 8 , false );
// 8 = UTIL_nextPowerOfTwo( 5 , true );
// 16 = UTIL_nextPowerOfTwo( 8 , true );
// Control boolean ignored if value is maximum power
quint64 UTIL_nextPowerOfTwo( const quint64 &value , bool valueCannotBeNextPower );

quint64 UTIL_lastPowerOfTwo( const quint64 &value , bool valueCannotBeLastPower );

quint64 UTIL_nearestPowerOfTwo( const quint64 &value );

// Returns -1 if zero or not a power of two
unsigned char UTIL_flagToIndex( quint64 flag );

#endif // UTIL_UTIL_H



#if 0
	if( scanTable->rowCount() )
	{
		// Clear list
		for( unsigned short curRow = 0 ; curRow < scanTable->rowCount() ; curRow++ )
			for( unsigned char curCol = 0 ; curCol < scanTable->columnCount() ; curCol++ )
				delete scanTable->item( curRow , curCol );
		scanTable->setRowCount( 0 );
	}

	// Init
	QFile curFile;
	QFileInfo curFileInfo;
	const char *directionalSuffix[] = { "rt" , "lf" , "ft" , "bk" , "up" , "dn" };

	QString curPath , curName , key , value , gameDir = gameDirEdit->text();
	UTIL_filePathSlashToForward( gameDir );
	UTIL_trimFilePath( gameDir );

#ifdef _LINUX
	gameDir.prepend( '/' );
#endif

	gameDirEdit->setText( gameDir );
	gameDirEdit->setEnabled( false ); // Disable editing until applied or canceled

	QStringList *fileList = new QStringList;

	PCFDelegate pcfDelegate;
	QList<CFilePCF*> particleList;
	QList<QString> particleFileList;

	// Make a list of particle files
	if( scanOptionArray[PARTICLES] )
	{

		QDir particleDir = gameDir + "/particles";
		particleFileList = particleDir.entryList( QStringList( "*.pcf" ) , QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot | QDir::Readable , QDir::Name );
		for( int index = 0 ; index < particleFileList.length() ; index++ )
			particleFileList[index] = UTIL_trimFilePath( particleFileList.at( index ) );

		for( int index = 0 ; index < particleFileList.length() ; index++ )
		{
			particleList.append( new CFilePCF );
			if( !pcfDelegate.readFile( gameDir + "/particles/" + particleFileList.at( index ) , particleList.last() ) )
			{
				//QMessageBox::warning( this , this->windowTitle() , pcfDelegate.lastError() + "\"" + particleDir.absolutePath() + particleFileList.at( index ) + "\"" , QMessageBox::Ok , QMessageBox::Ok );
				particleFileList.removeAt( index );
				delete particleList.takeLast();
				index--;
			}
		}
	}

	bool inScan = false , inPak = false;
	char bufferChar = 0;
	short dividerPos = 0;
	unsigned short pakIndex = 0;
	unsigned int /* textureCountOffset = 0 , */ texdataStringDataLength = bspDelegate->lumpInfo( CDelegateBSP::LUMP_TEXDATASTRINGTABLE , CDelegateBSP::LUMP_LENGTH ) / 4; /* sizeof( int32 ) */

	QProgressDialog readProgress( "" , "Abort" , 0 , 1 , this );
	readProgress.setWindowTitle( this->windowTitle() );
	readProgress.setFixedWidth( 400 );
	readProgress.setWindowModality( Qt::WindowModal );
	readProgress.setCancelButton( NULL );
	readProgress.setAutoClose( false );
	readProgress.setMinimumDuration( 500 );
	readProgress.adjustSize();
	readProgress.setFixedSize( readProgress.width() , readProgress.height() );
	UTIL_centerOverParent( &readProgress );

	// Scan VMTs and MDLs in the pak for referenced files
#if 1
	if( scanOptionArray[MATERIALS_PACKED] || scanOptionArray[MODELS_PACKED] )
	{
		QByteArray tempFile;
		tempFile.reserve( 4194304 ); // 4 MB
		QBuffer tempFileBuffer;
		tempFileBuffer.setBuffer( &tempFile );

		readProgress.setLabelText( "Scanning Pak Files..." );
		readProgress.setMaximum( pakfileTable->rowCount() );

		for( unsigned short index = 0 ; index < pakfileTable->rowCount() ; index++ )
		{
			VPakfileTableWidgetItem *selectedTracker = static_cast<VPakfileTableWidgetItem*>( pakfileTable->item( index , 0 ) ); // WARNING: Assumption

			if( selectedTracker->centralDirectoryIndex != -1 && ( ( selectedTracker->text().endsWith( ".vmt" , Qt::CaseInsensitive ) && progOp->scan_pak_vmts ) || ( selectedTracker->text().endsWith( ".mdl" , Qt::CaseInsensitive ) && progOp->scan_pak_mdls ) || ( selectedTracker->text().endsWith( ".phy" , Qt::CaseInsensitive ) && progOp->scan_pak_phys ) ) )
			{
				// Checking for duplicates here saves time by reducing the number of files read
				// as well as eliminating the need to remove duplicates from an entire, longer list
				if( !fileList->contains( pakfileTable->item( index , 1 )->text() + "/" + pakfileTable->item( index , 0 )->text() , Qt::CaseInsensitive ) )
				{
					pakStream->device()->seek( bspDelegate->lumpInfo( CDelegateBSP::LUMP_PAKFILE , CDelegateBSP::LUMP_OFFSET ) + ZIP_file_header_array[selectedTracker->centralDirectoryIndex]->relative_offset_of_local_header + ZIP_SIGNATURE_SIZE + ZIP_LOCAL_FILE_HEADER_SIZE + ZIP_file_header_array[selectedTracker->centralDirectoryIndex]->file_name_length );
					char *pakFileContents = new char[ZIP_file_header_array[selectedTracker->centralDirectoryIndex]->compressed_size];

					if( pakStream->readRawData( pakFileContents , (int)ZIP_file_header_array[selectedTracker->centralDirectoryIndex]->compressed_size ) == (int)ZIP_file_header_array[selectedTracker->centralDirectoryIndex]->compressed_size )
					{
						tempFileBuffer.setData( pakFileContents , (int)ZIP_file_header_array[selectedTracker->centralDirectoryIndex]->compressed_size );

						key = pakfileTable->item( index , 1 )->text() + "/" + pakfileTable->item( index , 0 )->text();
						UTIL_filePathSlashToForward( key );

						if( scanOptionArray[MATERIALS_PACKED] )
						{
							if( selectedTracker->text().endsWith( "t" , Qt::CaseInsensitive ) )
							{
								//fileList->append( key );
								scanVMT( tempFileBuffer , fileList , gameDir , key , value );
							}
						}
						else if( scanOptionArray[MODELS_PACKED] )
						{
							if( selectedTracker->text().endsWith( "l" , Qt::CaseInsensitive ) )
							{
								//fileList->append( key );
								scanMDL( tempFileBuffer , fileList , gameDir , key , value );
							}
						}
						else if( scanOptionArray[MODELS_PACKED] )
						{
							scanPHY( tempFileBuffer , fileList , gameDir , key , value );
						}
					}
				}
			}

			readProgress.setValue( index + 1 );
		}
	}
#endif

	// Scan referenced VMTs
	if( scanOptionArray[MATERIALS] )
	{
		// Iterate through list
		readProgress.setLabelText( "Scanning materials..." );
		readProgress.setMaximum( texdataStringDataLength );
		pakStream->device()->seek( bspDelegate->lumpInfo( CDelegateBSP::LUMP_TEXDATASTRINGDATA , CDelegateBSP::LUMP_OFFSET ) );
		for( unsigned int index = 0 ; index < texdataStringDataLength ; index++ )
		{
			// Clean up
			curPath.clear();
			bufferChar = 0;

			// Get the local path
			pakStream->device()->getChar( &bufferChar );
			for( unsigned short charIndex = 0 ; bufferChar ; charIndex++ )
			{
				curPath.resize( curPath.length() + 1 );
				//if( bufferChar > 64 && bufferChar < 91 )
					//bufferChar += 32;
				curPath.data()[charIndex] = bufferChar;
				pakStream->device()->getChar( &bufferChar );
			}

			UTIL_filePathSlashToForward( curPath );
			UTIL_trimFilePath( curPath );
			// Checking for duplicates here saves time by reducing the number of files read
			// as well as eliminating the need to remove duplicates from an entire, longer list
			if( !fileList->contains( "/materials/" + curPath + ".vmt" , Qt::CaseInsensitive ) )
			{
				fileList->append( "/materials/" + curPath + ".vmt" );
				if( !gameDir.isEmpty() ) // Don't check if the base directory is unknown
				{
					curFile.setFileName( gameDir + "/materials/" + curPath + ".vmt" );
					scanVMT( curFile , fileList , gameDir , key , value );
				}
			}

			readProgress.setValue( index + 1 );
		}
	}

	// Scan referenced MDLs

	if( scanOptionArray[MODELS] || scanOptionArray[MATERIALS] || scanOptionArray[SOUNDS] || scanOptionArray[SCENES] || scanOptionArray[SPECIAL_FILES] || scanOptionArray[SKYBOX] || scanOptionArray[DETAILSPRITES] || scanOptionArray[PARTICLES] )
	{
		// Seek to the entity lump and read non-static props and everything else useful
		readProgress.setLabelText( "Scanning entities..." );
		readProgress.setMaximum( bspDelegate->lumpInfo( CDelegateBSP::LUMP_ENTITIES , CDelegateBSP::LUMP_LENGTH ) );
		pakStream->device()->seek( bspDelegate->lumpInfo( CDelegateBSP::LUMP_ENTITIES , CDelegateBSP::LUMP_OFFSET ) );
		for( unsigned int endPos = bspDelegate->lumpInfo( CDelegateBSP::LUMP_ENTITIES , CDelegateBSP::LUMP_OFFSET ) + bspDelegate->lumpInfo( CDelegateBSP::LUMP_ENTITIES , CDelegateBSP::LUMP_LENGTH ) ; pakStream->device()->pos() < endPos ; )
		{
			if( UTIL_getPart( pakStream->device()->readLine() , curName , curPath ) )
			{
				curPath = curPath.toLower();
				UTIL_filePathSlashToForward( curPath );
				UTIL_trimFilePath( curPath );

				// Check known extensions first
				if( scanOptionArray[MODELS] && curPath.endsWith( ".mdl" , Qt::CaseInsensitive ) )
				{
					if( !fileList->contains( "/" + curPath , Qt::CaseInsensitive ) )
					{
						fileList->append( "/" + curPath );
						curFile.setFileName( gameDir + "/" + curPath );
						scanMDL( curFile , fileList , gameDir , key , value );
					}
				}
				else if( scanOptionArray[MATERIALS] && ( curPath.endsWith( ".vmt" , Qt::CaseInsensitive ) || curPath.endsWith( ".spr" , Qt::CaseInsensitive ) ) )
				{
					if( !fileList->contains( "/materials/" + curPath , Qt::CaseInsensitive ) )
					{
						fileList->append( "/materials/" + curPath );
						curFile.setFileName( gameDir + "/materials/" + curPath );
						scanVMT( curFile , fileList , gameDir , key , value );
					}
				}
				else if( scanOptionArray[SOUNDS] && ( curPath.endsWith( ".wav" , Qt::CaseInsensitive ) || curPath.endsWith( ".mp3" , Qt::CaseInsensitive ) ) )
				{
					if( !fileList->contains( "/sound/" + curPath , Qt::CaseInsensitive ) )
						fileList->append( "/sound/" + curPath );
				}
				else if( scanOptionArray[SCENES] && curPath.endsWith( ".vcd" , Qt::CaseInsensitive ) )
				{
					if( !fileList->contains( "/" + curPath , Qt::CaseInsensitive ) )
						fileList->append( "/" + curPath );
				}
				else // No extension, check key name against keys known to lack extensions
				{
					curName = curName.toLower();

					// Check for special key
					int specialListIndex = progOp->specialKeyList->indexOf( curName );

					if( specialListIndex != -1 )
					{
						if( scanOptionArray[SPECIAL_FILES] )
						{
							QString replaceValue = QString( progOp->specialKeyValueReplacementList->at( specialListIndex ) ).replace( '*' , curPath , Qt::CaseInsensitive );
							if( !fileList->contains( replaceValue ) )
								fileList->append( replaceValue );
						}
					}
					else
					{
						// Special-er special keys
						if( !inScan && curName == "skyname" )
						{
							if( scanOptionArray[SKYBOX] )
								for( unsigned char index = 0 ; index < 6 ; index++ )
								{
									if( !fileList->contains( "/materials/skybox/" + curPath + directionalSuffix[index] + ".vmt" , Qt::CaseInsensitive ) )
									{
										fileList->append( "/materials/skybox/" + curPath + directionalSuffix[index] + ".vmt" );
										curFile.setFileName( gameDir + "/materials/skybox/" + curPath + directionalSuffix[index] + ".vmt" );
										scanVMT( curFile , fileList , gameDir , key , value );

										inScan = true;
									}
								}
						}
						else if( !inPak && curName == "detailmaterial" )
						{
							if( scanOptionArray[DETAILSPRITES] && !fileList->contains( "/materials/" + curPath + ".vmt" , Qt::CaseInsensitive ) )
							{
								fileList->append( "/materials/" + curPath + ".vmt" );
								curFile.setFileName( gameDir + "/materials/" + curPath + ".vmt" );
								scanVMT( curFile , fileList , gameDir , key , value );

								inPak = true;
							}
						}
						else if( progOp->materialKeyList->indexOf( curName ) != -1 || progOp->spriteKeyList->indexOf( curName ) != -1 )
						{
							if( scanOptionArray[MATERIALS] )
							{
								// Materials may lack extensions

								unsigned char extLength = 2;
								const char *ext[] = { ".vmt" , ".spr" };
								unsigned char extIndex = 0;

								for( ; extIndex < extLength ; extIndex++ )
								{
									// Because the extension is unknown, check if the file exists before trying to do anything with it.
									// Otherwise adding it creates undesired statistical anomalies
									if( QFile::exists( gameDir + "/materials/" + curPath + ext[extIndex] ) && !fileList->contains( "/materials/" + curPath + ext[extIndex] , Qt::CaseInsensitive ) )
									{
										fileList->append( "/materials/" + curPath + ext[extIndex] );
										curFile.setFileName( gameDir + "/materials/" + curPath + ext[extIndex] );
										scanVMT( curFile , fileList , gameDir , key , value );
									}
								}
							}
						}
						else if( progOp->particleKeyList->indexOf( curName ) != -1 )
						{
							if( scanOptionArray[PARTICLES] )
							{
								// Particles lack extensions because they don't refer to entire files
								int particleListIndex = 0;
								for( ; particleListIndex < particleList.length() && !particleList.at( particleListIndex )->findSystemByName( curPath ) ; particleListIndex++ );

								if( particleListIndex != particleList.length() && !fileList->contains( "/particles/" + particleFileList.at( particleListIndex ) ) )
								{
									fileList->append( "/particles/" + particleFileList.at( particleListIndex ) );
									CFilePCF pcfFile;

									if( pcfDelegate.readFile( gameDir + fileList->last() , &pcfFile ) )
										scanPCF( pcfFile , fileList , gameDir , key , value , curPath );
									else
									{
										// TODO: Error handling, QProgressDialogger
										fileList->removeLast(); // Don't pack a potentially corrupt file
									}
								}
							}
						}
					}
				}
			}

			readProgress.setValue( pakStream->device()->pos() - bspDelegate->lumpInfo( CDelegateBSP::LUMP_ENTITIES , CDelegateBSP::LUMP_OFFSET ) );
		}
	}

	// Reset // Used in above to ignore excessive string comparison
	inScan = false;
	inPak = false;
	readProgress.setMaximum( readProgress.maximum() );

	if( scanOptionArray[STATIC_PROPS] )
	{
		int gameLumpCount = 0;
		pakStream->device()->seek( bspDelegate->lumpInfo( CDelegateBSP::LUMP_GAMELUMP , CDelegateBSP::LUMP_OFFSET ) );
		*pakStream >> gameLumpCount;

		// Seek to the position of the static prop lump
		for( int index = 0 , lumpID = 0 ; index < gameLumpCount ; index++ )
		{
			*pakStream >> lumpID;
			if( lumpID == GAMELUMP_PROP_STATIC_LITTLE_ENDIAN )
			{
				pakStream->device()->seek( pakStream->device()->pos() + 4 ); // Skip flags and version
				*pakStream >> lumpID; // lumpID now contains the lump offset // length isn't required, it's also wrong
				pakStream->device()->seek( lumpID );
				*pakStream >> index; // index now contains the number of static prop entries
				curPath.reserve( MDL_NAME_LENGTH );

				readProgress.setLabelText( "Scanning static props..." );
				readProgress.setMaximum( index );

				// Iterate through list
				for( int curIndex = 0 , charIndex = 0 ; curIndex < index ; curIndex++ )
				{
					// Get the model name
					curPath.clear();
					pakStream->device()->getChar( &bufferChar );
					for( charIndex = 0 ; bufferChar ; charIndex++ )
					{
						curPath.resize( curPath.length() + 1 );
						curPath.data()[charIndex] = bufferChar;
						pakStream->device()->getChar( &bufferChar );
					}
					pakStream->device()->seek( pakStream->device()->pos() + ( 127 - charIndex ) );

					UTIL_filePathSlashToForward( curPath );
					if( !fileList->contains( "/" + curPath , Qt::CaseInsensitive ) )
					{
						fileList->append( "/" + curPath );
						if( !gameDir.isEmpty() ) // Don't check if the base directory is unknown
						{
							curFile.setFileName( gameDir + "/" + curPath );
							scanMDL( curFile , fileList , gameDir , key , value );
						}
					}

					readProgress.setValue( curIndex + 1 );
				}

				// Break
				index = gameLumpCount;
			}
			else
				pakStream->device()->seek( pakStream->device()->pos() + 12 ); // Skip over the rest of the header
		}
	}


	// Add extra files
	if( scanOptionArray[MAP_SPECIFIC_FILES] )
		for( unsigned short index = 0 ; index < progOp->mapFileList->length() ; index++ )
			fileList->append( ( (QString)( progOp->mapFileList->at( index ) ) ).replace( '*' , this->windowTitle() , Qt::CaseInsensitive ) );

	// Debugging fileList dump
#if 0
	if( *appOut )
	{
		(*appOut)->append( "DEBUG: fileList dump" );
		for( unsigned short index = 0 ; index < fileList->length() ; index++ )
			(*appOut)->append( fileList->at( index ) );
	}
#endif

	// Debugging fileList comparison
#if 0
	if( *appOut )
	{
		readProgress.setLabelText( "DEBUG: Comparing pakfileTable to fileList..." );
		readProgress.setMaximum( pakfileTable->rowCount() );
		(*appOut)->append( "Files In Pak Not Found In Reference List:" );
		for( pakIndex = 0 ; pakIndex < pakfileTable->rowCount() ; pakIndex++ )
		{
			inScan = false;
			for( unsigned short index = 0 ; index < fileList->length() && !inScan ; index++ )
				if( VPakfile::compareFilePath( pakfileTable->item( pakIndex , 1 )->text() + "/" + pakfileTable->item( pakIndex , 0 )->text() , fileList->at( index ) ) )
					inScan = true;
			if( !inScan )
				(*appOut)->append( pakfileTable->item( pakIndex , 1 )->text() + "/" + pakfileTable->item( pakIndex , 0 )->text() );
			readProgress.setValue( pakIndex + 1 );
		}
	}
#endif

	//if( *appOut )
		//(*appOut)->append( QString( "Removed %1 duplicates" ).arg( fileList->removeDuplicates() ) ); // How convenient
	//unsigned short notOnDisk = 0;
	//unsigned short notInPakEither = 0;
	scanTable->setRowCount( fileList->length() );
	readProgress.setLabelText( "Building table..." );
	readProgress.setMaximum( fileList->length() );

	// Build the table
	QString cmpSz;
	unsigned short specialIndex = scanTable->rowCount() - ( scanOptionArray[MAP_SPECIFIC_FILES] ? progOp->mapFileList->length() : 0 ) , specialIndexIgnoreCount = 0;
	unsigned int totalSpecialIndexIgnoreCount = 0;
	// specialIndexIgnoreCount is used for adding regular files referenced by special files
	// A total is kept to properly offset mapFileTypeList

	for( unsigned short index = 0 ; index < fileList->length() /* scanTable->rowCount() */ ; index++ )
	{
		//if( *appOut )
			//(*appOut)->append( QString( "Current Item: %1/%2" ).arg( index ).arg( fileList->length() - 1 ) );

		// Get the file name
		curPath = fileList->at( index );
		for( dividerPos = curPath.length() - 1 ; dividerPos > -1 && curPath[dividerPos] != '/' && curPath[dividerPos] != '\\' ; dividerPos-- );
		curName = curPath.right( curPath.length() - ( dividerPos == -1 ? 0 : dividerPos + 1 ) );

		// Get the relative path
		if( dividerPos == -1 )
			curPath.clear();
		else
			curPath = curPath.left( dividerPos );

		// Check if the file is already in the pak
		inPak = false;
		pakIndex = 0;
		for( cmpSz = fileList->at( index ).toLower() ; pakIndex < pakfileTable->rowCount() && !inPak ; pakIndex++ )
			if( UTIL_compareFilePath( pakfileTable->item( pakIndex , 1 )->text() + "/" + pakfileTable->item( pakIndex , 0 )->text() , cmpSz ) )
				inPak = true;
		pakIndex--;

		// Add table entries
		scanTable->setItem( index , 0 , new VScanTableWidgetItem( curName , gameDirEdit , scanTable , inPak ? pakIndex : -1 ) );
		scanTable->setItem( index , 1 , new QTableWidgetItem( curPath.isEmpty() ? "/" : curPath ) );
		scanTable->setItem( index , 3 , new QTableWidgetItem( ( index >= specialIndex && !specialIndexIgnoreCount ) ? progOp->mapFileTypeList->at( index - specialIndex - totalSpecialIndexIgnoreCount ) : VPakfile::typeForExtension( curName.right( curName.length() - curName.indexOf( '.' , Qt::CaseInsensitive ) - 1 ) ) ) );

		scanTable->setRowHeight( index , 15 );
		curFileInfo.setFile( gameDir + fileList->at( index ) );

		// Get and set the file size
		QTableWidgetItem *sizeItem = new QTableWidgetItem( "" );
		if( curFileInfo.exists() )
			sizeItem->setData( Qt::DisplayRole , curFileInfo.size() );
		else
		{
			//notOnDisk++;
			sizeItem->setData( Qt::DisplayRole , "N/A" );
			//if( !inPak )
				//notInPakEither++;
		}
		scanTable->setItem( index , 2 , sizeItem );
		scanTable->item( index , 2 )->setTextAlignment( Qt::AlignRight );

		// Set the row color
		static_cast<VScanTableWidgetItem*>( scanTable->item( index , 0 ) )->refreshStatus();

		// Check specific special files for referenced goodies
		if( index >= specialIndex && !gameDir.isEmpty() && ( scanTable->item( index , 3 )->text() == "Soundscript" || scanTable->item( index , 3 )->text() == "Soundscape" || scanTable->item( index , 3 )->text() == "Particle Data" ) && QFile::exists( gameDir + scanTable->item( index , 1 )->text() + "/" + scanTable->item( index , 0 )->text() ) )
		{
			const unsigned short fileListLengthBefore = fileList->length();
			QStringList swapList( fileList->mid( index + 1 ) );
			for( unsigned short removeIndex = fileList->length() - 1 ; removeIndex != index ; removeIndex-- )
				fileList->removeAt( removeIndex );

			curFile.setFileName( gameDir + fileList->last() );

			// Use different function depending on the special type
			if( scanTable->item( index , 3 )->text() == "Particle Data" )
				scanParticleManifest( curFile , fileList , gameDir , key , value );
			else
				scanSoundscript( curFile , fileList , gameDir , key , value );

			fileList->append( swapList );
			if( fileList->length() != fileListLengthBefore )
			{
				specialIndexIgnoreCount = fileList->length() - fileListLengthBefore + 1; // Plus one to counter the end loop check
				totalSpecialIndexIgnoreCount += specialIndexIgnoreCount - 1;
				scanTable->setRowCount( scanTable->rowCount() + specialIndexIgnoreCount - 1 );
			}
		}

		// Should ONLY be used by special files
		if( specialIndexIgnoreCount )
			specialIndexIgnoreCount--;
		else
			readProgress.setValue( index + 1 );
	}

	delete fileList;
#if 0
	if( *appOut )
	{
		(*appOut)->append( QString( "DEBUG: Referenced File Count: %1" ).arg( fileList->length() ) );
		(*appOut)->append( QString( "DEBUG: Files Not Found On Disk: %1\n       Those Not In Pak: %2" ).arg( notOnDisk ).arg( notInPakEither ) );
	}
#endif

#endif
