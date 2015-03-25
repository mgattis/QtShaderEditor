#ifndef COBJ_H
#define COBJ_H

#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "tinyobjloader/tiny_obj_loader.h"

#include <QByteArray>
#include <QFile>
#include <QImage>
#include <QMap>
#include <QString>
#include <QVector>

struct CGLMesh
{
	CGLMesh( int vertexCount );
	~CGLMesh();

	void draw( void );

	int vertexCount;

	GLint vertexLoc;
	GLuint vertexBuf;
	float (*vertices)[ 3 ];

	GLint normalLoc;
	GLuint normalBuf;
	float (*normals)[ 3 ];

	GLint texCoordLoc;
	GLuint texCoordBuf;
	float (*texCoords)[ 2 ];

	GLint ambientLoc;
	GLuint ambientBuf;
	GLuint (*ambientID);

	GLint diffuseLoc;
	GLuint diffuseBuf;
	GLuint (*diffuseID);

	GLint specularLoc;
	GLuint specularBuf;
	GLuint (*specularID);

	GLint normalMapLoc;
	GLuint normalMapBuf;
	GLuint (*normalMapID);

	GLuint texID;
};

class CObj
{
private:
	CObj( void );
	~CObj();

public:
	QVector< tinyobj::material_t* > materialArray;
	QVector< CGLMesh* > meshArray;
	QMap< QString , GLuint > texMap;

public:
	static CObj* loadFromPath( const QString &fileName , const QString &basePath = QString() )
	{
		// The OBJ library requires the texture path to end in a slash
		QString texturePath = basePath;
		if( !texturePath.isEmpty() && !texturePath.endsWith( '/'  ) )
			texturePath += "/";

		CObj *obj = new CObj;

		std::vector< tinyobj::shape_t > shapes;
		std::vector< tinyobj::material_t > materials;
		std::string err = tinyobj::LoadObj( shapes , materials , fileName.toLatin1().data() , texturePath.toLatin1().data() );

		if( err.empty() )
		{
			CGLMesh *mesh = NULL;
			obj->meshArray.resize( shapes.size() );

			for( size_t shapeIndex = 0 ; shapeIndex < shapes.size() ; shapeIndex++ )
			{
				obj->meshArray[ shapeIndex ] = mesh = new CGLMesh( shapes[ shapeIndex ].mesh.indices.size() );

				// Per triangle
				for( size_t tri = 0 ; tri < shapes[ shapeIndex ].mesh.indices.size() / 3 ; tri++ )
				{
					int materialIndex = shapes[ shapeIndex ].mesh.material_ids[ tri ];
					//std::cerr << "material id:" << materialIndex << std::endl;

					//std::cout << "ambient name:" << ambientName.data() << std::endl;
					//std::cout << "diffuse name:" << diffuseName.data() << std::endl;
					//std::cout << "specular name:" << specularName.data() << std::endl;
					//std::cout << "normal name:" << normalName.data() << std::endl;

					// Per texture
					for( int index = 0 ; index < 4 ; index++ )
					{
						QString materialName;

						switch( index )
						{
							case 0:
								materialName = materials[ materialIndex ].ambient_texname.data();
								break;
							case 1:
								materialName = materials[ materialIndex ].diffuse_texname.data();
								//materialName = "lambertian.jpg";
								break;
							case 2:
								materialName = materials[ materialIndex ].specular_texname.data();
								break;
							case 3:
								materialName = materials[ materialIndex ].normal_texname.data();
								break;
							default: {}
						}

						// Skip if no texture
						if( !materialName.isEmpty() )
						{
							GLuint texID = 0;

							if( !obj->texMap.contains( materialName ) )
							{
								QString path = texturePath + materialName;
								std::cout << "Loading: " << path.toLatin1().data() << std::endl;

								//QImage *image = new QImage( "/home/terrenteller/Projects/QtShaderEditor/resources/grassside.png" );
								// Load image
								QImage *image = new QImage( path );
								*image = image->mirrored( 0 , 1 );
								*image = image->convertToFormat( QImage::Format_RGB888 );

								// Load texture into OpenGL
								glGenTextures( 1 , &texID );
								glBindTexture( GL_TEXTURE_2D , texID );
								glTexImage2D( GL_TEXTURE_2D , 0 , GL_RGB , image->width() , image->height() , 0 , GL_RGB , GL_UNSIGNED_BYTE , ( (const QImage*)image )->bits() );
								glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_MAG_FILTER , GL_NEAREST );
								glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_MIN_FILTER , GL_NEAREST );
								delete image;

								while( GLenum errorEnum = glGetError() )
									std::cout << "Texture Load Error: " << gluErrorString( errorEnum ) << std::endl;

								obj->texMap[ materialName ] = texID;
							}
							else
								texID = obj->texMap.value( materialName , 0 );

							// Set the ID
							GLuint *idArray = new GLuint[ 1 ];
							obj->meshArray.at( shapeIndex )->texID = idArray[ 0 ] = texID;
							//std::cout << "Using: " << texID << std::endl;

							switch( index )
							{
								case 0:
									obj->meshArray[ shapeIndex ]->ambientID = idArray;
									break;
								case 1:
									obj->meshArray[ shapeIndex ]->diffuseID = idArray;
									break;
								case 2:
									obj->meshArray[ shapeIndex ]->specularID = idArray;
									break;
								case 3:
									obj->meshArray[ shapeIndex ]->normalMapID = idArray;
									break;
								default: {}
							}
						}
					}

					// Per vertex
					for( int vertex = 0 ; vertex < 3 ; vertex++ )
					{
						int vertexIndex = shapes[ shapeIndex ].mesh.indices[ 3 * tri + vertex ];

						// Per dimension
						for( int dim = 0 ; dim < 3 ; dim++ )
						{
							mesh->vertices[ 3 * tri + vertex ][ dim ] = shapes[ shapeIndex ].mesh.positions[ 3 * vertexIndex + dim ];

							// Some files may not provide normals
							if( ( vertexIndex * 3 ) + 3 <= shapes[ shapeIndex ].mesh.normals.size() )
								mesh->normals[ 3 * tri + vertex ][ dim ] = shapes[ shapeIndex ].mesh.normals[ 3 * vertexIndex + dim ];
						}

						// If normals are not provided, compute them here
						if( ( vertexIndex * 3 ) + 3 > shapes[ shapeIndex ].mesh.normals.size() )
						{
							glm::vec3 normalVec( mesh->vertices[ 3 * tri + vertex ][ 0 ] , mesh->vertices[ 3 * tri + vertex ][ 1 ] , mesh->vertices[ 3 * tri + vertex ][ 2 ] );
							normalVec = glm::normalize( normalVec );

							mesh->normals[ 3 * tri + vertex ][ 0 ] = normalVec.x;
							mesh->normals[ 3 * tri + vertex ][ 1 ] = normalVec.y;
							mesh->normals[ 3 * tri + vertex ][ 2 ] = normalVec.z;
						}

						for( int dim = 0 ; dim < 2 ; dim++ )
							mesh->texCoords[ 3 * tri + vertex ][ dim ] = shapes[ shapeIndex ].mesh.texcoords[ 2 * vertexIndex + dim ];
					}
				}
			}

			obj->materialArray.resize( materials.size() );
			tinyobj::material_t *material;

			for( size_t matIndex = 0 ; matIndex < materials.size() ; matIndex++ )
			{
				obj->materialArray[ matIndex ] = material = new tinyobj::material_t( materials[ matIndex ] );
			}
		}
		else
		{
			std::cerr << err << std::endl;
			delete obj;
			obj = NULL;
		}

		return obj;
	}
};

#endif // COBJ_H
