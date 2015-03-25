#include "VGLView.h"

VGLView::VGLView( QWidget *parent /* = NULL */ )
{
	keyBits = 0;
	mouseSensitivity = 10.0;
	moveSpeed = 20.0;
	fov = 45.0;
	xPos = yPos = zPos = 0.0;
	xRot = yRot = zRot = 0.0;

	this->setMouseTracking( true );

	QFile file;
	file.setFileName( "/home/terrenteller/Projects/QtShaderEditor/resources/vertexShader.glsl" );
	if( file.open( QFile::ReadOnly ) )
	{
		QByteArray data = file.readAll();
		vertShader = new char[ data.size() ];
		memcpy( vertShader , data , data.size() );

		file.close();
	}

	file.setFileName( "/home/terrenteller/Projects/QtShaderEditor/resources/fragmentShader.glsl" );
	if( file.open( QFile::ReadOnly ) )
	{
		QByteArray data = file.readAll();
		fragShader = new char[ data.size() ];
		memcpy( fragShader , data , data.size() );

		file.close();
	}

	//vertShader = "#version 430\n\nin vec3 in_Position;\n\nvoid main()\n{\ngl_Position = vec4(in_Position.x, in_Position.y, in_Position.z, 1.0);\n}\n";
	//vertShader = "#version 430\n\nuniform mat4 camera;\nuniform mat4 model;\nin vec3 in_Position;\n\nvoid main()\n{\ngl_Position = camera * vec4( in_Position, 1.0);\n}\n";
	//fragShader = "#version 430\n\nprecision highp float;\n\nout vec4 fragColor;\n\nvoid main()\n{\nfragColor = vec4(1.0,1.0,1.0,1.0);\n}\n";

	/*
	vertShader = "#version 430"
	""
	"uniform mat4 camera;"
	"//uniform mat4 model;"
	""
	"in vec3 in_Position;"
	"//in vec2 UV;"
	""
	"//out vec3 colour;"
	"//out vec2 fragUV;"
	""
	"void main()"
	"{"
	"	//fragUV = UV;"
	"	//fragUV = in_Position.xy;"
	""
	"	//gl_Position = camera * vec4( 0.0 , 0.0 , 0.0 , 1.0 );"
	"	gl_Position = camera * vec4( in_Position , 1.0 );"
	"}";
	*/

	// TEST

	//QDir::setCurrent( "/home/terrenteller/Projects/QtShaderEditor/resources/" );
	//QString currentDir = QDir::current().absolutePath();

	const char *filename = "/home/terrenteller/Projects/QtShaderEditor/resources/grassblock.obj";
	const char *basepath = "/home/terrenteller/Projects/QtShaderEditor/resources/";
	std::cout << "Loading " << filename << std::endl;
	//std::cout << "Current: " << currentDir.toLatin1().data() << std::endl;

	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string err = tinyobj::LoadObj(shapes, materials, filename, basepath);

	if( !err.empty() )
	{
		std::cerr << err << std::endl;
		return;
	}

	//mesh = new CGLMesh( 6 );

	//size_t shapeIndex = 2;
	meshArray.resize( shapes.size() );
	for( size_t shapeIndex = 0 ; shapeIndex < shapes.size() ; shapeIndex++ )
	{
		meshArray[ shapeIndex ] = mesh = new CGLMesh( shapes[ shapeIndex ].mesh.indices.size() );

		// Per triangle
		for( size_t tri = 0 ; tri < shapes[ shapeIndex ].mesh.indices.size() / 3 ; tri++ )
		{
#if 0
			int materialIndex = shapes[ shapeIndex ].mesh.material_ids[ tri ];
			std::cerr << "material id:" << materialIndex << std::endl;

			std::string ambientName = materials[ materialIndex ].ambient_texname;
			std::string diffuseName = materials[ materialIndex ].diffuse_texname;
			std::string specularName = materials[ materialIndex ].specular_texname;
			std::string normalName = materials[ materialIndex ].normal_texname;

			std::cerr << "ambient name:" << ambientName.data() << std::endl;
			std::cerr << "diffuse name:" << diffuseName.data() << std::endl;
			std::cerr << "specular name:" << specularName.data() << std::endl;
			std::cerr << "normal name:" << normalName.data() << std::endl;
#endif

			// Per vertex
			for( int vertex = 0 ; vertex < 3 ; vertex++ )
			{
				int vertexIndex = shapes[ shapeIndex ].mesh.indices[ 3 * tri + vertex ];

				// Per dimension
				for( int dim = 0 ; dim < 3 ; dim++ )
				{
					//data[ 3 * tri + vertex ][ dim ] = shapes[ shapeIndex ].mesh.positions[ 3 * vertexIndex + dim ];
					//normal[ 3 * tri + vertex ][ dim ] = shapes[ shapeIndex ].mesh.normals[ 3 * vertexIndex + dim ];

					mesh->vertices[ 3 * tri + vertex ][ dim ] = shapes[ shapeIndex ].mesh.positions[ 3 * vertexIndex + dim ];
					mesh->normals[ 3 * tri + vertex ][ dim ] = shapes[ shapeIndex ].mesh.normals[ 3 * vertexIndex + dim ];
				}

				for( int dim = 0 ; dim < 2 ; dim++ )
				{
					//tex[ 3 * tri + vertex ][ dim ] = shapes[ shapeIndex ].mesh.texcoords[ 2 * vertexIndex + dim ];
					mesh->texCoords[ 3 * tri + vertex ][ dim ] = shapes[ shapeIndex ].mesh.texcoords[ 2 * vertexIndex + dim ];
				}
			}
		}
	}

	mesh = meshArray.at( 2 );

#if 0
	std::cout << "texcoords: " << shapes[ shapeIndex ].mesh.texcoords.size() << std::endl;

	for( int index = 0 ; index < 6 ; index++ )
	{
		printf("vert(%f, %f, %f)\n",
		data[ index ][ 0 ],
		data[ index ][ 1 ],
		data[ index ][ 2 ]);
	}

	for( int index = 0 ; index < 6 ; index++ )
	{
		printf("normal(%f, %f, %f)\n",
		normal[ index ][ 0 ],
		normal[ index ][ 1 ],
		normal[ index ][ 2 ]);
	}

	for( int index = 0 ; index < 6 ; index++ )
	{
		printf("tex(%f, %f)\n",
		tex[ index ][ 0 ],
		tex[ index ][ 1 ]);
	}

	for( int index = 0 ; index < 6 ; index++ )
	{
		printf("vert(%f, %f, %f)\n",
		mesh.vertices[ index ][ 0 ],
		mesh.vertices[ index ][ 1 ],
		mesh.vertices[ index ][ 2 ]);
	}

	for( int index = 0 ; index < 6 ; index++ )
	{
		printf("normal(%f, %f, %f)\n",
		mesh.normals[ index ][ 0 ],
		mesh.normals[ index ][ 1 ],
		mesh.normals[ index ][ 2 ]);
	}

	for( int index = 0 ; index < 6 ; index++ )
	{
		printf("tex(%f, %f)\n",
		mesh.texCoords[ index ][ 0 ],
		mesh.texCoords[ index ][ 1 ]);
	}
#endif

	shaderAttribute = 0;
}

VGLView::~VGLView()
{
	repaintTimer.stop();
	makeCurrent();

	glDeleteProgram( shaderProgram );
	glDeleteShader( fragmentShader );
	glDeleteShader( vertexShader );

	//glDeleteBuffers( 1 , &eboID );
	glDeleteBuffers( 2 , &vboID[ 0 ] );

	glDeleteVertexArrays( 1 , &vaoID );

	delete vertShader;
	delete fragShader;

	//delete texture;
	delete image;
	delete mesh;
}

#if 0
void VGLView::setVertexShader( QGLShader *shader )
{
	program->release();
	program->removeShader( vertexShader );
	delete vertexShader;
	vertexShader = shader;
	program->addShader( shader );
	program->bind();
}

void VGLView::setFragmentShader( QGLShader *shader )
{
	program->release();
	program->removeShader( fragmentShader );
	delete fragmentShader;
	fragmentShader = shader;
	program->addShader( shader );
	program->bind();
}
#endif

void VGLView::initializeGL( void )
{
	static bool complete = false;

	if( !complete )
	{
		connect( &repaintTimer , SIGNAL(timeout()) , this , SLOT(updateGL()) );
		repaintTimer.setInterval( 33 );
		repaintTimer.start();

		glewInit();

		glGenVertexArrays( 1 , &vaoID );
		glBindVertexArray( vaoID );

		glGenBuffers( 2 , &vboID[ 0 ] );

		//const char *filename = "/home/terrenteller/Projects/QtShaderEditor/resources/grassblock.obj";
		//const char *basepath = "/home/terrenteller/Projects/QtShaderEditor/resources";
		//const char *filename = "/home/terrenteller/Projects/QtShaderEditor/resources/minecraft.obj";
		//const char *basepath = "/home/terrenteller/Projects/QtShaderEditor/resources";
		//const char *filename = "/home/terrenteller/Projects/QtShaderEditor/resources/dabrovic-sponza/sponza.obj";
		//const char *basepath = "/home/terrenteller/Projects/QtShaderEditor/resources/dabrovic-sponza";
		//const char *filename = "/home/terrenteller/Projects/QtShaderEditor/resources/head/head.OBJ";
		//const char *basepath = "/home/terrenteller/Projects/QtShaderEditor/resources/head";
		const char *filename = "/home/terrenteller/Projects/QtShaderEditor/resources/lost-empire/lost_empire.obj";
		const char *basepath = "/home/terrenteller/Projects/QtShaderEditor/resources/lost-empire";

		obj = CObj::loadFromPath( filename , basepath );
		std::cout << "obj: " << obj << std::endl;

#if 1
		for( int index = 0 ; index < meshArray.size() ; index++ )
		{
			GLuint buf[ 2 ];
			glGenBuffers( 2 , &buf[ 0 ] );

			glBindBuffer( GL_ARRAY_BUFFER , buf[ 0 ] );
			glBufferData( GL_ARRAY_BUFFER , meshArray.at( index )->vertexCount * sizeof( *meshArray.at( index )->vertices ) , meshArray.at( index )->vertices , GL_STATIC_DRAW );
			meshArray.at( index )->vertexBuf = buf[ 0 ];

			glBindBuffer( GL_ARRAY_BUFFER , buf[ 1 ] );
			glBufferData( GL_ARRAY_BUFFER , meshArray.at( index )->vertexCount * sizeof( *meshArray.at( index )->texCoords ) , meshArray.at( index )->texCoords , GL_STATIC_DRAW );
			meshArray.at( index )->texCoordBuf = buf[ 1 ];
		}

		for( int index = 0 ; index < obj->meshArray.size() ; index++ )
		{
			GLuint buf[ 2 ];
			glGenBuffers( 2 , &buf[ 0 ] );

			glBindBuffer( GL_ARRAY_BUFFER , buf[ 0 ] );
			glBufferData( GL_ARRAY_BUFFER , obj->meshArray.at( index )->vertexCount * sizeof( *obj->meshArray.at( index )->vertices ) , obj->meshArray.at( index )->vertices , GL_STATIC_DRAW );
			obj->meshArray.at( index )->vertexBuf = buf[ 0 ];

			glBindBuffer( GL_ARRAY_BUFFER , buf[ 1 ] );
			glBufferData( GL_ARRAY_BUFFER , obj->meshArray.at( index )->vertexCount * sizeof( *obj->meshArray.at( index )->texCoords ) , obj->meshArray.at( index )->texCoords , GL_STATIC_DRAW );
			obj->meshArray.at( index )->texCoordBuf = buf[ 1 ];
		}

		mesh = meshArray.at( 2 );
#else
		glBindBuffer( GL_ARRAY_BUFFER , vboID[ 0 ] );
		mesh->vertexBuf = vboID[ 0 ];
		glBufferData( GL_ARRAY_BUFFER , mesh->vertexCount * sizeof( *mesh->vertices ) , mesh->vertices , GL_STATIC_DRAW );


		// Proof that the loading code shouldn't work... but it does
		//std::cout << "sizeof: " << sizeof( data ) << " , " << sizeof( data ) / 3 << std::endl;
		//std::cout << "sizeof: " << mesh->vertexCount * sizeof( *mesh->vertices ) << " , " << ( mesh->vertexCount * sizeof( *mesh->vertices ) ) / 3 << std::endl;

		//glVertexAttribPointer( shaderAttribute , 3 , GL_FLOAT , GL_FALSE , 0 , 0 );
		//glEnableVertexAttribArray( shaderAttribute );
		//glBindBuffer( GL_ARRAY_BUFFER , vboID[ 0 ] );

		glBindBuffer( GL_ARRAY_BUFFER , vboID[ 1 ] );
		mesh->texCoordBuf = vboID[ 1 ];
		glBufferData( GL_ARRAY_BUFFER , mesh->vertexCount * sizeof( *mesh->texCoords ) , mesh->texCoords , GL_STATIC_DRAW );
#endif

		/*
		GLfloat vertices[] = {
			-0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // Top-left
			 0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // Top-right
			 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // Bottom-right
			-0.5f, -0.5f, 1.0f, 1.0f, 1.0f  // Bottom-left
		};
		*/

		//glBufferData( GL_ARRAY_BUFFER , sizeof( vertices ) , vertices , GL_STATIC_DRAW );

#if 0
		glGenBuffers( 1 , &eboID );

		GLuint elements[] = {
			0, 1, 2,
			2, 3, 0
		};

		//GLuint elements[] = { 0 , 1 , 2 , 3 , 4 , 5 };
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER , eboID );
		glBufferData( GL_ELEMENT_ARRAY_BUFFER , sizeof( elements ) , elements , GL_STATIC_DRAW );
#endif

		// Make the shaders

		vertexShader = glCreateShader( GL_VERTEX_SHADER );
		fragmentShader = glCreateShader( GL_FRAGMENT_SHADER );

		glShaderSource( vertexShader , 1 , (const GLchar**)( &vertShader ) , 0 );
		glShaderSource( fragmentShader , 1 , (const GLchar**)( &fragShader ) , 0 );

		GLenum errorEnum;
		while( errorEnum = glGetError() )
			std::cout << "GL Shader Source Error: " << gluErrorString( errorEnum ) << std::endl;

		glCompileShader( vertexShader );
		glCompileShader( fragmentShader );

		while( errorEnum = glGetError() )
			std::cout << "GL Compile Error: " << gluErrorString( errorEnum ) << std::endl;

		GLint shaderCompiled;

		glGetShaderiv( vertexShader , GL_COMPILE_STATUS , &shaderCompiled );
		if( shaderCompiled == GL_FALSE )
		{
			GLint maxLength = 0;
			glGetShaderiv( vertexShader , GL_INFO_LOG_LENGTH , &maxLength );

			// The maxLength includes the NULL character
			std::vector< GLchar > errorLog( maxLength );
			glGetShaderInfoLog( vertexShader , maxLength , &maxLength , &errorLog[ 0 ] );

			std::cout << (const char*)errorLog.data() << std::endl;

			std::cout << "Error: Vertex shader (below) did not compile:\n" << vertShader << std::endl;
		}

		glGetShaderiv( fragmentShader , GL_COMPILE_STATUS , &shaderCompiled );
		if( shaderCompiled == GL_FALSE )
		{
			GLint maxLength = 0;
			glGetShaderiv( fragmentShader , GL_INFO_LOG_LENGTH , &maxLength );

			// The maxLength includes the NULL character
			std::vector< GLchar > errorLog( maxLength );
			glGetShaderInfoLog( fragmentShader , maxLength , &maxLength , &errorLog[ 0 ] );

			std::cout << (const char*)errorLog.data() << std::endl;

			std::cout << "Error: Fragment shader (below) did not compile:\n" << fragShader << std::endl;
		}

		shaderProgram = glCreateProgram();
		glAttachShader( shaderProgram , vertexShader );
		glAttachShader( shaderProgram , fragmentShader );

		while( errorEnum = glGetError() )
			std::cout << "GL Attach Error: " << gluErrorString( errorEnum ) << std::endl;

		//glBindFragDataLocation( shaderProgram , 0 , "outColor" );
		//glBindAttribLocation( shaderProgram , shaderAttribute , "vertex" );

		glLinkProgram( shaderProgram );
		//glUseProgram( shaderProgram );

		while( errorEnum = glGetError() )
			std::cout << "GL Link/Use Error: " << gluErrorString( errorEnum ) << std::endl;

		image = new QImage( "/home/terrenteller/Projects/QtShaderEditor/resources/grassside.png" );
		*image = image->mirrored( 0 , 1 );
		*image = image->convertToFormat( QImage::Format_RGB888 );

		glGenTextures( 1 , &textureID );
		glBindTexture( GL_TEXTURE_2D , textureID );
		glTexImage2D( GL_TEXTURE_2D , 0 , GL_RGB , image->width() , image->height() , 0 , GL_RGB , GL_UNSIGNED_BYTE , ( (const QImage*)image )->bits() );
		glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_MAG_FILTER , GL_NEAREST );
		glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_MIN_FILTER , GL_NEAREST );

		complete = true;
	}
}

void VGLView::paintGL( void )
{
	makeCurrent();
	//program->bind();

	// Stuff

	static int totalFrames = 0;
	totalFrames++;
	static QTime frameTimer;
	static float totalTime = 0.0;
	static float lastFrameTime;
	lastFrameTime = frameTimer.restart() / 1000.0;
	totalTime += lastFrameTime;

	glLoadIdentity();

	// Position Camera

	//glRotatef( xRot , 1.0 , 0.0 , 0.0 );
	//glRotatef( yRot , 0.0 , 1.0 , 0.0 );

	if( ( keyBits & KEY_W ) ^ ( keyBits & KEY_S ) )
	{
		if( keyBits & KEY_W )
		{
			xPos -= lastFrameTime * moveSpeed * cos( xRot * DEG2RAD ) * sin( yRot * DEG2RAD );// * cos( zRot * DEG2RAD );
			yPos += lastFrameTime * moveSpeed * sin( xRot * DEG2RAD );// * cos( zRot * DEG2RAD );
			zPos += lastFrameTime * moveSpeed * cos( xRot * DEG2RAD ) * cos( yRot * DEG2RAD );
		}
		else
		{
			xPos += lastFrameTime * moveSpeed * cos( xRot * DEG2RAD ) * sin( yRot * DEG2RAD );// * cos( zRot * DEG2RAD )
			yPos -= lastFrameTime * moveSpeed * sin( xRot * DEG2RAD );// * cos( zRot * DEG2RAD )
			zPos -= lastFrameTime * moveSpeed * cos( xRot * DEG2RAD ) * cos( yRot * DEG2RAD );
		}
	}

	if( ( keyBits & KEY_A ) ^ ( keyBits & KEY_D ) )
	{
		if( keyBits & KEY_A )
		{
			xPos += lastFrameTime * moveSpeed * cos( yRot * DEG2RAD );// * cos( zRot * DEG2RAD );
			//yPos -= lastFrameTime * moveSpeed * sin( zRot * DEG2RAD );
			zPos += lastFrameTime * moveSpeed * sin( yRot * DEG2RAD );// * cos( xRot * DEG2RAD )
		}
		else
		{
			xPos -= lastFrameTime * moveSpeed * cos( yRot * DEG2RAD );// * cos( zRot * DEG2RAD );
			//yPos += lastFrameTime * moveSpeed * sin( zRot * DEG2RAD );
			zPos -= lastFrameTime * moveSpeed * sin( yRot * DEG2RAD );// * cos( xRot * DEG2RAD )
		}
	}

	//glTranslatef( xPos , yPos , zPos );

	// Umm

	glm::mat4 orientation;
	orientation = glm::rotate( orientation , -xRot , glm::vec3( 1 , 0 , 0 ) );
	orientation = glm::rotate( orientation , -yRot , glm::vec3( 0 , 1 , 0 ) );

	glm::mat4 camera = glm::perspective( 80.0 , (double)this->width()/(double)this->height() , 0.1 , 100.0 );
	camera *= orientation;
	camera = glm::translate( camera , glm::vec3( -xPos , yPos , zPos ) );

	//camera = glm::perspective( 80.0 , -(double)this->width()/(double)this->height() , 0.1 , 100.0 );
	//camera = glm::translate( camera , glm::vec3( 0.0 , 0.0 , 3.0 ) );
	camera = glm::inverse( camera );

	glUseProgram( shaderProgram );

	GLenum errorEnum;
	if( !( totalFrames % 30 ) )
		while( errorEnum = glGetError() )
			std::cout << "GL Error: " << gluErrorString( errorEnum ) << std::endl;

#if 1
	/*
	GLint cameraLocation = glGetUniformLocation( shaderProgram , "camera" );
	if( !( totalFrames % 30 ) )
		std::cout << "camera: " << cameraLocation << std::endl;
	glUniformMatrix4fv( cameraLocation , 1 , GL_FALSE , &camera[ 0 ][ 0 ] );
	*/

	// Modelview

	glm::mat4 model;
	model = glm::rotate( model , glm::radians( 180.0f ), glm::vec3( 0.0f , 0.0f , 1.0f ) );
	GLint modelLoc = glGetUniformLocation( shaderProgram , "model" );
	if( !( totalFrames % 30 ) )
		std::cout << "model: " << modelLoc << std::endl;
	glUniformMatrix4fv( modelLoc , 1 , GL_FALSE , &model[ 0 ][ 0 ] );

	// View

	glm::mat4 view = glm::lookAt(
		glm::vec3( xPos , yPos - 2 , zPos ), // Camera position
		glm::vec3( 0.0f , 0.0f , 0.0f ), // What to look at
		glm::vec3( 0.0f , 1.0f , 0.0f )  // Up
	);
	GLint viewLoc = glGetUniformLocation( shaderProgram , "view" );
	if( !( totalFrames % 30 ) )
		std::cout << "view: " << viewLoc << std::endl;
	glUniformMatrix4fv( viewLoc , 1 , GL_FALSE , &view[ 0 ][ 0 ] );

	// Projection

	glm::mat4 proj = glm::perspective( 80.0 , (double)this->width()/(double)this->height() , 0.1 , 100.0 );
	GLint projLoc = glGetUniformLocation( shaderProgram , "projection" );
	if( !( totalFrames % 30 ) )
		std::cout << "proj: " << projLoc << std::endl;
	glUniformMatrix4fv( projLoc , 1 , GL_FALSE , &proj[ 0 ][ 0 ] );

	// Start drawing

	glClearColor( 0.0 , 0.0 , 0.0, 0.0 );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	for( int index = 0 ; index < obj->meshArray.size() ; index++ )
	{
#if 0
	// Vertex

	GLint vertexLoc = glGetAttribLocation( shaderProgram , "vertex" );
	if( !( totalFrames % 30 ) )
		std::cout << "vertex: " << vertexLoc << std::endl;
	glEnableVertexAttribArray( vertexLoc );
	glBindBuffer( GL_ARRAY_BUFFER , vboID[ 0 ] );
	//glVertexAttribPointer( vertexLoc , 3 , GL_FLOAT , GL_FALSE , 3 * sizeof( GLfloat ) , 0 );
	glVertexAttribPointer( vertexLoc , 3 , GL_FLOAT , GL_FALSE , 0 , 0 );
#endif
	obj->meshArray.at( index )->vertexLoc = glGetAttribLocation( shaderProgram , "vertex" );
	//mesh->vertexBuf = vboID[ 0 ];

#if 0
	GLint uvLoc = glGetAttribLocation( shaderProgram , "UV" );
	if( !( totalFrames % 30 ) )
		std::cout << "UV: " << uvLoc << std::endl;
	glEnableVertexAttribArray( uvLoc );
	glBindBuffer( GL_ARRAY_BUFFER , vboID[ 1 ] );
	//glVertexAttribPointer( uvLoc , 2 , GL_FLOAT , GL_FALSE , 2 * sizeof( float ) , 0 );
	glVertexAttribPointer( uvLoc , 2 , GL_FLOAT , GL_FALSE , 0 , 0 );
#endif
	obj->meshArray.at( index )->texCoordLoc = glGetAttribLocation( shaderProgram , "UV" );
	//mesh->texCoordBuf = vboID[ 1 ];

#if 0
	GLint texLocation = glGetUniformLocation( shaderProgram , "tex" );
	if( !( totalFrames % 30 ) )
		std::cout << "tex: " << texLocation << std::endl;
	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D , textureID );
	//glVertexAttribPointer( texLocation , 2 , GL_FLOAT , GL_FALSE , 2 * sizeof( float ) , 0 );
	//glVertexAttribPointer( texLocation , 2 , GL_FLOAT , GL_FALSE , 0 , 0 );
#endif
	//obj->meshArray.at( index )->specularLoc = glGetUniformLocation( shaderProgram , "tex" );

		obj->meshArray.at( index )->draw();
	}

	//glActiveTexture( GL_TEXTURE0 );
	//glBindTexture( GL_TEXTURE_2D , textureID );
	//glTexImage2D( GL_TEXTURE_2D , 0 , GL_RGB , image->width() , image->height() , 0 , GL_RGB , GL_UNSIGNED_BYTE , ( (const QImage*)image )->bits() );
	//glUniform1i( glGetUniformLocation( shaderProgram , "tex" ) , 0 );

	//texture->bind( glGetUniformLocation( shaderProgram , "tex" ) );
	//glUniform1i( glGetUniformLocation( shaderProgram , "tex" ) , 0 );

	//glTexImage2D( GL_TEXTURE_2D , 0 , texture->() , texture->width() , texture->height() , 0 ,  );

	//glDrawArrays( GL_TRIANGLES , 0 , ( ( mesh->vertexCount * sizeof( *mesh->vertices ) ) / 3 ) / sizeof( GLfloat ) );
	//mesh->draw();

	if( !( totalFrames % 30 ) )
	{
		//std::cout << "sizeof: " << sizeof( mesh->vertices ) << " , " << sizeof( mesh->vertices ) / 3 << std::endl;
		std::cout << "----------------------------------------" << std::endl;
	}

	//glDrawArrays( GL_TRIANGLES , 0 , 6 );
	//glDrawElements( GL_TRIANGLES , 6 , GL_UNSIGNED_INT , 0 );
#endif

#if 0
	GLint cameraLocation = glGetUniformLocation( shaderProgram , "camera" );
	std::cout << "camera: " << cameraLocation << std::endl;
	glUniformMatrix4fv( cameraLocation , 1 , GL_FALSE , &camera[ 0 ][ 0 ] );

	// Specify the layout of the vertex data
	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	std::cout << "position: " << posAttrib << std::endl;
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);

	//GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
	//std::cout << "color: " << colAttrib << std::endl;
	//glEnableVertexAttribArray(colAttrib);
	//glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));

	glDrawElements( GL_TRIANGLES , 6 , GL_UNSIGNED_INT , 0 );
#endif

	if( !( totalFrames % 30 ) )
	{
		while( errorEnum = glGetError() )
			std::cout << "GL Error: " << gluErrorString( errorEnum ) << std::endl;
		std::cout << "----------------------------------------" << std::endl;
	}

#if 0
	glBegin( GL_TRIANGLES );
		glColor3f( 1.0 , 0.0 , 0.0 );
		glVertex3f( -0.5 , -0.5 , -2.0 );
		glColor3f( 1.0 , 1.0 , 0.0 );
		glVertex3f( 0.5 , -0.5 , -2.0 );
		glColor3f( 0.0 , 0.0 , 1.0 );
		glVertex3f( 0.0 , 0.5 , -2.0 );
	glEnd();
#endif
}

void VGLView::resizeGL( int width , int height )
{
	glEnable( GL_DEPTH_TEST );
	glClearColor( 0.0 , 0.0 , 0.0, 0.0 );
	glDepthFunc( GL_LEQUAL );

	glViewport( 0 , 0 , (GLint)this->width() , (GLint)this->height() );
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective( 80.0 , (double)width/(double)height , 0.1 , 100.0 );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
}

void VGLView::enterEvent( QEvent *event )
{
	grabKeyboard();
}

void VGLView::leaveEvent( QEvent *event )
{
	keyBits = 0;
	releaseKeyboard();
	releaseMouse();
}

void VGLView::mousePressEvent( QMouseEvent *event )
{
	if( event )
	{
		lastCursorPos = event->pos();

		//if( event->button() & Qt::LeftButton )
			//emit mouseLeftClick( event->x() , event->y() , event->modifiers() );
	}
}

void VGLView::mouseMoveEvent( QMouseEvent *event )
{
	if( event )
	{
		//if( this->rect().contains( event->pos() ) )
			//this->grabKeyboard();

		if( event->buttons() & Qt::RightButton )
		{
			// Grab & Drag

			//xRot += lastPos.y() - event->pos().y();
			//yRot += lastPos.x() - event->pos().x();

			// FPS Style

			xRot += ( event->pos().y() - lastCursorPos.y() ) / mouseSensitivity;
			yRot += ( event->pos().x() - lastCursorPos.x() ) / mouseSensitivity;

			// Clamp

			if( xRot > 90.0 )
				xRot = 90.0;
			else if( xRot < -90.0 )
				xRot = -90.0;

			lastCursorPos = event->pos();
			event->accept();
		}
#if 0
		else if( event->buttons() & Qt::RightButton )
		{
			xPos += ( event->pos().y() - lastCursorPos.y() ) / 100;
			if( xPos > 5.0 )
				xPos = 5.0;
			else if( xPos < -5.0 )
				xPos = -5.0;

			lastCursorPos = event->pos();
			event->accept();
			repaint();
			return;

			xPos += ( event->pos().y() - lastCursorPos.y() ) / mouseSensitivity * 10;
			yPos += ( event->pos().x() - lastCursorPos.x() ) / mouseSensitivity * 10;

			if( xPos > 5.0 )
				xPos = 5.0;
			else if( xPos < -5.0 )
				xPos = -5.0;

			if( yPos > 5.0 )
				yPos = 5.0;
			else if( yPos < -5.0 )
				yPos = -5.0;

			lastCursorPos = event->pos();
			event->accept();

			repaint();
		}
#endif
		//else if( !this->rect().contains( event->pos() ) )
			//this->releaseKeyboard();

		lastCursorPos = event->pos();
	}
}

void VGLView::mouseReleaseEvent( QMouseEvent *event )
{
#if 0
	if( lastCursorPos == event->pos() && event->button() == Qt::LeftButton )
	{
		QVector< GLuint > hitLog;
		QVector< GLuint > zLog;

		GLuint buff[ 64 ] = { 0 };
		GLint hits , viewport[ 4 ];

		glSelectBuffer( meshes->size() , buff ); // This choose the buffer where store the values for the selection data
		glGetIntegerv( GL_VIEWPORT , viewport ); // This retrieve info about the viewport

		glMatrixMode( GL_PROJECTION ); // Now modify the vieving volume, restricting selection area around the cursor
		glPushMatrix();
		glLoadIdentity();

		gluPickMatrix( GLdouble( event->pos().x() ) , GLdouble( viewport[ 3 ] - event->pos().y() ) , 1.0 , 1.0 , viewport );
		//gluPickMatrix( GLdouble( width() / 2 ) , GLdouble( height() / 2 ) , 1.0 , 1.0 , viewport );

		GLfloat x = GLfloat( width() ) / height();
		gluPerspective( fov , (float)viewport[ 2 ] / (float)viewport[ 3 ] , 0.0001 , 10000.0 );
		//glFrustum( -1.0 , 1.0 , -1.0 , 1.0 , 0.0001f, 5000.0f );

		glMatrixMode( GL_MODELVIEW );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		glLoadIdentity();
		glRotatef( xRot , 1.0 , 0.0 , 0.0 );
		glRotatef( yRot , 0.0 , 1.0 , 0.0 );
		glTranslatef( xPos , yPos , zPos );

#if 1
		GLdouble depth , modelview[ 16 ] , projection[ 16 ];
		glGetDoublev( GL_MODELVIEW_MATRIX , modelview );
		glGetDoublev( GL_PROJECTION_MATRIX , projection );

		GLdouble worldX , worldY , worldZ;
		glReadPixels( event->pos().x() , viewport[ 3 ] - event->pos().y() , 1 , 1 , GL_DEPTH_COMPONENT , GL_DOUBLE , &depth );
		gluUnProject( GLdouble( event->pos().x() ) , GLdouble( viewport[ 3 ] - event->pos().y() ) , depth ,  // 1.0 = far clipping plane,  0.0 = near clipping plane
								  modelview , projection , viewport ,
								  &worldX , &worldY , &worldZ );

		//QMessageBox::information( this , "Hit" , QString( "Coords: %1 %2 %3" ).arg( worldX ).arg( worldY ).arg( worldZ ) );
		qWarning( QString( "Coords: %1 %2 %3" ).arg( worldX ).arg( worldY ).arg( worldZ ).toAscii().data() );
#endif

		glRenderMode( GL_SELECT );

		do
		{
			glInitNames(); // This stack contains all the info about the objects
			glPushName( 0 ); // Now fill the stack with one element (or glLoadName will generate an error)

			for( int index = 0 ; index < meshes->size() ; index++ )
				if( !hitLog.contains( index ) && meshes->at( index ) )
				{
					glLoadName( index );
					meshes->at( index )->draw();
				}

			hits = glRenderMode( GL_SELECT );

			if( hits && buff[ 0 ] )
			{
				hitLog.append( buff[ 3 ] );
				zLog.append( buff[1] );
				//QMessageBox::information( this , "Hits" , QString( "%1 %2" ).arg( hits ).arg( buff[2] ) );
				//glm::vec3 rgb = CGL::randomRGB();
				//meshes->at( buff[3] )->setColor( rgb.r , rgb.g , rgb.b );

				//delete meshes->at( buff[ 3 ] );
				//meshes[ buff[ 3 ] ] = NULL;

				continue;
			}
			else
				break;
		}
		while( true );

		glMatrixMode( GL_PROJECTION );
		glPopMatrix();
		//glFlush();

		glRenderMode( GL_RENDER );
		glMatrixMode( GL_MODELVIEW );

		//QMessageBox::information( this , "Hits" , QString( "Total Hits: %1" ).arg( hitLog.size() ) );

		//for( int index = 0 ; index < zLog.size() ; index++ )
			//QMessageBox::information( this , "Hits" , QString( "Depth: %1" ).arg( zLog.at( index ) ) );

		// Find nearest pick
		if( hitLog.size() )
		{
			GLint pick = hitLog.at( 0 );
			GLuint depth = zLog.at( 0 );

			for( int index = 1 ; index < hitLog.size() ; index++ )
				if( zLog.at( index ) < depth )
				{
					depth = zLog.at( index );
					pick = hitLog.at( index );
				}

			glm::vec3 rgb = CGL::randomRGB();
			meshes->at( pick )->setColor( rgb.r , rgb.g , rgb.b );

			//delete meshes->at( pick );
			//meshes[ pick ] = NULL;
		}
	}
#endif
}

void VGLView::keyPressEvent( QKeyEvent *event )
{
	if( event )
	{
		switch( event->key() )
		{
			case Qt::Key_W:
				keyBits |= KEY_W;
				break;
			case Qt::Key_A:
				keyBits |= KEY_A;
				break;
			case Qt::Key_S:
				keyBits |= KEY_S;
				break;
			case Qt::Key_D:
				keyBits |= KEY_D;
				break;
			//case Qt::Key_Direction_R:
			case 0x60: // Tilde
				QMessageBox::information( this , "" , QString( "Pos: %1 %2 %3" ).arg( QString::number( xPos ) ).arg( QString::number( yPos ) ).arg( QString::number( zPos ) ) + QString( "\nRot: %1 %2 %3" ).arg( QString::number( xRot ) ).arg( QString::number( yRot ) ).arg( QString::number( zRot ) ) );
				break;
			case Qt::Key_Space:
				xPos = yPos = zPos = xRot = yRot = zRot = 0.0;
				break;
			default:
			{
				event->ignore();
				return;
			}
		}
#if 0
		switch( event->key() )
		{
			case Qt::Key_W:
				xPos -= 0.1 * sin( yRot * deg2rad );
				zPos += 0.1 * cos( yRot * deg2rad );
				break;
			case Qt::Key_A:
				xPos += 0.1 * cos( yRot * deg2rad );
				zPos += 0.1 * sin( yRot * deg2rad );
				break;
			case Qt::Key_S:
				xPos += 0.1 * sin( yRot * deg2rad );
				zPos -= 0.1 * cos( yRot * deg2rad );
				break;
			case Qt::Key_D:
				xPos -= 0.1 * cos( yRot * deg2rad );
				zPos -= 0.1 * sin( yRot * deg2rad );
				break;
			default:
			{
				event->ignore();
				return;
			}
		}
#endif

		event->accept();
	}
}

void VGLView::keyReleaseEvent( QKeyEvent *event )
{
	if( event )
		{
			switch( event->key() )
			{
				case Qt::Key_W:
					keyBits &= ~KEY_W;
					break;
				case Qt::Key_A:
					keyBits &= ~KEY_A;
					break;
				case Qt::Key_S:
					keyBits &= ~KEY_S;
					break;
				case Qt::Key_D:
					keyBits &= ~KEY_D;
					break;
				default:
				{
					event->ignore();
					return;
				}
			}

			event->accept();
		}
}
