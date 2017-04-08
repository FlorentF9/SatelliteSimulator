#include "TrackBallCamera.h"

TrackBallCamera::TrackBallCamera()
{
    m_hold     = false;
    m_dist     = 40.0;
    m_xAngle   = 0.0;
    m_yAngle   = 0.0;
    m_scrollSens = 0.05;
    m_motionSens = 0.15;
}

TrackBallCamera::~TrackBallCamera()
{

}

void TrackBallCamera::look()
{
    gluLookAt(0.0, 0.0, m_dist,
              0.0,    0.0, 0.0,
              0.0,    1.0, 0.0);
    glRotated(m_xAngle, 1.0, 0.0, 0.0);
    glRotated(m_yAngle, 0.0, 1.0, 0.0);
}

void TrackBallCamera::onMouseMotion(QMouseEvent *event)
{
    if(m_hold)
    {
        double dx = event->x() - m_lastPos.x();
        double dy = event->y() - m_lastPos.y();
        m_xAngle += dy*m_motionSens;
        m_yAngle += dx*m_motionSens;
        m_lastPos = event->pos();
    }

    return;
}

void TrackBallCamera::onMousePress(QMouseEvent* event)
{
    if(!m_hold)
    {
        m_hold = true;
        m_lastPos = event->pos();
    }

    return;
}

void TrackBallCamera::onMouseRelease(QMouseEvent* event)
{
    if(m_hold)
    {
        m_hold = false;
    }

    return;
}

void TrackBallCamera::onWheel(QWheelEvent *event)
{
    m_dist -= event->angleDelta().y()*m_scrollSens;
    if(m_dist < 0.1)
        m_dist = 0.1;

    return;
}

void TrackBallCamera::onKeyPressed(QEvent *event)
{

    return;
}
