#include "CObj.h"

static GLint CGLMesh::vertexLocShared;
static GLint CGLMesh::normalLocShared;
static GLint CGLMesh::texCoordLocShared;

CGLMesh::CGLMesh( int vertexCount )
{
	this->vertexCount = vertexCount;

	vertices = new float[ vertexCount ][ 3 ];
	normals = new float[ vertexCount ][ 3 ];
	texCoords = new float[ vertexCount ][ 2 ];

	vertexBuf = normalBuf = texCoordBuf = 0;
	ambientID = diffuseID = specularID = normalMapID = NULL;
}

CGLMesh::~CGLMesh()
{
	// FIXME: This segfaults
	//glDeleteBuffers( 1 , vertexBuf );
	//glDeleteBuffers( 1 , normalBuf );
	//glDeleteBuffers( 1 , texCoordBuf );

	//glDeleteTextures( 1 , ambientID );
	//glDeleteTextures( 1 , diffuseID );
	//glDeleteTextures( 1 , specularID );
	//glDeleteTextures( 1 , normalMapID );

	delete[] ambientID;
	delete[] diffuseID;
	delete[] specularID;
	delete[] normalMapID;
}

void CGLMesh::build( void )
{
	// This looks weird, but this function may be called many, many times.
	// A little optimization can go a long way
	int bufIndex = 0 , bufCount = (bool)vertices + (bool)normals + (bool)texCoords;
	GLuint buf[ bufCount ];
	glGenBuffers( bufCount , &buf[ 0 ] );

	if( vertices )
	{
		glBindBuffer( GL_ARRAY_BUFFER , buf[ bufIndex ] );
		glBufferData( GL_ARRAY_BUFFER , vertexCount * sizeof( *vertices ) , vertices , GL_STATIC_DRAW );
		vertexBuf = buf[ bufIndex ];
		bufIndex++;
	}

	if( normals )
	{
		glBindBuffer( GL_ARRAY_BUFFER , buf[ bufIndex ] );
		glBufferData( GL_ARRAY_BUFFER , vertexCount * sizeof( *normals ) , normals , GL_STATIC_DRAW );
		normalBuf = buf[ bufIndex ];
		bufIndex++;
	}

	if( texCoords )
	{
		glBindBuffer( GL_ARRAY_BUFFER , buf[ bufIndex ] );
		glBufferData( GL_ARRAY_BUFFER , vertexCount * sizeof( *texCoords ) , texCoords , GL_STATIC_DRAW );
		texCoordBuf = buf[ bufIndex ];
		bufIndex++;
	}
}

void CGLMesh::draw( void )
{
	glEnableVertexAttribArray( vertexLoc );
	glBindBuffer( GL_ARRAY_BUFFER , vertexBuf );
	glVertexAttribPointer( vertexLoc , 3 , GL_FLOAT , GL_FALSE , 0 , 0 );

	glEnableVertexAttribArray( normalLoc );
	glBindBuffer( GL_ARRAY_BUFFER , normalBuf );
	glVertexAttribPointer( normalLoc , 3 , GL_FLOAT , GL_FALSE , 0 , 0 );

	glEnableVertexAttribArray( texCoordLoc );
	glBindBuffer( GL_ARRAY_BUFFER , texCoordBuf );
	glVertexAttribPointer( texCoordLoc , 2 , GL_FLOAT , GL_FALSE , 0 , 0 );

#if 1
	if( diffuseID )
	{
		glActiveTexture( GL_TEXTURE0 );
		glBindTexture( GL_TEXTURE_2D , diffuseID[ 0 ] );
	}
#endif

	glDrawArrays( GL_TRIANGLES , 0 , ( ( vertexCount * sizeof( *vertices ) ) / 3 ) / sizeof( GLfloat ) );
}

void CGLMesh::clear( void )
{
	delete[] vertices;
	vertices = NULL;

	delete[] normals;
	normals = NULL;

	delete[] texCoords;
	texCoords = NULL;
}

CObj::CObj()
{
	// Nothing to do
}

CObj::~CObj()
{
	for( int index = 0 ; index < meshArray.size() ; index++ )
		delete meshArray.at( index );

	for( int index = 0 ; index < materialArray.size() ; index++ )
		delete materialArray.at( index );
}

void CObj::build( void )
{
	for( int index = 0 ; index < meshArray.size() ; index++ )
	{
		meshArray.at( index )->build();
		meshArray.at( index )->clear();
	}
}

void CObj::draw( GLuint vertexLoc /* = -1 */ , GLuint normalLoc /* = -1 */ , GLuint uvLoc /* = -1 */ )
{
	if( meshArray.size() )
	{
		CGLMesh::vertexLocShared = vertexLoc;
		CGLMesh::normalLocShared = normalLoc;
		CGLMesh::texCoordLocShared = uvLoc;

		for( int index = 0 ; index < meshArray.size() ; index++ )
			meshArray.at( index )->draw();
	}
}
