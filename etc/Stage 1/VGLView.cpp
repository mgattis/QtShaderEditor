#include "VGLView.h"

VGLView::VGLView( QWidget *parent /* = NULL */ )
{
	keyBits = 0;
	mouseSensitivity = 2.0;
	moveSpeed = 20.0;
	fov = 45.0;
	xPos = yPos = zPos = 0.0;
	xRot = yRot = zRot = 0.0;

	this->setMouseTracking( true );
}

VGLView::~VGLView()
{
	//
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
	connect( &repaintTimer , SIGNAL(timeout()) , this , SLOT(updateGL()) );
	repaintTimer.setInterval( 33 );
	repaintTimer.start();
}

void VGLView::paintGL( void )
{
	makeCurrent();
	//program->bind();

	// Stuff

	static QTime frameTimer;
	static float totalTime;
	static float lastFrameTime;
	lastFrameTime = frameTimer.restart() / 1000.0;
	totalTime += lastFrameTime;

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glLoadIdentity();

	// Position Camera

	glRotatef( xRot , 1.0 , 0.0 , 0.0 );
	glRotatef( yRot , 0.0 , 1.0 , 0.0 );

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

	glTranslatef( xPos , yPos , zPos );

	// Umm

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
