#ifndef CONFIGUREWINDOW_H
#define CONFIGUREWINDOW_H

#include <QDialog>
#include <QGridLayout>
#include <QFormLayout>
#include <QFrame>
#include <QGroupBox>
#include <QLineEdit>
#include <QLabel>
#include <QSpinBox>
#include <QPushButton>
#include <QFileDialog>
#include <QEvent>
#include <QCheckBox>
#include <Simulation.h>
#include <QMessageBox>

class ConfigureWindow : public QDialog
{
    Q_OBJECT
public:
    explicit ConfigureWindow(bool isNew, Simulation *sim, QWidget *parent = 0);
    ~ConfigureWindow();

signals:

public slots:
     void confirmSlot(void);
     void updateSpeedLimits(double value);

private:
     bool m_isNew;
     Simulation* m_sim;

     QGridLayout *mainLayout;
     QGroupBox *simFrame;
     QGroupBox *planetFrame;
     QGroupBox *settingsFrame;
     QFormLayout *simForm;
     QFormLayout *planetForm;
     QFormLayout *settingsForm;
     QLineEdit *nameF;
     QLineEdit *planetNameF;
     QLineEdit *planetImgF;
     QLabel *planetLabel;
     QPixmap *planetPix;
     QDoubleSpinBox *planetRadius;
     QDoubleSpinBox *planetMu;
     QDoubleSpinBox *planetDay;
     QPushButton *confirmButton;
     QDoubleSpinBox *dtF;
     QDoubleSpinBox *speedF;
     QCheckBox *autoPlayF;
     QCheckBox *writeLogF;

     bool eventFilter(QObject *object, QEvent *event);

};

#endif // CONFIGUREWINDOW_H
