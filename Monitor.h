#ifndef MONITOR_H
#define MONITOR_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QTimer>
#include <Simulation.h>
#include <GuiConstants.h>
#include <QPalette>
#include <QKeyEvent>

class Monitor : public QWidget
{
    Q_OBJECT
public:
    explicit Monitor(Simulation* sim, QWidget *parent = 0, int refreshRate = 0);
    ~Monitor();
    void setSimulation(Simulation* sim);
    void onResize(int w, int h);

signals:

public slots:
    void updateSlot(void);

protected:
    void keyPressEvent(QKeyEvent *event)
    {
        if(event->key()==Qt::Key_A)
        {
            std::cout << "jcsuej" << std::endl;
            move(0,50);
        }
    }

private:

    Simulation* m_sim;
    QTimer*     m_timer;
    int m_refreshRate;

    QVBoxLayout* mainLayout;
    //QFrame* bottomFrame;
    QLabel* titleLabel;
    QLabel* timeLabel;

};

#endif // MONITOR_H
