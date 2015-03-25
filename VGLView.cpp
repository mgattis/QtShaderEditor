#include "VGLView.h"

VGLView::VGLView( QWidget *parent /* = NULL */ )
{
	keyBits = 0;
	mouseSensitivity = 10.0;
	/*moveSpeed = 20.0;
	fov = 45.0;
	xPos = yPos = zPos = 0.0;
	xRot = yRot = zRot = 0.0;*/

    maxAcceleration = 8.0 * 8.0;
    maxVelocity = 1.0 * 8.0;
	friction = 2.0 * 8.0;

	this->setMouseTracking( true );

	QFile file;
	file.setFileName( QDir::home().absolutePath() + "/Projects/QtShaderEditor/resources/vertexShader.glsl" );
	if( file.open( QFile::ReadOnly ) )
	{
		QByteArray data = file.readAll();
		vertShader = new char[ data.size() ];
		memcpy( vertShader , data , data.size() );

		file.close();
	}

	file.setFileName( QDir::home().absolutePath() + "/Projects/QtShaderEditor/resources/fragmentShader.glsl" );
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

	//QDir::setCurrent( QDir::home().absolutePath() + "/Projects/QtShaderEditor/resources/" );
	//QString currentDir = QDir::current().absolutePath();

#if 0
	const char *filename = QDir::home().absolutePath() + "/Projects/QtShaderEditor/resources/grassblock.obj";
	const char *basepath = QDir::home().absolutePath() + "/Projects/QtShaderEditor/resources/";
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
#endif

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

void VGLView::initializeGL( void )
{
	static bool complete = false;

	if( !complete )
	{
		glewInit();
#if 1
		glGenVertexArrays( 1 , &vaoID );
		glBindVertexArray( vaoID );

		glGenBuffers( 2 , &vboID[ 0 ] );

		QString basePath = QDir::home().absolutePath();
		QString filePath;
		//std::cout << "homedir: " << homeDir.toLatin1().data() << std::endl;

		//basePath += "/Projects/QtShaderEditor/resources";
		//filePath = basePath + "/grassblock.obj";
		//basePath += "/Projects/QtShaderEditor/resources";
		//filePath = basePath + "/minecraft.obj";
		basePath += "/Projects/QtShaderEditor/resources/dabrovic-sponza";
		filePath = basePath + "/sponza.obj";
		//basePath += "/Projects/QtShaderEditor/resources/head";
		//filePath = basePath + "/head.OBJ";
		//basePath += "/Projects/QtShaderEditor/resources/lost-empire";
		//filePath = basePath + "/lost_empire.obj";

		obj = CObj::loadFromPath( filePath , basePath );
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

#if 1
		image = new QImage( QDir::home().absolutePath() + "/Projects/QtShaderEditor/resources/grassside.png" );
		*image = image->mirrored( 0 , 1 );
		*image = image->convertToFormat( QImage::Format_RGB888 );

		glGenTextures( 1 , &textureID );
		glBindTexture( GL_TEXTURE_2D , textureID );
		glTexImage2D( GL_TEXTURE_2D , 0 , GL_RGB , image->width() , image->height() , 0 , GL_RGB , GL_UNSIGNED_BYTE , ( (const QImage*)image )->bits() );
		glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_MAG_FILTER , GL_NEAREST );
		glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_MIN_FILTER , GL_NEAREST );
#endif

		complete = true;

		connect( &repaintTimer , SIGNAL(timeout()) , this , SLOT(updateGL()) );
		repaintTimer.setInterval( 33 );
		repaintTimer.start();
#else
		program.addShaderFromSourceCode(QGLShader::Vertex,
			 "uniform mat4 ProjectionMatrix;\n"
			 "uniform mat4 ModelMatrix;\n"
			 "uniform mat4 ViewMatrix;\n"
			 "void main(void)\n"
			 "{\n"
			 "   gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * gl_Vertex;\n"
			 "}");
		 program.addShaderFromSourceCode(QGLShader::Fragment,
			 "uniform float time;\n"
			 "void main(void)\n"
			 "{\n"
			 "   float red = sin(time);\n"
			 "   float green = sin(time + 3.14159 * 2.0 * 0.3333333);\n"
			 "   float blue = sin(time + 3.14159 * 2.0 * 0.6666667);\n"
			 "   gl_FragColor = vec4(red, green, blue, 1.0);\n"
			 "}");

		connect( &repaintTimer , SIGNAL(timeout()) , this , SLOT(updateGL()) );
		repaintTimer.setInterval( 16 );
		repaintTimer.start();
#endif
	}
}

void VGLView::paintGL( void )
{
    makeCurrent();
    program.bind();

	static int totalFrames = 0;
	totalFrames++;
	static QTime frameTimer;
	static float totalTime = 0.0;
	static float lastFrameTime;
	lastFrameTime = frameTimer.restart() / 1000.0;
	totalTime += lastFrameTime;

#if 0
	program.bind();
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	updateCamera(lastFrameTime);

    //program.setUniformValue("time", (GLfloat)totalTime);
	GLuint result = glGetUniformLocation(program.programId(), "time");
	glUniform1f(result, totalTime);

	result = glGetUniformLocation(program.programId(), "ProjectionMatrix");
    glUniformMatrix4fv(result, 1, GL_FALSE, &(camera.projectionMatrix[0][0]));

	result = glGetUniformLocation(program.programId(), "ViewMatrix");
	glUniformMatrix4fv(result, 1, GL_FALSE, &(camera.viewMatrix[0][0]));

    result = glGetUniformLocation(program.programId(), "ModelMatrix");
    glUniformMatrix4fv(result, 1, GL_FALSE, &(camera.modelMatrix[0][0]));

    drawScene();
#else
	glLoadIdentity();
	//glRotatef( xRot , 1.0 , 0.0 , 0.0 );
	//glRotatef( yRot , 0.0 , 1.0 , 0.0 );
	//glTranslatef( xPos , yPos , zPos );

	//program.setUniformValue("time", (GLfloat)totalTime);

#if 0
	glm::mat4 orientation;
	orientation = glm::rotate( orientation , -xRot , glm::vec3( 1 , 0 , 0 ) );
	orientation = glm::rotate( orientation , -yRot , glm::vec3( 0 , 1 , 0 ) );

	glm::mat4 camera = glm::perspective( 80.0 , (double)this->width()/(double)this->height() , 0.1 , 100.0 );
	camera *= orientation;
	camera = glm::translate( camera , glm::vec3( -xPos , yPos , zPos ) );

	//camera = glm::perspective( 80.0 , -(double)this->width()/(double)this->height() , 0.1 , 100.0 );
	//camera = glm::translate( camera , glm::vec3( 0.0 , 0.0 , 3.0 ) );
	camera = glm::inverse( camera );
#endif

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

#if 0
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
#endif

	// New

	updateCamera(lastFrameTime);

	GLuint result = glGetUniformLocation( shaderProgram , "time");
	glUniform1f(result, totalTime);

	result = glGetUniformLocation( shaderProgram , "projection");
	glUniformMatrix4fv(result, 1, GL_FALSE, &(camera.projectionMatrix[0][0]));

	result = glGetUniformLocation( shaderProgram , "view");
	glUniformMatrix4fv(result, 1, GL_FALSE, &(camera.viewMatrix[0][0]));

	result = glGetUniformLocation( shaderProgram , "model");
	glUniformMatrix4fv(result, 1, GL_FALSE, &(camera.modelMatrix[0][0]));

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
		//std::cout << "----------------------------------------" << std::endl;
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
		//std::cout << "----------------------------------------" << std::endl;
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

#endif
    return;
}

void VGLView::updateCamera(float lastFrameTime) {
    // Walk on the XY plane.

    // Apply friction.
	glm::vec3 velocity = glm::vec3(camera.velocity);
    if (glm::length(velocity) <= lastFrameTime * friction) {
		camera.velocity = glm::vec4(0.0, 0.0, 0.0, 0.0);
    }
    else {
        velocity -= friction * lastFrameTime * glm::normalize(velocity);
		camera.velocity = glm::vec4(velocity, 0.0);
    }

	// Apply acceleration.
	glm::vec4 acceleration = glm::vec4(0.0, 0.0, 0.0, 0.0);
	acceleration.x += keyBits & KEY_D ? 1.0 : 0.0;
	acceleration.x -= keyBits & KEY_A ? 1.0 : 0.0;
	acceleration.y += keyBits & KEY_E ? 1.0 : 0.0;
	acceleration.y -= keyBits & KEY_Q ? 1.0 : 0.0;
	acceleration.z += keyBits & KEY_S ? 1.0 : 0.0;
	acceleration.z -= keyBits & KEY_W ? 1.0 : 0.0;
	if (glm::length(acceleration) > 1.0) {
		acceleration = glm::normalize(acceleration);
	}
	// Apply acceleration rotation.
	acceleration = glm::rotate(glm::mat4(), camera.angle.y, glm::vec3(0.0, -1.0, 0.0)) * acceleration;
	// Apply acceleration to velocity.
	camera.velocity += lastFrameTime * maxAcceleration * acceleration;

	// limit velocity
	velocity = glm::vec3(camera.velocity);
	if (glm::length(velocity) > maxVelocity) {
		velocity = maxVelocity * glm::normalize(velocity);
		camera.velocity = glm::vec4(velocity, 0.0);
	}

#if 0
    // Apply acceleration.
    glm::vec4 acceleration = glm::vec4(0.0, 0.0, 0.0, 0.0);
    acceleration.x += keyBits & KEY_A ? 1.0 : 0.0;
    acceleration.x -= keyBits & KEY_D ? 1.0 : 0.0;
    acceleration.y += keyBits & KEY_S ? 1.0 : 0.0;
    acceleration.y -= keyBits & KEY_W ? 1.0 : 0.0;
    if (glm::length(acceleration) > 1.0) {
        acceleration = glm::normalize(acceleration);
    }
    // Apply acceleration rotation.
    acceleration = glm::rotate(glm::mat4(), camera.angle.z, glm::vec3(0.0, 0.0, -1.0)) * acceleration;
    // Apply acceleration to velocity.
    camera.velocity += lastFrameTime * maxAcceleration * acceleration;

    // limit velocity
    velocity = glm::vec2(camera.velocity);
    if (glm::length(velocity) > maxVelocity) {
        velocity = maxVelocity * glm::normalize(velocity);
        camera.velocity = glm::vec4(velocity, camera.velocity.z, 0.0);
    }
#endif

    // apply velocity
    camera.position += lastFrameTime * camera.velocity;

    //glRotatef(camera.angle.x, 1.0, 0.0, 0.0);
    //glRotatef(camera.angle.z, 0.0, 0.0, 1.0);
    //glTranslatef(camera.position.x, camera.position.y, camera.position.z);

    camera.viewMatrix = camera.getMatrixFromPosition();
}

void VGLView::drawScene() {
    glBegin( GL_TRIANGLES );
        glColor3f( 1.0 , 0.0 , 0.0 );
        glVertex3f( -0.5 , -0.5 , -2.0 );
        glColor3f( 1.0 , 1.0 , 0.0 );
        glVertex3f( 0.5 , -0.5 , -2.0 );
        glColor3f( 0.0 , 0.0 , 1.0 );
        glVertex3f( 0.0 , 0.5 , -2.0 );
    glEnd();
}

void VGLView::resizeGL( int width , int height )
{
    glEnable( GL_DEPTH_TEST );
    glClearColor( 0.0 , 0.0 , 0.0, 0.0 );
    glDepthFunc( GL_LEQUAL );

    glViewport( 0 , 0 , (GLint)width , (GLint)height );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective( 80.0 , (double)width/(double)height , 0.1 , 100.0 );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    camera.projectionMatrix = glm::perspective(80.0, (double)width / (double)height, 0.1, 100.0);
}

void VGLView::enterEvent( QEvent *event )
{
	grabKeyboard();
    grabMouse();
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
	}
}

void VGLView::mouseMoveEvent( QMouseEvent *event )
{
	if( event )
    {
		if( event->buttons() & Qt::RightButton )
		{
            // FPS Style

			camera.angle.y += ( event->pos().x() - lastCursorPos.x() ) / mouseSensitivity;
			camera.angle.x -= ( event->pos().y() - lastCursorPos.y() ) / mouseSensitivity;

			// Clamp

#if 1
			if( camera.angle.x > 89.0 ) {
				camera.angle.x = 89.0;
            }
			else if( camera.angle.x < -89.0 ) {
				camera.angle.x = -89.0;
			}
#endif

			lastCursorPos = event->pos();
			event->accept();
        }

        //lastCursorPos = event->pos();
	}
}

void VGLView::mouseReleaseEvent( QMouseEvent *event )
{

}

void VGLView::keyPressEvent( QKeyEvent *event )
{
	if( event )
	{
		switch( event->key() )
		{
		case Qt::Key_Q:
			keyBits |= KEY_Q;
			break;
        case Qt::Key_W:
            keyBits |= KEY_W;
            break;
		case Qt::Key_E:
			keyBits |= KEY_E;
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
        default:
            event->ignore();
            return;
		}

		event->accept();
	}
}

void VGLView::keyReleaseEvent( QKeyEvent *event )
{
	if( event )
		{
			switch( event->key() )
			{
			case Qt::Key_Q:
				keyBits &= ~KEY_Q;
				break;
            case Qt::Key_W:
                keyBits &= ~KEY_W;
                break;
			case Qt::Key_E:
				keyBits &= ~KEY_E;
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
                event->ignore();
                return;
			}

			event->accept();
		}
}
