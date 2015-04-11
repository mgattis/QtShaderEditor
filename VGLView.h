#ifndef VGLVIEW_H
#define VGLVIEW_H

#include <iostream>

#include "CCamera.h"
#include "CObj.h"
#include "CProject.h"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <QApplication>
#include <QByteArray>
#include <QDir>
#include <QFile>
#include <QGLShader>
#include <QGLWidget>
#include <QKeyEvent>
#include <QMessageBox>
#include <QMouseEvent>
#include <QObject>
#include <QOpenGLTexture>
#include <QTime>
#include <QTimer>

#include <QGLShader>

#ifndef M_PI
#define M_PI (3.14159265359)
#endif

#define DEG2RAD (M_PI/180.0)

class VGLView : public QGLWidget
{
public:
    VGLView( QWidget *parent = NULL );
    ~VGLView();

public:
    enum KeyBits_t
    {
        KEY_Q = 1 << 31,
        KEY_W = 1 << 30,
        KEY_E = 1 << 29,
        KEY_A = 1 << 28,
        KEY_S = 1 << 27,
        KEY_D = 1 << 26,
        KEY_SPACE = 1 << 25,
        KEY_LSHIFT = 1 << 24,
        KEY_C = 1 << 23
    };

public:
    void initializeGL( void );
    void resizeGL( int width , int height );
    void paintGL( void );

public:
    void updateCamera(float lastFrameTime);
    void drawScene();

    void openProject(QString projectFile);
    void closeProject();

    CProject *getProject();

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

    QPoint originalGlobalCursorPos , originalLocalCursorPos , lastCursorPos;
    float mouseSensitivity;
    /* , moveSpeed;
    float fov;
    float xPos , yPos , zPos;
    float xRot , yRot , zRot;*/
    CCamera camera;
    CProject *project;

    float maxAcceleration;
    float maxVelocity;
    float friction;
    float speedMultiplier;

    //CObj *obj;

    /*QGLShaderProgram program;
    const char *vertShader , *fragShader;
    GLuint shaderProgram , vertexShader , fragmentShader;

    //QOpenGLTexture *texture;
    GLuint defaultTextureID;
    QImage *defaultTexture;*/
};

#endif // VGLVIEW_H
