#include "VGLView.h"

VGLView::VGLView( QWidget *parent /* = NULL */ )
{
    keyBits = 0;
    mouseSensitivity = 8.0;

    maxAcceleration = 8.0;
    maxVelocity = 1.0;
    friction = 2.0;
    speedMultiplier = 1.0;

    project = NULL;

    this->setMouseTracking( true );
}

VGLView::~VGLView()
{
    repaintTimer.stop();
}

void VGLView::initializeGL( void )
{
    // NOTE: This method has been shown to be called twice somehow.
    // Make sure code here only occurs once. Remove this if the
    // problem is found and resolved
    static bool complete = false;

	if( !complete )
	{
        GLenum err = glewInit();
        if (err != GLEW_OK) {
            std::clog << "[WARNING]: Failed to initialize GLEW." << std::endl;
            std::clog << "[WARNING]: " << glewGetErrorString(err) << std::endl;
        }

        if (!GLEW_VERSION_2_1) {
            // check that the machine supports the 2.1 API.
            std::clog << "[WARNING]: GLEW: API 2.1 not supported." << std::endl;
        }

        connect( &repaintTimer , SIGNAL(timeout()) , this , SLOT(updateGL()) );
        repaintTimer.setInterval( 33 );
        repaintTimer.start();

        openProject("./QtSEProjects/testProject/testProject.project.json");

        complete = true;
    }
}

void VGLView::paintGL( void )
{
    makeCurrent();
    //program.bind();

    static int totalFrames = 0;
    totalFrames++;
    static QTime frameTimer;
    static float totalTime = 0.0;
    static float lastFrameTime;
    lastFrameTime = frameTimer.restart() / 1000.0;
    totalTime += lastFrameTime;

    if (project) {
        updateCamera(lastFrameTime);
        project->run(lastFrameTime);
    }

    /*glUseProgram(0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable( GL_DEPTH_TEST );
    glClearColor( 1.0 , 0.0 , 0.0, 0.0 );
    glDepthFunc( GL_LEQUAL );
    glEnable( GL_DEPTH_TEST );

    glViewport( 0 , 0 , (GLint)this->width() , (GLint)this->height() );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective( 80.0 , (double)this->width()/(double)this->height() , 0.1 , 100.0 );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    //glRotatef(totalTime*360.0, 1.0, 0.0, 0.0);

    glBegin( GL_TRIANGLES );
        glColor3f( 1.0 , 0.0 , 0.0 );
        glVertex3f( -0.5 , -0.5 , -2.0 );
        glColor3f( 1.0 , 1.0 , 0.0 );
        glVertex3f( 0.5 , -0.5 , -2.0 );
        glColor3f( 0.0 , 0.0 , 1.0 );
        glVertex3f( 0.0 , 0.5 , -2.0 );
    glEnd();*/
}

void VGLView::updateCamera(float lastFrameTime) {
    // Walk on the XY plane.

    // Apply friction.
    glm::vec2 velocity = glm::vec2(camera.velocity);
    if (glm::length(velocity) <= lastFrameTime * friction) {
        camera.velocity.x = 0.0;
        camera.velocity.y = 0.0;
    }
    else {
        velocity -= friction * lastFrameTime * glm::normalize(velocity);
        camera.velocity = glm::vec4(velocity, camera.velocity.z, 0.0);
    }

    // Apply acceleration.
    glm::vec2 acceleration = glm::vec2(0.0, 0.0);
    acceleration.x -= keyBits & KEY_A ? 1.0 : 0.0;
    acceleration.x += keyBits & KEY_D ? 1.0 : 0.0;
    acceleration.y -= keyBits & KEY_S ? 1.0 : 0.0;
    acceleration.y += keyBits & KEY_W ? 1.0 : 0.0;
    if (glm::length(acceleration) > 1.0) {
        acceleration = glm::normalize(acceleration);
    }
    // Apply acceleration rotation.
    float cameraRotation = camera.angle.z * (M_PI/180.0);
    acceleration = glm::vec2(glm::rotate(glm::mat4(), cameraRotation, glm::vec3(0.0, 0.0, -1.0)) * glm::vec4(acceleration, 0.0, 0.0));
    // Apply acceleration to velocity.
    camera.velocity += lastFrameTime * maxAcceleration * glm::vec4(acceleration, 0.0, 0.0);

    // limit velocity
    velocity = glm::vec2(camera.velocity);
    if (glm::length(velocity) > maxVelocity) {
        velocity = maxVelocity * glm::normalize(velocity);
        camera.velocity = glm::vec4(velocity, camera.velocity.z, 0.0);
    }

    // Handle Z
    if (glm::abs(camera.velocity.z) <= friction * lastFrameTime) {
        camera.velocity.z = 0.0;
    }
    else {
        camera.velocity.z -= glm::sign(camera.velocity.z) * friction * lastFrameTime;
    }

    float zacc = 0.0;
    zacc -= keyBits & KEY_SPACE ? 1.0 : 0.0;
    zacc += keyBits & KEY_LSHIFT ? 1.0 : 0.0;

    camera.velocity.z += maxAcceleration * zacc * lastFrameTime;

    // Cap Z max speed.
    if (glm::abs(camera.velocity.z) > maxVelocity) {
        camera.velocity.z = glm::sign(camera.velocity.z) * maxVelocity;
    }

    // apply velocity
    camera.position += lastFrameTime * camera.velocity;

    //glLoadIdentity();
    //glRotatef(camera.angle.x, 1.0, 0.0, 0.0);
    //glRotatef(camera.angle.z, 0.0, 0.0, 1.0);
    //glTranslatef(-camera.position.x, -camera.position.y, -camera.position.z);

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

void VGLView::openProject(QString projectFile) {
    if (project) {
        closeProject();
    }

    project = new CProject(projectFile);
    project->setViewPort(this->width(), this->height());
    project->setCamera(&camera);
}

void VGLView::closeProject() {
    delete project;
    project = NULL;
}


void VGLView::resizeGL( int width , int height )
{
    /*glEnable( GL_DEPTH_TEST );
    glClearColor( 0.0 , 0.0 , 0.0, 0.0 );
    glDepthFunc( GL_LEQUAL );
    glEnable( GL_BLEND );

    glViewport( 0 , 0 , (GLint)width , (GLint)height );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective( 80.0 , (double)width/(double)height , 0.1 , 100.0 );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();*/

    if (project) {
        project->setViewPort(width, height);
    }
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

            camera.angle.z += ( event->pos().x() - lastCursorPos.x() ) / mouseSensitivity;
            camera.angle.x -= ( event->pos().y() - lastCursorPos.y() ) / mouseSensitivity;

            if( camera.angle.x < -90.0 ) {
                camera.angle.x = -90.0;
            }
            else if( camera.angle.x > 90.0 ) {
                camera.angle.x = 90.0;
            }

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
        case Qt::Key_Space:
            keyBits |= KEY_SPACE;
            break;
        case Qt::Key_Shift:
            keyBits |= KEY_LSHIFT;
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
        case Qt::Key_Space:
            keyBits &= ~KEY_SPACE;
            break;
        case Qt::Key_Shift:
            keyBits &= ~KEY_LSHIFT;
            break;
        default:
            event->ignore();
            return;
        }

        event->accept();
    }
}
