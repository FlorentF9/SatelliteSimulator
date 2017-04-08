#ifndef SIMULATIONDISPLAY_H
#define SIMULATIONDISPLAY_H

#include <SimulationGL.h>
#include <Simulation.h>
#include <GuiConstants.h>
#include <QImage>
#include <TrackBallCamera.h>

class SimulationDisplay : public SimulationGL
{
    Q_OBJECT

public:
    SimulationDisplay(int framesPerSecond = GuiConstants::fps, QWidget *parent = 0, char const* name = 0, Simulation *sim = NULL);
    ~SimulationDisplay();
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();
    void loadTexture(QString path, unsigned int i);

    void drawEllipse(const Orbit* orbit, double scale, int i, int n);

    Simulation*  sim(void) const {return m_sim;}
    QTimer*      timer(void) const {return sim_Timer;}
    void setSimulation(Simulation *sim);

public slots:
    void simUpdateSlot(void);

protected:
    void keyPressEvent(QKeyEvent *event)
    {
        if(sim() != NULL)
        {
         switch(event->key())
         {
            case Qt::Key_Space:
                sim()->togglePlay();
                break;
            case Qt::Key_F:
                if(sim()->speed()*1.5 > sim()->dt()/Constants::minTimeStep)
                {
                    sim()->setSpeed(1000*sim()->dt());
                    QMessageBox::warning(this, "Maximum speed reached", "Warning: the simulation is running at its highest speed (1000 updates per second).<br>To make it run faster, you may increase the time step (<i>Simulation>Configure...</i>)");
                }
                else
                    sim()->setSpeed(sim()->speed()*1.5);
                sim_Timer->setInterval(1000*sim()->dt()/sim()->speed());
                break;
            case Qt::Key_S:
                if(sim()->speed()/1.5 < sim()->dt()/Constants::maxTimeStep)
                {
                     sim()->setSpeed(sim()->dt()/Constants::maxTimeStep);
                    QMessageBox::warning(this, "Minimum speed reached", "Warning: the simulation is running at its lowest speed (one update every 60 seconds...do you want to fall asleep?).<br>To make it run even slower, you may decrease the time step (<i>Simulation>Configure...</i>)");
                }
                else
                    sim()->setSpeed(sim()->speed()/1.5);
                sim_Timer->setInterval(1000*sim()->dt()/sim()->speed());
                break;
            case Qt::Key_V:
                sim()->toggleVerbose();
                break;
         }
        }

         // Pass event to the parent class
         SimulationGL::keyPressEvent(event);
    }

    void mouseMoveEvent(QMouseEvent* event)
    {
        if(sim() != NULL)
        {
            m_camera->onMouseMotion(event);
        }

        // Pass event to the parent class
        SimulationGL::mouseMoveEvent(event);
    }

    void mousePressEvent(QMouseEvent* event)
    {
        if(sim() != NULL)
        {
            switch(event->button())
            {
                case Qt::LeftButton:
                    //std::cout << "Left click" << std::endl;
                    setCursor(QCursor(Qt::ClosedHandCursor));
                    m_camera->onMousePress(event);
                    break;
                default:
                    break;
            }
        }

        // Pass event to the parent class
        SimulationGL::mousePressEvent(event);
    }

    void mouseReleaseEvent(QMouseEvent* event)
    {
        if(sim() != NULL)
        {
            switch(event->button())
            {
                case Qt::LeftButton:
                    //std::cout << "Left released" << std::endl;
                    setCursor(QCursor(Qt::OpenHandCursor));
                    m_camera->onMouseRelease(event);
                    break;
                default:
                    break;
            }
        }

        // Pass event to the parent class
        SimulationGL::mouseReleaseEvent(event);
    }

    void wheelEvent(QWheelEvent* event)
    {
        if(sim() != NULL)
        {
            //std::cout << "Wheel !" << std::endl;
            m_camera->onWheel(event);
        }

        // Pass event to the parent class
        SimulationGL::wheelEvent(event);
    }

private:
    Simulation*      m_sim;
    QTimer*          sim_Timer;
    GLuint           texture[3];
    QString          planetTexturePath;
    TrackBallCamera* m_camera;
};

#endif // SIMULATIONDISPLAY_H
