#include "CObj.h"

CGLMesh::CGLMesh( int vertexCount )
{
	this->vertexCount = vertexCount;

	vertices = new float[ vertexCount ][ 3 ];
	normals = new float[ vertexCount ][ 3 ];
	texCoords = new float[ vertexCount ][ 2 ];

	ambientID = NULL;
	diffuseID = NULL;
	specularID = NULL;
	normalMapID = NULL;
}

CGLMesh::~CGLMesh()
{
	delete[] vertices;
	delete[] normals;
	delete[] texCoords;

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

	//glActiveTexture( GL_TEXTURE0 );
	//glBindTexture( GL_TEXTURE_2D , texID );

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
	//vertexBuf = normalBuf = texCoordBuf = 0;
	//ambientBuf = diffuseBuf = specularBuf = normalMapBuf = 0;
}

CObj::~CObj()
{
	CGLMesh *mesh = NULL;
	for( int index = 0 ; index < meshArray.size() ; index++ )
	{
		mesh = meshArray.at( index );

		glDeleteBuffers( 1 , mesh->vertexBuf );
		glDeleteBuffers( 1 , mesh->normalBuf );
		glDeleteBuffers( 1 , mesh->texCoordBuf );
		glDeleteBuffers( 1 , mesh->ambientBuf );
		glDeleteBuffers( 1 , mesh->diffuseBuf );
		glDeleteBuffers( 1 , mesh->specularBuf );
		glDeleteBuffers( 1 , mesh->normalMapBuf );

		delete meshArray.at( index );
	}

	for( int index = 0 ; index < materialArray.size() ; index++ )
		delete materialArray.at( index );
}

