#ifndef MYGLWIDGET_H
#define SIMULATIONGL_H

#include <QtOpenGL>
#include <QGLWidget>
#include <qgl.h>
#include <GL/glu.h>

class SimulationGL : public QGLWidget
{
    Q_OBJECT
public:
    explicit SimulationGL(int framesPerSecond = 0, QWidget *parent = 0, char const* name = 0);
    virtual void initializeGL() = 0;
    virtual void resizeGL(int width, int height) = 0;
    virtual void paintGL() = 0;
    virtual void keyPressEvent( QKeyEvent *keyEvent );

public slots:
    virtual void timeOutSlot();

private:
    QTimer *t_Timer;

};


#endif // SIMULATIONGL_H
