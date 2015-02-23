#include "util.h"

// Subroutine for UTIL_getPart
bool UTIL_isWhitespace( const char letter )
{
	return letter == '\t' || letter == ' ' || letter == '\n' || letter == '\r' || letter == '\b' || letter == '\v' || letter == '\f' || letter == '\a';
}

// Break a keyvalue string into component parts, ignoring comments.
// Return true if key and value, otherwise false
bool UTIL_getPart( const std::string &line , std::string &key , std::string &value )
{
	unsigned int startPos = 0 , endPos = 0;

	for( ; startPos < line.length() && UTIL_isWhitespace( line[startPos] ) ; startPos++ );

	if( startPos == line.length() || ( startPos + 1 < line.length() && line[startPos] == '/' && line[startPos + 1] == '/' ) )
	{
		// Line empty or is a comment
		key.clear();
		value.clear();
		return false;
	}

	//static unsigned int counter = 0;
	//counter++;

	//if( !( counter % 100 ) )
		//QMessageBox::information( NULL , "Notice" , QString( line.c_str() ) + "\n" + QString( key.c_str() ) + "\n" + QString( value.c_str() ) );

	// If not empty, read key
	for( endPos = startPos + 1 ; endPos < line.length() && !UTIL_isWhitespace( line[endPos] ) ; endPos++ );
	key = line.substr( startPos , endPos - startPos );

	// Remove quotes if wrapped with // TODO: Merge with above line
	if( key.length() > 1 )
		key = key.substr( key[0] == '\"' , key.length() - ( key[0] == '\"' ) - ( key[key.length() - 1] == '\"' ) );

	// Locate start of value
	for( startPos = endPos ; startPos < line.length() && UTIL_isWhitespace( line[startPos] ) ; startPos++ );

	if( startPos == line.length() || ( startPos + 1 < line.length() && line[startPos] == '/' && line[startPos + 1] == '/' ) )
	{
		// Key only. Value not present or is comment
		value = "?"; // Allows for quick checking if getPart failed with a valid key
		return false;
	}

	// If not end of line, read value // TODO: Merge with above line
	for( endPos = startPos + 1 ; endPos < line.length() && ( line[startPos] == '\"' ? line[endPos] != '\"' : !UTIL_isWhitespace( line[endPos] ) ) ; endPos++ );

	if( endPos != line.length() )
	{
		value = line.substr( startPos , endPos - ( startPos - 1 ) );
		if( value.length() > 1 )
			value = value.substr( value[0] == '\"' , value.length() - ( value[0] == '\"' ) - ( value[value.length() - 1] == '\"' ) );

		//QMessageBox::information( NULL , "Notice" , QString( line.c_str() ) + "\n" + QString( key.c_str() ) + "\n" + QString( value.c_str() ) );

		return true;
	}

	value = "";
	return false;
}

quint64 UTIL_nextPowerOfTwo( const quint64 &value , bool valueCannotBeNextPower )
{
	if( value )
	{
		if( !UTIL_isPowerOfTwo( value ) || ( valueCannotBeNextPower && value != quint64( 1 ) << ( ( sizeof( quint64 ) * 8 ) - 1 ) ) )
		{
			// -2 because maximum already checked for above
			for( quint64 index = quint64( 1 ) << ( ( sizeof( quint64 ) * 8 ) - 2 ) ; index ; index >>= 1 )
				if( index & value )
					return index << 1;
		}

		return value;
	}

	return 1;
}

quint64 UTIL_lastPowerOfTwo( const quint64 &value , bool valueCannotBeLastPower )
{
	if( value )
	{
		if( !UTIL_isPowerOfTwo( value ) || ( valueCannotBeLastPower && value != quint64( 1 ) << ( ( sizeof( quint64 ) * 8 ) - 1 ) ) )
		{
			// -2 because maximum already checked for above
			for( quint64 index = quint64( 1 ) << ( ( sizeof( quint64 ) * 8 ) - 2 ) ; index ; index >>= 1 )
				if( index & value )
					return index;
		}

		return value;
	}

	return 1;
}

quint64 UTIL_nearestPowerOfTwo( const quint64 &value )
{
	if( !UTIL_isPowerOfTwo( value ) )
	{
		quint64 next = UTIL_nextPowerOfTwo( value , false );
		quint64 last = UTIL_lastPowerOfTwo( value , false );

		return UTIL_lowerLimit( ( ( value - last ) < ( next - value ) ) ? last : next , (quint64)1 );
	}

	return value;
}

unsigned char UTIL_flagToIndex( quint64 flag )
{
	if( UTIL_isPowerOfTwo( flag ) )
		for( unsigned char index = 0 ; index < 64 ; index++ )
			if( flag == 1 << index )
				return index;

	return (unsigned char)-1;
}
