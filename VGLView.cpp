#include "VGLView.h"

VGLView::VGLView( QWidget *parent /* = NULL */ )
{
    keyBits = 0;
    mouseSensitivity = 4.0;

    maxAcceleration = 8.0 * 8.0;
    maxVelocity = 1.0 * 8.0;
    friction = 2.0 * 8.0;

	this->setMouseTracking( true );
}

VGLView::~VGLView()
{
	//
}

void VGLView::initializeGL( void )
{
    glewInit();

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
}

void VGLView::paintGL( void )
{
    makeCurrent();
    program.bind();

	static QTime frameTimer;
	static float totalTime;
	static float lastFrameTime;
	lastFrameTime = frameTimer.restart() / 1000.0;
	totalTime += lastFrameTime;

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    //program.setUniformValue("time", (GLfloat)totalTime);
    GLuint iResult = glGetUniformLocation(program.programId(), "time");
    glUniform1f(iResult, totalTime);

    updateCamera(lastFrameTime);

    GLuint result = glGetUniformLocation(program.programId(), "ProjectionMatrix");
    glUniformMatrix4fv(result, 1, GL_FALSE, &(camera.projectionMatrix[0][0]));

    result = glGetUniformLocation(program.programId(), "ModelMatrix");
    glUniformMatrix4fv(result, 1, GL_FALSE, &(camera.modelMatrix[0][0]));

    result = glGetUniformLocation(program.programId(), "ViewMatrix");
    glUniformMatrix4fv(result, 1, GL_FALSE, &(camera.viewMatrix[0][0]));

    drawScene();

    return;
}

void VGLView::updateCamera(float lastFrameTime) {
    // Walk on the XY plane.

    // Apply friction.
    glm::vec2 velocity = glm::vec2(camera.velocity);
    if (glm::length(velocity) <= lastFrameTime * friction) {
        camera.velocity = glm::vec4(0.0, 0.0, camera.velocity.z, 0.0);
    }
    else {
        velocity -= friction * lastFrameTime * glm::normalize(velocity);
        camera.velocity = glm::vec4(velocity, camera.velocity.z, 0.0);
    }

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

            camera.angle.x += ( event->pos().y() - lastCursorPos.y() ) / mouseSensitivity;
            camera.angle.z += ( event->pos().x() - lastCursorPos.x() ) / mouseSensitivity;

			// Clamp

            /*if( camera.angle.x > 90.0 ) {
                camera.angle.x = 90.0;
            }
            else if( camera.angle.x < -90.0 ) {
                camera.angle.x = -90.0;
            }*/

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
                event->ignore();
                return;
			}

			event->accept();
		}
}
