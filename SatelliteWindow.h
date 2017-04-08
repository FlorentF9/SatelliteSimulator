#ifndef SATELLITEWINDOW_H
#define SATELLITEWINDOW_H

#include <QDialog>
#include <Satellite.h>
#include <QLineEdit>
#include <QLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QMessageBox>

class SatelliteWindow : public QDialog
{
    Q_OBJECT
public:
    SatelliteWindow(bool isNew, Satellite *sat, Planet *planet, QWidget *parent = 0);
    ~SatelliteWindow();

public slots:
    void onAChanged(double a);
    void onEChanged(double e);
    void confirmSlot(void);

private:
    bool       m_isNew;
    Satellite* m_sat;
    Planet*    m_planet;

    QVBoxLayout *mainLayout;
    QGroupBox   *satFrame;
    QGroupBox   *orbFrame;
    QGroupBox   *attFrame;
    QGroupBox   *propFrame;
    QFormLayout *satForm;
    QFormLayout *orbForm;
    QFormLayout *attForm;
    QFormLayout *propForm;
    QPushButton *confirmButton;

    //Satellite form
    QPushButton* importButton;
    QLineEdit*   satNameF;

    //Orbit form
    QDoubleSpinBox* aBox;
    QDoubleSpinBox* eBox;
    QDoubleSpinBox* iBox;
    QDoubleSpinBox* OmBox;
    QDoubleSpinBox* omBox;
    QDoubleSpinBox* tpBox;

    //Attitude
    QDoubleSpinBox* rxBox;
    QDoubleSpinBox* ryBox;
    QDoubleSpinBox* rzBox;


};

#endif // SATELLITEWINDOW_H
