#include <SimulationGL.h>

SimulationGL::SimulationGL(int framesPerSecond, QWidget *parent, char const* name) : QGLWidget(parent)
{
    setWindowTitle(QString::fromUtf8(name));
    if(framesPerSecond == 0)
            t_Timer = NULL;
    else
    {
        int timerInterval = 1000 / framesPerSecond;
        t_Timer = new QTimer(this);
        connect(t_Timer, SIGNAL(timeout()), this, SLOT(timeOutSlot()));
        t_Timer->start(timerInterval);
    }
}

void SimulationGL::keyPressEvent(QKeyEvent *keyEvent)
{

}

void SimulationGL::timeOutSlot()
{
    updateGL();
}
