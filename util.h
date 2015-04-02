// =======================================================
// Generic Utility Code
// =======================================================
// Purpose: Collection of randomly useful code
//********************************************************

#include <iostream>
#include <string>
#include <vector>

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

typedef unsigned long long int quint64;

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
