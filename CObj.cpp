#include "CObj.h"

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

void CGLMesh::draw( void )
{
	glEnableVertexAttribArray( vertexLoc );
	glBindBuffer( GL_ARRAY_BUFFER , vertexBuf );
	glVertexAttribPointer( vertexLoc , 3 , GL_FLOAT , GL_FALSE , 0 , 0 );

	//glEnableVertexAttribArray( normalLoc );
	//glBindBuffer( GL_ARRAY_BUFFER , normalBuf );
	//glVertexAttribPointer( normalLoc , 3 , GL_FLOAT , GL_FALSE , 0 , 0 );

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
