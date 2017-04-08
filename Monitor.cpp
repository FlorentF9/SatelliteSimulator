#include "Monitor.h"

Monitor::Monitor(Simulation *sim, QWidget *parent, int refreshRate) : QWidget(parent)
{

    m_sim = sim;
    m_refreshRate = refreshRate;

    QPalette palette;
    palette.setColor(QPalette::Background, QColor(100,100,100));
    palette.setColor(QPalette::Foreground, Qt::white);
    setAutoFillBackground(true);
    setPalette(palette);

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    mainLayout = new QVBoxLayout;
    //mainLayout->setAlignment(Qt::AlignHCenter);
    mainLayout->setAlignment(Qt::AlignCenter);
    //bottomFrame = new QFrame(this);

    titleLabel = new QLabel(this);
    timeLabel  = new QLabel(this);
    //titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(timeLabel);

    if(m_sim != NULL)
    {
        std::string label = m_sim->name();
        titleLabel->setText(label.c_str());
        timeLabel->setText(QString("t = %1 s").arg(m_sim->t()));
    }

    setLayout(mainLayout);

    m_timer = new QTimer;
    if(m_refreshRate == 0 && m_sim != NULL)
        m_timer->start(1000*m_sim->dt()/m_sim->speed());
    else
        m_timer->start(refreshRate);

    connect(m_timer, SIGNAL(timeout()), this, SLOT(updateSlot()));

    move(0, parent->height()-GuiConstants::monitorH);
    setMinimumSize(parent->width(),GuiConstants::monitorH);
}


Monitor::~Monitor()
{

}

void Monitor::setSimulation(Simulation *sim)
{
    m_sim = sim;

    if(m_sim != NULL)
    {
        std::string label = m_sim->name();
        titleLabel->setText(label.c_str());
        timeLabel->setText(QString("t = %1 s").arg(m_sim->t()));
        if(m_refreshRate == 0)
            m_timer->start(1000*m_sim->dt()/m_sim->speed());
    }

    return;
}

void Monitor::updateSlot(void)
{
    if(m_sim != NULL)
    {
        std::string label = m_sim->name();
        titleLabel->setText(label.c_str());
        timeLabel->setText(QString("t = %1 s").arg(m_sim->t()));
        if(m_refreshRate == 0)
            m_timer->setInterval(1000*m_sim->dt()/m_sim->speed());
    }

    return;
}

void Monitor::onResize(int w, int h)
{
    move(0, h-GuiConstants::monitorH);
    setMinimumSize(w,GuiConstants::monitorH);
}
