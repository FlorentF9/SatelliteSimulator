#include <MainWindow.h>
#include <QGLWidget>
#include <QResizeEvent>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{

    //Set window properties
    setWindowTitle(Constants::programName.c_str());
    setWindowIcon(QIcon("icon.png"));

    //Set window menu
    menuFile = menuBar()->addMenu("&File");
    menuSim = menuBar()->addMenu("&Simulation");
    menuSat = menuBar()->addMenu("&Satellites");
    menuHelp = menuBar()->addMenu("&?");

        //File menu
        QAction *actionNew = new QAction("&New simulation", this);
        menuFile->addAction(actionNew);
        actionNew->setShortcut(QKeySequence("Ctrl+N"));
        QAction *actionOpen = new QAction("&Open existing simulation", this);
        menuFile->addAction(actionOpen);
        actionOpen->setShortcut(QKeySequence("Ctrl+O"));
        QAction *actionSave = new QAction("&Save simulation", this);
        menuFile->addAction(actionSave);
        actionSave->setShortcut(QKeySequence("Ctrl+S"));
        menuFile->addAction(menuFile->addSeparator());
        // --------
        QAction *actionQuit = new QAction("&Quit", this);
        menuFile->addAction(actionQuit);
        actionQuit->setShortcut(QKeySequence("Ctrl+Q"));

        //Simulation menu
        actionTogglePlay = new QAction("", this);
        menuSim->addAction(actionTogglePlay);
        actionTogglePlay->setShortcut(QKeySequence("Space"));
        if(Constants::autoPlay)
            actionTogglePlay->setText("Pause");
        else
            actionTogglePlay->setText("Play");
        menuSim->addAction(menuFile->addSeparator());
        // --------
        QAction *actionConfigure = new QAction("Configure", this);
        menuSim->addAction(actionConfigure);
        actionConfigure->setShortcut(QKeySequence("Ctrl+C"));
        menuSim->addAction(menuFile->addSeparator());
        // --------
        QAction *actionReset = new QAction("Reset to t = 0", this);
        menuSim->addAction(actionReset);
        actionReset->setShortcut(QKeySequence("Ctrl+R"));
        QAction *actionResetAll = new QAction("Reset all", this);
        menuSim->addAction(actionResetAll);
        actionResetAll->setShortcut(QKeySequence("Ctrl+Shift+R"));

        //Satellites menu
        //list of all satellites will be inserted here...
        menuSat->addAction(menuFile->addSeparator());
        // --------
        QAction *actionAddSat = new QAction("Add new satellite", this);
        menuSat->addAction(actionAddSat);
        actionAddSat->setShortcut(QKeySequence("Ctrl+A"));

        //Help menu
        QAction *actionAbout = new QAction("About", this);
        menuHelp->addAction(actionAbout);

    /*
    //Status bar
    QStatusBar* sb = new QStatusBar(this);
    setStatusBar(sb);
    */

    //Add simulation display
    m_simDisplay = new SimulationDisplay(GuiConstants::fps, this, "3D simulation rendering");
    m_simDisplay->move(0, 0);
    m_simDisplay->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setCentralWidget(m_simDisplay);

    //Add simulation monitor
    m_monitor = new Monitor(NULL, m_simDisplay);


    //Connect menu actions to slots
        //Quit program
        connect(actionQuit, SIGNAL(triggered()), this, SLOT(close()));
        //About...
        connect(actionAbout, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
        //Toggle play
        connect(actionTogglePlay, SIGNAL(triggered()), this, SLOT(togglePlaySlot()));
        //Configuration
        connect(actionConfigure, SIGNAL(triggered()), this, SLOT(configureSlot()));
        //Reset
        connect(actionReset, SIGNAL(triggered()), this, SLOT(resetSlot()));
        //Reset all
        connect(actionResetAll, SIGNAL(triggered()), this, SLOT(resetAllSlot()));
        //New simulation
        connect(actionNew, SIGNAL(triggered()), this, SLOT(newSlot()));
        //Open simulation
        connect(actionOpen, SIGNAL(triggered()), this, SLOT(openSlot()));
        //Save simulation
        connect(actionSave, SIGNAL(triggered()), this, SLOT(saveSlot()));
        //Connect satellite menu
        connect(menuSat, SIGNAL(hovered(QAction*)), this, SLOT(hoveredSatelliteSlot(QAction*)));

    //Make monitor invisible at the beginning
    m_monitor->setVisible(false);
}

MainWindow::~MainWindow()
{

}

void MainWindow::togglePlaySlot(void)
{
    if(m_simDisplay != NULL && m_simDisplay->sim() != NULL)
    {
        m_simDisplay->sim()->togglePlay();
        if(m_simDisplay->sim()->play())
            actionTogglePlay->setText("Pause");
        else
            actionTogglePlay->setText("Play");
    }
}

void MainWindow::configureSlot(void)
{
    if(m_simDisplay != NULL && m_simDisplay->sim() != NULL)
    {
        m_simDisplay->sim()->setPlay(false);
        ConfigureWindow *confWindow = new ConfigureWindow(false, m_simDisplay->sim());
        if(confWindow->exec())
        {
            setWindowTitle(m_simDisplay->sim()->name().c_str());
            m_simDisplay->timer()->setInterval(1000*m_simDisplay->sim()->dt()/m_simDisplay->sim()->speed());
        }
    }

    return;
}

void MainWindow::addSatelliteSlot(void)
{
    if(m_simDisplay != NULL && m_simDisplay->sim() != NULL)
    {
        m_simDisplay->sim()->setPlay(false);

        //Allocate new satellite
        Satellite* sat = new Satellite(Orbit(m_simDisplay->sim()->getPlanet(), 1.0,0.0,0.0), m_simDisplay->sim()->getPlanet(), Propulsion());
        SatelliteWindow *satWindow = new SatelliteWindow(true, sat,  m_simDisplay->sim()->getPlanet());

        if(satWindow->exec())
        {
            //Check wether satellite with the same name exists, if yes rename it
            unsigned int suffix  = 1;
            std::string  satName = sat->getName();
            bool         doublon = true;
            do
            {
                doublon = false;
                for(unsigned int i=0; i<m_simDisplay->sim()->nsat(); ++i)
                {
                    if(m_simDisplay->sim()->sat(i)->getName() == satName)
                    {
                        doublon = true;
                        satName = sat->getName()+QString("[%1]").arg(suffix).toStdString();
                        suffix++;
                        break;
                    }

                }
            }while(doublon);
            //Inform user if satellite has been renamed
            if(satName != sat->getName())
            {
                QMessageBox::information(this, "Name conflict", ("A satellite with named "+sat->getName()+" already exists. The created satellite has been automatically renamed to "+satName).c_str());
                sat->setName(satName);
            }
            m_simDisplay->sim()->addSatellite(*sat);
            updateSatMenu();
        }

        return;
    }

    return;
}

void MainWindow::remSatelliteSlot(void)
{
    if(m_simDisplay != NULL && m_simDisplay->sim() != NULL)
    {
        for(unsigned int i=0; i<m_simDisplay->sim()->nsat(); ++i)
        {
            if(m_simDisplay->sim()->sat(i)->getName() == m_hoveredSat.toStdString())
            {
                //Ask confirmation
                if(QMessageBox::question(this, "Remove satellite", "Remove this satellite?<br/>If you did not export your satellite, its configuration will be lost.", QMessageBox::Yes | QMessageBox::No)
                        == QMessageBox::Yes)
                {
                    m_simDisplay->sim()->remSatellite(i);
                    updateSatMenu();
                }
                return;
            }
        }
    }

    return;
}

void MainWindow::hoveredSatelliteSlot(QAction *action)
{
    if(action->text()    != "Add new satellite"
       && action->text() != "Configure"
       && action->text() != "Remove")
        m_hoveredSat = action->text();

    return;
}

void MainWindow::confSatelliteSlot(void)
{
    if(m_simDisplay != NULL && m_simDisplay->sim() != NULL)
    {
        m_simDisplay->sim()->setPlay(false);
        for(unsigned int i=0; i<m_simDisplay->sim()->nsat(); ++i)
        {
            if(m_simDisplay->sim()->sat(i)->getName() == m_hoveredSat.toStdString())
            {
                SatelliteWindow *satWindow = new SatelliteWindow(false, m_simDisplay->sim()->sat(i),  m_simDisplay->sim()->getPlanet());
                satWindow->exec();
                updateSatMenu();
                return;
            }
        }
    }

    return;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(m_simDisplay != NULL && m_simDisplay->sim() != NULL)
    {
        m_simDisplay->sim()->setPlay(false);

        //Save and close simulation
        switch(showSaveMsgBox("Quit"))
        {
            case QMessageBox::Save:
                //Save and quit
                saveSimulation();
                event->accept();
                break;
            case QMessageBox::Discard:
                //quit
                event->accept();
                break;
            case QMessageBox::Cancel:
                //Don't quit
                event->ignore();
                return;
                break;
            default:
                event->ignore();
                return;
                break;
        };

        delete m_simDisplay->sim();
        m_simDisplay->setSimulation(NULL);
    }

}

void MainWindow::newSlot(void)
{
    if(m_simDisplay != NULL)
    {
        if(m_simDisplay->sim() != NULL)
        {
            m_simDisplay->sim()->setPlay(false);

            //Save and close simulation
            switch(showSaveMsgBox("New simulation"))
            {
                case QMessageBox::Save:
                    //Save and continue
                    saveSimulation();
                    break;
                case QMessageBox::Discard:
                    //Continue
                    break;
                case QMessageBox::Cancel:
                    //Quit creating a new simulation
                    return;
                    break;
                default:
                    return;
                    break;
            };

            delete m_simDisplay->sim();
            m_simDisplay->setSimulation(NULL);
            m_monitor->setSimulation(NULL);
        }

        //Allocate new simulation
        Planet* planet = new Planet();
        std::string name = "New_Simulation";

        Simulation* sim = new Simulation(planet, name);

        ConfigureWindow *newWindow = new ConfigureWindow(true, sim);
        if(newWindow->exec())
        {
            m_simDisplay->setSimulation(sim);
            m_monitor->setSimulation(sim);
            //Force load planet texture
            m_simDisplay->loadTexture(m_simDisplay->sim()->getPlanet()->getImgPath().c_str(), 0);

            setWindowTitle(m_simDisplay->sim()->name().c_str());
            m_simDisplay->timer()->setInterval(1000*m_simDisplay->sim()->dt()/m_simDisplay->sim()->speed());
            updateSatMenu();
        }

        //Update monitor size
        m_monitor->onResize(m_simDisplay->width(),
                            m_simDisplay->height());
        m_monitor->setVisible(true);
    }


    return;
}

void MainWindow::openSlot(void)
{
    if(m_simDisplay != NULL)
    {
        if(m_simDisplay->sim() != NULL)
        {
            m_simDisplay->sim()->setPlay(false);

            //Save and close simulation
            switch(showSaveMsgBox("Open simulation"))
            {
                case QMessageBox::Save:
                    //Save and continue
                    saveSimulation();
                    break;
                case QMessageBox::Discard:
                    //Continue
                    break;
                case QMessageBox::Cancel:
                    //Quit opening a simulation
                    return;
                    break;
                default:
                    return;
                    break;
            };

            delete m_simDisplay->sim();
            m_simDisplay->setSimulation(NULL);
            m_monitor->setSimulation(NULL);
        }

        QString filePath = QFileDialog::getOpenFileName(this, "Open simulation", QDir::currentPath(), "Simulations (*.sim)");

        if(!filePath.isEmpty())
        {
            //Open file
            std::ifstream file(filePath.toStdString().c_str());
            if(!file)
            {
                QMessageBox::critical(this, "Error", "Error opening simulation "+filePath+".sim");
                return;
            }

            //Load data
            std::string line;
            std::istringstream token;
            std::string simName;
            double simTime = 0.0;
            double simDt = 1.0;
            double simSpeed = 1.0;
            unsigned int n = 0;
            std::string planetName = Constants::defaultPlanetName;
            double planetRadius = Constants::r_earth;
            double planetMu = Constants::mu_earth;
            double planetDay = Constants::day_earth;
            std::string planetImg = Constants::defaultImgPath;
            std::vector<std::string> satNames;
            std::vector<double> orbitA;
            std::vector<double> orbitE;
            std::vector<double> orbitI;
            std::vector<double> orbitOm;
            std::vector<double> orbitom;
            std::vector<double> orbitTp;
            std::vector<double> orbitM;

            while(std::getline(file, line))
            {
                if(line == "----------")
                {
                    std::getline(file, line);
                    if(line == "Simulation")
                    {
                        //Simulation name
                        std::getline(file, line);
                        token.str(line.substr(line.find(':')+2, line.length()));
                        token >> simName;
                        //Current time
                        std::getline(file, line);
                        token.str(line.substr(line.find(':')+2, line.length()));
                        token.clear();
                        token >> simTime;
                        //Time step
                        std::getline(file, line);
                        token.str(line.substr(line.find(':')+2, line.length()));
                        token.clear();
                        token >> simDt;
                        //Speed
                        std::getline(file, line);
                        token.str(line.substr(line.find(':')+2, line.length()));
                        token.clear();
                        token >> simSpeed;
                        //Number of satellites
                        std::getline(file, line);
                        token.str(line.substr(line.find(':')+2, line.length()));
                        token.clear();
                        token >> n;
                    }
                    else if(line == "Planet")
                    {
                        //Planet name
                        std::getline(file, line);
                        token.str(line.substr(line.find(':')+2, line.length()));
                        token.clear();
                        token >> planetName;
                        //Planet radius
                        std::getline(file, line);
                        token.str(line.substr(line.find(':')+2, line.length()));
                        token.clear();
                        token >> planetRadius;
                        //Planet mu
                        std::getline(file, line);
                        token.str(line.substr(line.find(':')+2, line.length()));
                        token.clear();
                        token >> planetMu;
                        //Planet day
                        std::getline(file, line);
                        token.str(line.substr(line.find(':')+2, line.length()));
                        token.clear();
                        token >> planetDay;
                        //Planet imgPath
                        std::getline(file, line);
                        token.str(line.substr(line.find(':')+2, line.length()));
                        token.clear();
                        token >> planetImg;
                    }
                    else if(line == "Satellites")
                    {
                        satNames.resize(n);
                        orbitA.resize(n);
                        orbitE.resize(n);
                        orbitI.resize(n);
                        orbitOm.resize(n);
                        orbitom.resize(n);
                        orbitTp.resize(n);
                        orbitM.resize(n);

                        for(unsigned int i=0; i<n; ++i)
                        {
                            // ---------- line
                            std::getline(file, line);
                            //Satellite name
                            std::getline(file, line);
                            token.str(line.substr(line.find(':')+2, line.length()));
                            token.clear();
                            token >> satNames[i];
                            //a
                            std::getline(file, line);
                            token.str(line.substr(line.find(':')+2, line.length()));
                            token.clear();
                            token >> orbitA[i];
                            //e
                            std::getline(file, line);
                            token.str(line.substr(line.find(':')+2, line.length()));
                            token.clear();
                            token >> orbitE[i];
                            //i
                            std::getline(file, line);
                            token.str(line.substr(line.find(':')+2, line.length()));
                            token.clear();
                            token >> orbitI[i];
                            //Omega
                            std::getline(file, line);
                            token.str(line.substr(line.find(':')+2, line.length()));
                            token.clear();
                            token >> orbitOm[i];
                            //omega
                            std::getline(file, line);
                            token.str(line.substr(line.find(':')+2, line.length()));
                            token.clear();
                            token >> orbitom[i];
                            //tp
                            std::getline(file, line);
                            token.str(line.substr(line.find(':')+2, line.length()));
                            token.clear();
                            token >> orbitTp[i];
                            //M
                            std::getline(file, line);
                            token.str(line.substr(line.find(':')+2, line.length()));
                            token.clear();
                            token >> orbitM[i];
                        }
                    }
                }
            }

            //Allocate simulation
            Planet* planet = new Planet(planetMu, planetRadius, planetDay, planetName, planetImg);
            Simulation* sim = new Simulation(planet, simName, simSpeed, simDt);
            sim->setT(simTime);
            //Add satellites and set their position
            for(unsigned int i=0; i<n; ++i)
            {
                Orbit orbit(planet, orbitA[i], orbitE[i], orbitI[i], orbitOm[i], orbitom[i], orbitTp[i]);
                orbit.setM(orbitM[i]);
                Satellite sat(orbit, planet, Propulsion(), satNames[i]);
                sim->addSatellite(sat);
            }
            m_simDisplay->setSimulation(sim);
            m_monitor->setSimulation(sim);
            //Force load planet texture
            m_simDisplay->loadTexture(m_simDisplay->sim()->getPlanet()->getImgPath().c_str(), 0);

            setWindowTitle(m_simDisplay->sim()->name().c_str());
            m_simDisplay->timer()->setInterval(1000*m_simDisplay->sim()->dt()/m_simDisplay->sim()->speed());
            m_simDisplay->updateGL();
            updateSatMenu();
        }

        //Update monitor size
        m_monitor->onResize(m_simDisplay->width(),
                            m_simDisplay->height());
        m_monitor->setVisible(true);

    }

    return;
}

void MainWindow::resetSlot(void)
{
    if(m_simDisplay != NULL && m_simDisplay->sim() != NULL)
    {
        //Pause
        m_simDisplay->sim()->setPlay(false);
        //Reset
        m_simDisplay->sim()->reset();
    }
}

void MainWindow::resetAllSlot(void)
{
    if(m_simDisplay != NULL && m_simDisplay->sim() != NULL)
    {
        //Pause
        m_simDisplay->sim()->setPlay(false);
        //Ask confirmation
        if(QMessageBox::question(this, "Reset all simulation", "Remove all satellites and reset simulation?<br/>If you did not export your satellites, their configuration will be lost.", QMessageBox::Yes | QMessageBox::No)
                == QMessageBox::Yes)
        {
            //Remove all satellites and reset
            m_simDisplay->sim()->resetAll();
            updateSatMenu();
        }
    }
}

int MainWindow::showSaveMsgBox(QString title)
{
    QMessageBox* saveQuestionBox = new QMessageBox(this);
    saveQuestionBox->setWindowTitle(title);
    saveQuestionBox->setIcon(QMessageBox::Question);

    saveQuestionBox->addButton(QMessageBox::Save);
    saveQuestionBox->addButton(QMessageBox::Discard);
    saveQuestionBox->addButton(QMessageBox::Cancel);

    saveQuestionBox->setDefaultButton(QMessageBox::Save);

    saveQuestionBox->setText("Do you want to save your current simulation?<br/>\
If you click \"Discard\", you will lose your simulation current state and unsaved settings. Only the logged data will be kept on disk.");

    return saveQuestionBox->exec();
}

void MainWindow::saveSimulation(void)
{
    if(m_simDisplay != NULL && m_simDisplay->sim() != NULL)
    {
        //Pause
        m_simDisplay->sim()->setPlay(false);

        QString filePath = QFileDialog::getSaveFileName(this, "Save simulation", (m_simDisplay->sim()->name()+".sim").c_str(), "Simulations (*.sim)");

        if(!filePath.isEmpty())
        {
            if(m_simDisplay->sim()->saveToFile(filePath.toStdString(), QDateTime::currentDateTime().toString().toStdString()) != 0)
                QMessageBox::critical(this, "Error", ("Error saving simulation "+m_simDisplay->sim()->name()+".sim").c_str());
        }
    }
    return;
}

void MainWindow::updateSatMenu(void)
{
    menuSat->clear();

    //list of all satellites
    for(unsigned int i=0; i<m_simDisplay->sim()->nsat(); ++i)
    {
        QMenu   *sat  = menuSat->addMenu(m_simDisplay->sim()->sat(i)->getName().c_str());
        QAction *conf = new QAction("Configure", this);
        QAction *rem  = new QAction("Remove", this);
        sat->addAction(conf);
        sat->addAction(rem);
        connect(conf, SIGNAL(triggered()), this, SLOT(confSatelliteSlot()));
        connect(rem, SIGNAL(triggered()), this, SLOT(remSatelliteSlot()));
    }

    menuSat->addAction(menuFile->addSeparator());
    // --------
    QAction *actionAddSat = new QAction("Add new satellite", this);
    menuSat->addAction(actionAddSat);
    actionAddSat->setShortcut(QKeySequence("Ctrl+A"));

    connect(actionAddSat, SIGNAL(triggered()), this, SLOT(addSatelliteSlot()));

    return;
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    m_monitor->onResize(event->size().width(),
                        event->size().height());

    return;
}
