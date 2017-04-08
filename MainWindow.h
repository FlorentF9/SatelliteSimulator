#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <SimulationDisplay.h>
#include <GuiConstants.h>
#include <ConfigureWindow.h>
#include <SatelliteWindow.h>
#include <QAction>
#include <Monitor.h>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    int showSaveMsgBox(QString title);
    void saveSimulation(void);
    void updateSatMenu(void);

    void closeEvent(QCloseEvent *event);
    void resizeEvent(QResizeEvent *event);

public slots:
    void togglePlaySlot(void);
    void newSlot(void);
    void configureSlot(void);
    void resetSlot(void);
    void resetAllSlot(void);
    void openSlot(void);
    void saveSlot(void) {saveSimulation();}
    void addSatelliteSlot(void);
    void hoveredSatelliteSlot(QAction* action);
    void confSatelliteSlot(void);
    void remSatelliteSlot(void);


private:
     SimulationDisplay* m_simDisplay;

     Monitor* m_monitor;

     QAction *actionTogglePlay; //toggle play menu action (connected with slot)

     QString m_hoveredSat; //used to remember wich satellite sub-menu was opened

     //Menus
     QMenu *menuFile;
     QMenu *menuSim;
     QMenu *menuSat;
     QMenu *menuHelp;
};

#endif // MAINWINDOW_H
