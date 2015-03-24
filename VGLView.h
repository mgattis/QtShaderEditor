#ifndef VGLVIEW_H
#define VGLVIEW_H

#include "CCamera.h"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <QGLShader>
#include <QGLWidget>
#include <QKeyEvent>
#include <QMessageBox>
#include <QMouseEvent>
#include <QObject>
#include <QTime>
#include <QTimer>

#include <QGLShader>

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
        KEY_D = 1 << 28,
        KEY_SPACE = 1 << 27,
        KEY_LSHIFT = 1 << 26,
        KEY_C = 1 << 25
	};

public:
	void initializeGL( void );
	void resizeGL( int width , int height );
	void paintGL( void );

public:
    void updateCamera(float lastFrameTime);
    void drawScene();

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
    float mouseSensitivity;
    CCamera camera;

    float maxAcceleration;
    float maxVelocity;
    float friction;

    QGLShaderProgram program;
};

#endif // VGLVIEW_H
