#include "VGLView.h"

VGLView::VGLView( QWidget *parent /* = NULL */ )
{
	keyBits = 0;
	mouseSensitivity = 30.0;

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

	// TODO: Turn into default/fallback shaders
	//vertShader = "#version 430\n\nin vec3 in_Position;\n\nvoid main()\n{\ngl_Position = vec4(in_Position.x, in_Position.y, in_Position.z, 1.0);\n}\n";
	//vertShader = "#version 430\n\nuniform mat4 camera;\nuniform mat4 model;\nin vec3 in_Position;\n\nvoid main()\n{\ngl_Position = camera * vec4( in_Position, 1.0);\n}\n";
	//fragShader = "#version 430\n\nprecision highp float;\n\nout vec4 fragColor;\n\nvoid main()\n{\nfragColor = vec4(1.0,1.0,1.0,1.0);\n}\n";
}

VGLView::~VGLView()
{
	repaintTimer.stop();
	makeCurrent();

	glDeleteProgram( shaderProgram );
	glDeleteShader( fragmentShader );
	glDeleteShader( vertexShader );

	delete vertShader;
	delete fragShader;

	delete defaultTexture;

	delete obj;
}

void VGLView::initializeGL( void )
{
	static bool complete = false;

	if( !complete )
	{
		glewInit();
#if 1
		QString basePath = QDir::home().absolutePath();
		QString filePath;
		//std::cout << "homedir: " << homeDir.toLatin1().data() << std::endl;

		// TODO: Remove
		basePath += "/Projects/QtShaderEditor/resources";
		filePath = basePath + "/grassblock.obj";
		//basePath += "/Projects/QtShaderEditor/resources";
		//filePath = basePath + "/minecraft.obj";
		//basePath += "/Projects/QtShaderEditor/resources/dabrovic-sponza";
		//filePath = basePath + "/sponza.obj";
		//basePath += "/Projects/QtShaderEditor/resources/head";
		//filePath = basePath + "/head.OBJ";
		//basePath += "/Projects/QtShaderEditor/resources/lost-empire";
		//filePath = basePath + "/lost_empire.obj";
		//basePath += "/Projects/QtShaderEditor/resources/rungholt";
		//filePath = basePath + "/rungholt.obj";
		//basePath += "/Projects/QtShaderEditor/resources/san-miguel";
		//filePath = basePath + "/san-miguel.obj";

		//std::cout << "Address: " << (long*)&system << std::endl;

		obj = CObj::loadFromPath( filePath , basePath );
		std::cout << "obj: " << obj << std::endl;

		for( int index = 0 ; index < obj->meshArray.size() ; index++ )
		{
			GLuint buf[ 2 ];
			glGenBuffers( 2 , &buf[ 0 ] );

			glBindBuffer( GL_ARRAY_BUFFER , buf[ 0 ] );
			glBufferData( GL_ARRAY_BUFFER , obj->meshArray.at( index )->vertexCount * sizeof( *obj->meshArray.at( index )->vertices ) , obj->meshArray.at( index )->vertices , GL_STATIC_DRAW );
			delete[] obj->meshArray.at( index )->vertices;
			obj->meshArray.at( index )->vertices = NULL;
			obj->meshArray.at( index )->vertexBuf = buf[ 0 ];

			delete[] obj->meshArray.at( index )->normals;
			obj->meshArray.at( index )->normals = NULL;

			glBindBuffer( GL_ARRAY_BUFFER , buf[ 1 ] );
			glBufferData( GL_ARRAY_BUFFER , obj->meshArray.at( index )->vertexCount * sizeof( *obj->meshArray.at( index )->texCoords ) , obj->meshArray.at( index )->texCoords , GL_STATIC_DRAW );
			delete[] obj->meshArray.at( index )->texCoords;
			obj->meshArray.at( index )->texCoords = NULL;
			obj->meshArray.at( index )->texCoordBuf = buf[ 1 ];
		}

		// Make shaders

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
		// Default texture because why not
		// TODO: Change to Source default
		defaultTexture = new QImage( QDir::home().absolutePath() + "/Projects/QtShaderEditor/resources/grassside.png" );
		*defaultTexture = defaultTexture->mirrored( 0 , 1 );
		*defaultTexture = defaultTexture->convertToFormat( QImage::Format_RGB888 );

		glGenTextures( 1 , &defaultTextureID );
		glBindTexture( GL_TEXTURE_2D , defaultTextureID );
		glTexImage2D( GL_TEXTURE_2D , 0 , GL_RGB , defaultTexture->width() , defaultTexture->height() , 0 , GL_RGB , GL_UNSIGNED_BYTE , ( (const QImage*)defaultTexture )->bits() );
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
			std::cout << "GL Use Error: " << gluErrorString( errorEnum ) << std::endl;

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

	GLint vertexLoc = glGetAttribLocation( shaderProgram , "vertex" );
	GLint uvLoc = glGetAttribLocation( shaderProgram , "UV" );

	for( int index = 0 ; index < obj->meshArray.size() ; index++ )
	{
		obj->meshArray.at( index )->vertexLoc = vertexLoc;
		obj->meshArray.at( index )->texCoordLoc = uvLoc;

		obj->meshArray.at( index )->draw();
	}

	// End of draw error reporting. Run once per second
	if( !( totalFrames % 30 ) )
	{
		if( errorEnum = glGetError() )
		{
			do
				std::cout << "GL Draw Error: " << gluErrorString( errorEnum ) << std::endl;
			while( errorEnum = glGetError() );

			std::cout << "----------------------------------------" << std::endl;
		}
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
		QApplication::setOverrideCursor( Qt::BlankCursor );
		originalLocalCursorPos = lastCursorPos = event->pos();

		originalGlobalCursorPos = this->mapToGlobal( lastCursorPos );
	}
}

void VGLView::mouseMoveEvent( QMouseEvent *event )
{
	if( event )
    {
		if( event->buttons() & Qt::RightButton )
		{
			event->accept();

			if( event->pos() == originalLocalCursorPos )
				return;

            // FPS Style

			camera.angle.y += ( event->pos().x() - lastCursorPos.x() ) / mouseSensitivity;
			camera.angle.x -= ( event->pos().y() - lastCursorPos.y() ) / mouseSensitivity;

			// Clamp in radians
#if 0
			if( camera.angle.x > 4.7 )
				camera.angle.x = 4.7;
			else if( camera.angle.x < 1.6 )
				camera.angle.x = 1.6;
#endif

			QCursor::setPos( originalGlobalCursorPos );
			lastCursorPos = originalLocalCursorPos;
		}
	}
}

void VGLView::mouseReleaseEvent( QMouseEvent *event )
{
	QApplication::restoreOverrideCursor();
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
