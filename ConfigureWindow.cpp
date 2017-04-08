#include "ConfigureWindow.h"

ConfigureWindow::ConfigureWindow(bool isNew, Simulation* sim, QWidget *parent) : QDialog(parent)
{
    m_isNew   = isNew;
    m_sim = sim;

    setModal(true);
    setWindowTitle("Configure simulation");
    mainLayout = new QGridLayout;
    simFrame = new QGroupBox("General information", this);
    planetFrame = new QGroupBox("Planet", this);
    settingsFrame = new QGroupBox("Simulation settings", this);

    mainLayout->addWidget(simFrame, 0, 0, 1, 2);
    mainLayout->addWidget(planetFrame, 1, 0);
    mainLayout->addWidget(settingsFrame, 1, 1);

    simForm = new QFormLayout(simFrame);
    planetForm = new QFormLayout(planetFrame);
    settingsForm = new QFormLayout(settingsFrame);

    //General information
    nameF = new QLineEdit;
    simForm->addRow("Simulation name:", nameF);

    //Planet name
    planetNameF = new QLineEdit;
    planetForm->addRow("Planet name:", planetNameF);
    //Planet radius
    planetRadius = new QDoubleSpinBox;
    planetRadius->setMinimum(Constants::minPlanetRadius);
    planetRadius->setMaximum(Constants::maxPlanetRadius);
    planetRadius->setDecimals(4);
    planetRadius->setSingleStep(1.0);
    planetForm->addRow("Radius (km):", planetRadius);
    //Planet mu
    planetMu = new QDoubleSpinBox;
    planetMu->setMinimum(Constants::minPlanetMu);
    planetMu->setMaximum(Constants::maxPlanetMu);
    planetMu->setDecimals(4);
    planetMu->setSingleStep(1.0);
    planetForm->addRow("mu (km^3/s^2):", planetMu);
    //Planet day duration
    planetDay = new QDoubleSpinBox;
    planetDay->setMinimum(Constants::minPlanetDay);
    planetDay->setMaximum(Constants::maxPlanetDay);
    planetDay->setSingleStep(1.0);
    planetForm->addRow("Sideral day duration (s):", planetDay);
    //Planet texture
    planetImgF = new QLineEdit;
    planetImgF->setReadOnly(true);
    planetForm->addRow("Planet texture:", planetImgF);
    planetImgF->installEventFilter(this);
    //Planet texture preview
    planetLabel = new QLabel;
    planetForm->addRow("Preview:", planetLabel);

    //Sim settings
    dtF = new QDoubleSpinBox;
    dtF->setMinimum(Constants::minTimeStep);
    dtF->setMaximum(Constants::maxTimeStep);
    dtF->setDecimals(3);
    settingsForm->addRow("Time step:", dtF);
    connect(dtF, SIGNAL(valueChanged(double)), this, SLOT(updateSpeedLimits(double)));
    speedF = new QDoubleSpinBox;
    speedF->setMinimum(dtF->value()/Constants::maxTimeStep); // Here we check
    speedF->setMaximum(dtF->value()/Constants::minTimeStep); // sim speed.
    settingsForm->addRow("Simulation speed factor:", speedF);
    writeLogF = new QCheckBox;
    settingsForm->addRow("Write simulation into log file", writeLogF);
    autoPlayF = new QCheckBox;
    settingsForm->addRow("Start simulation automatically", autoPlayF);

    //Confirm button
    confirmButton = new QPushButton;
    if(isNew)
        confirmButton->setText("Start simulation");
    else
        confirmButton->setText("Apply and reset simulation");
    confirmButton->setDefault(true);
    mainLayout->addWidget(confirmButton,2,0,1,2);
    setLayout(mainLayout);

    //If existing simulation, use its values in form
    if(!isNew)
    {
        nameF->setText(sim->name().c_str());
        planetNameF->setText(sim->getPlanet()->getName().c_str());
        planetImgF->setText(sim->getPlanet()->getImgPath().c_str());
        planetPix = new QPixmap(sim->getPlanet()->getImgPath().c_str());
        planetLabel->setPixmap(planetPix->scaled(300,150));
        planetRadius->setValue(sim->getPlanet()->getRadius());
        planetMu->setValue(sim->getPlanet()->getMu());
        planetDay->setValue(sim->getPlanet()->getDay());
        dtF->setValue(sim->dt());
        speedF->setValue(sim->speed());
        if(sim->play())
            autoPlayF->setChecked(true);
        if(sim->writeLog())
            writeLogF->setChecked(true);
    }
    else
    {
        //Default simulation name
        nameF->setText("New_Simulation");
        //Default planet name
        planetNameF->setText(Constants::defaultPlanetName.c_str());
        //Default texture path
        planetImgF->setText(Constants::defaultImgPath.c_str());
        //Default planet texture
        planetPix = new QPixmap(Constants::defaultImgPath.c_str());
        planetLabel->setPixmap(planetPix->scaled(300,150));
        //Default time step
        dtF->setValue(1.0);
        //Default speed
        speedF->setValue(1.0);
        //Default planet charasteristics (earth)
        planetRadius->setValue(Constants::r_earth);
        planetMu->setValue(Constants::mu_earth);
        planetDay->setValue(Constants::day_earth);

        writeLogF->setChecked(Constants::writeLog);
        autoPlayF->setChecked(Constants::autoPlay);
    }

    connect(confirmButton, SIGNAL(clicked()), this, SLOT(confirmSlot()));

}

ConfigureWindow::~ConfigureWindow()
{

}

bool ConfigureWindow::eventFilter(QObject* object, QEvent* event)
{
    if(object == planetImgF && event->type() == QEvent::MouseButtonPress) {
        QString imgPath = QFileDialog::getOpenFileName(this, "Select an image for your planet", QDir::currentPath(), "Images (*.jpg *.jpeg *.png *.bmp *.gif)");
        if(imgPath != NULL)
        {
            planetImgF->setText(imgPath);
            delete planetPix;
            planetPix = new QPixmap(imgPath);
            planetLabel->setPixmap(planetPix->scaled(300,150));
        }
        return false; // lets the event continue to the edit
    }
    return false;
}

void ConfigureWindow::confirmSlot(void)
{
    if(nameF->text().isEmpty())
    {
        QMessageBox::warning(this, "Empty name", "Please enter a simulation name !");
        return;
    }
    if(planetNameF->text().isEmpty())
    {
        QMessageBox::warning(this, "Empty name", "Please enter a planet name !");
        return;
    }
    m_sim->getPlanet()->setDay(planetDay->value());
    m_sim->getPlanet()->setMu(planetMu->value());
    m_sim->getPlanet()->setRadius(planetRadius->value());
    m_sim->getPlanet()->setName(planetNameF->text().toStdString());
    m_sim->getPlanet()->setImgPath(planetImgF->text().toStdString());
    m_sim->setName(nameF->text().toStdString());
    m_sim->setPlay(autoPlayF->isChecked());
    m_sim->setWriteLog(writeLogF->isChecked());
    m_sim->setSpeed(speedF->value());
    m_sim->setDt(dtF->value());
    m_sim->reset();

    done(QDialog::Accepted);
}

void ConfigureWindow::updateSpeedLimits(double value)
{
    speedF->setMinimum(value/Constants::maxTimeStep); // Here we check
    speedF->setMaximum(value/Constants::minTimeStep); // sim speed.
}
