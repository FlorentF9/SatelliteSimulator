#include "SatelliteWindow.h"

SatelliteWindow::SatelliteWindow(bool isNew, Satellite *sat, Planet *planet, QWidget *parent) : QDialog(parent)
{
    m_isNew  = isNew;
    m_sat    = sat;
    m_planet = planet;

    setModal(true);
    setWindowTitle("Configure satellite");
    mainLayout = new QVBoxLayout;
    satFrame   = new QGroupBox("General information", this);
    orbFrame   = new QGroupBox("Orbit", this);
    attFrame   = new QGroupBox("Attitude", this);
    propFrame  = new QGroupBox("Propulsion engine", this);

    mainLayout->addWidget(satFrame);
    mainLayout->addWidget(orbFrame);
    mainLayout->addWidget(attFrame);
    mainLayout->addWidget(propFrame);

    satForm  = new QFormLayout(satFrame);
    orbForm  = new QFormLayout(orbFrame);
    attForm  = new QFormLayout(attFrame);
    propForm = new QFormLayout(propFrame);

    //General information
    satNameF = new QLineEdit;
    QRegExp filter("^[^\]\[ ]+$");
    QRegExpValidator *v = new QRegExpValidator(filter);
    satNameF->setValidator(v);
    satForm->addRow("Satellite name:", satNameF);
    importButton = new QPushButton("Import satellite");
    satForm->addWidget(importButton);

    //Orbit
    aBox  = new QDoubleSpinBox;
    aBox->setDecimals(3);
    aBox->setSingleStep(1.0);
    aBox->setMinimum(m_planet->getRadius()); // Initially set minimum to R but adapt it in onEChanged slot
    aBox->setMaximum(Constants::maxSatA);
    aBox->setToolTip(QString("[%1 , %2]").arg(aBox->minimum()).arg(aBox->maximum()));
    orbForm->addRow("Semimajor axis (km):", aBox);
    eBox  = new QDoubleSpinBox;
    eBox->setDecimals(8);
    eBox->setSingleStep(0.01);
    eBox->setMinimum(0.0);
    eBox->setMaximum(0.999); // Initially set maximum to 1.0 but adapt it in onAChanged slot
    eBox->setToolTip(QString("[%1 , %2]").arg(eBox->minimum()).arg(eBox->maximum()));
    orbForm->addRow("Eccentricity:", eBox);
    iBox  = new QDoubleSpinBox;
    iBox->setDecimals(4);
    iBox->setSingleStep(0.1);
    iBox->setMinimum(0.0);
    iBox->setMaximum(Constants::pi);
    iBox->setToolTip(QString("[%1 , %2]").arg(iBox->minimum()).arg(iBox->maximum()));
    orbForm->addRow("Inclination (rad):", iBox);
    OmBox  = new QDoubleSpinBox;
    OmBox->setDecimals(4);
    OmBox->setSingleStep(0.1);
    OmBox->setMinimum(0.0);
    OmBox->setMaximum(Constants::twopi);
    OmBox->setToolTip(QString("[%1 , %2]").arg(OmBox->minimum()).arg(OmBox->maximum()));
    orbForm->addRow("Longitude of the ascending node (rad):", OmBox);
    omBox  = new QDoubleSpinBox;
    omBox->setDecimals(4);
    omBox->setSingleStep(0.1);
    omBox->setMinimum(0.0);
    omBox->setMaximum(Constants::twopi);
    omBox->setToolTip(QString("[%1 , %2]").arg(omBox->minimum()).arg(omBox->maximum()));
    orbForm->addRow("Argument of periapsis (rad):", omBox);
    tpBox  = new QDoubleSpinBox;
    tpBox->setDecimals(4);
    tpBox->setSingleStep(1.0);
    double period = 1.0/Constants::twopi*std::sqrt(std::pow(aBox->value(), 3.0)/m_planet->getMu());
    tpBox->setRange(-period, period); // Update with onAChanged slot
    tpBox->setToolTip(QString("[%1 , %2]").arg(tpBox->minimum()).arg(tpBox->maximum()));
    orbForm->addRow("Epoch (s) - can be negative:", tpBox);

    //Attitude
    rxBox = new QDoubleSpinBox;
    rxBox->setDecimals(4);
    rxBox->setSingleStep(0.1);
    rxBox->setMinimum(0.0);
    rxBox->setMaximum(Constants::twopi);
    rxBox->setToolTip(QString("[%1 , %2]").arg(rxBox->minimum()).arg(rxBox->maximum()));
    attForm->addRow("Rotation around X axis (rad):", rxBox);

    ryBox = new QDoubleSpinBox;
    ryBox->setDecimals(4);
    ryBox->setSingleStep(0.1);
    ryBox->setMinimum(0.0);
    ryBox->setMaximum(Constants::twopi);
    ryBox->setToolTip(QString("[%1 , %2]").arg(ryBox->minimum()).arg(ryBox->maximum()));
    attForm->addRow("Rotation around Y axis (rad):", ryBox);

    rzBox = new QDoubleSpinBox;
    rzBox->setDecimals(4);
    rzBox->setSingleStep(0.1);
    rzBox->setMinimum(0.0);
    rzBox->setMaximum(Constants::twopi);
    rzBox->setToolTip(QString("[%1 , %2]").arg(rzBox->minimum()).arg(rzBox->maximum()));
    attForm->addRow("Rotation around Z axis (rad):", rzBox);

    //Propulsion
    //TO DO

    //Confirm button
    confirmButton = new QPushButton;
    if(isNew)
        confirmButton->setText("Add satellite");
    else
        confirmButton->setText("Apply");
    confirmButton->setDefault(true);
    mainLayout->addWidget(confirmButton);

    setLayout(mainLayout);

    //If existing satellite, use its values in form, else default values
    if(!isNew)
    {
        satNameF->setText(m_sat->getName().c_str());
        aBox->setValue(m_sat->getOrbit()->getA());
        aBox->setMinimum(m_planet->getRadius()/(1.0-m_sat->getOrbit()->gete()));
        aBox->setToolTip(QString("[%1 , %2]").arg(aBox->minimum()).arg(aBox->maximum()));
        eBox->setValue(m_sat->getOrbit()->gete());
        eBox->setMaximum(1.0-m_planet->getRadius()/m_sat->getOrbit()->getA());
        eBox->setToolTip(QString("[%1 , %2]").arg(eBox->minimum()).arg(eBox->maximum()));
        iBox->setValue(m_sat->getOrbit()->getI());
        OmBox->setValue(m_sat->getOrbit()->getOmega());
        omBox->setValue(m_sat->getOrbit()->getomega());
        tpBox->setValue(m_sat->getOrbit()->getTp());
        rxBox->setValue(m_sat->getRx());
        ryBox->setValue(m_sat->getRy());
        rzBox->setValue(m_sat->getRz());
    }
    else
    {
        satNameF->setText("Satellite");
        aBox->setValue(m_planet->a_geo());
        eBox->setValue(0.0);
        iBox->setValue(0.0);
        OmBox->setValue(0.0);
        omBox->setValue(0.0);
        tpBox->setValue(0.0);
        rxBox->setValue(0.0);
        ryBox->setValue(0.0);
        rzBox->setValue(0.0);
    }

    connect(aBox, SIGNAL(valueChanged(double)), this, SLOT(onAChanged(double)));
    connect(eBox, SIGNAL(valueChanged(double)), this, SLOT(onEChanged(double)));
    connect(confirmButton, SIGNAL(clicked()), this, SLOT(confirmSlot()));
}

SatelliteWindow::~SatelliteWindow()
{

}

void SatelliteWindow::confirmSlot(void)
{
    if(satNameF->text().isEmpty())
    {
        QMessageBox::warning(this, "Empty name", "Please enter a satellite name !");
        return;
    }
    m_sat->setName(satNameF->text().toStdString());
    m_sat->getOrbit()->setA(aBox->value());
    m_sat->getOrbit()->sete(eBox->value());
    m_sat->getOrbit()->setI(iBox->value());
    m_sat->getOrbit()->setOmega(OmBox->value());
    m_sat->getOrbit()->setomega(omBox->value());
    m_sat->getOrbit()->setTp(tpBox->value());
    m_sat->setRx(rxBox->value());
    m_sat->setRy(ryBox->value());
    m_sat->setRz(rzBox->value());

    done(QDialog::Accepted);
}

void SatelliteWindow::onAChanged(double a)
{
    //update e maximum
    eBox->setMaximum(1.0-m_planet->getRadius()/a);
    eBox->setToolTip(QString("[%1 , %2]").arg(eBox->minimum()).arg(eBox->maximum()));
    //update tp range
    double period = 1.0/Constants::twopi*std::sqrt(std::pow(aBox->value(), 3.0)/m_planet->getMu());
    tpBox->setRange(-period, period);
    tpBox->setToolTip(QString("[%1 , %2]").arg(tpBox->minimum()).arg(tpBox->maximum()));

    return;
}

void SatelliteWindow::onEChanged(double e)
{
    //update a minimum
    aBox->setMinimum(m_planet->getRadius()/(1.0-e));
    aBox->setToolTip(QString("[%1 , %2]").arg(aBox->minimum()).arg(aBox->maximum()));

    return;
}
