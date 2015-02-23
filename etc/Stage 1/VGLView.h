#ifndef VGLVIEW_H
#define VGLVIEW_H

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <QGLShader>
#include <QGLWidget>
#include <QKeyEvent>
#include <QMessageBox>
#include <QMouseEvent>
#include <QObject>
#include <QTime>
#include <QTimer>

#ifndef M_PI
#define M_PI (3.14159265359)
#endif

#define DEG2RAD (M_PI/180)

class VGLView : public QGLWidget
{
public:
	VGLView( QWidget *parent = NULL );
	~VGLView();

public:
	enum KeyBits_t
	{
		KEY_W = 1 << 31,
		KEY_A = 1 << 30,
		KEY_S = 1 << 29,
		KEY_D = 1 << 28
	};

public:
	void initializeGL( void );
	void resizeGL( int width , int height );
	void paintGL( void );

public:
	//QGLShader* getVertexShader( void ) { return vertexShader; }
	//QGLShader* getFragmentShader( void ) { return fragmentShader; }
	void setVertexShader( QGLShader *shader );
	void setFragmentShader( QGLShader *shader );

	void moveCamera( const glm::vec3 &vector ) { xPos = vector.x; yPos = vector.y; zPos = vector.z; }

protected slots:
	void enterEvent( QEvent *event );
	void leaveEvent( QEvent *event );
	void mousePressEvent( QMouseEvent *event );
	void mouseMoveEvent( QMouseEvent *event );
	void mouseReleaseEvent( QMouseEvent *event );
	void keyPressEvent( QKeyEvent *event );
	void keyReleaseEvent( QKeyEvent *event );

signals:
	void mouseLeftClick( int x , int y , Qt::KeyboardModifiers keyboardModifiers );
	void mouseLeftClick( int x , int y , QFlags< Qt::KeyboardModifiers > keyboardModifiers );

protected:
	QTimer repaintTimer;
	unsigned int keyBits;

	QPoint lastCursorPos;
	float mouseSensitivity , moveSpeed;
	float fov;
	float xPos , yPos , zPos;
	float xRot , yRot , zRot;
};

#endif // VGLVIEW_H
