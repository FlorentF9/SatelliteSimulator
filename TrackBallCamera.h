#ifndef TRACKBALLCAMERA_H
#define TRACKBALLCAMERA_H

#include <QCursor>
#include <QMouseEvent>

#include <QtOpenGL>
#include <qgl.h>
#include <GL/glu.h>

class TrackBallCamera
{
public:
    TrackBallCamera();
    ~TrackBallCamera();

    void onMouseMotion(QMouseEvent* event);
    void onMousePress(QMouseEvent* event);
    void onMouseRelease(QMouseEvent* event);
    void onWheel(QWheelEvent* event);
    void onKeyPressed(QEvent* event);
    void look(void);
    void setMotionSens(double s) {m_motionSens = s;}
    void setScrollSens(double s) {m_scrollSens = s;}

private:
    double m_motionSens;
    double m_scrollSens;
    bool   m_hold;
    double m_dist;
    double m_xAngle;
    double m_yAngle;
    QPoint m_lastPos;
};

#endif // TRACKBALLCAMERA_H
