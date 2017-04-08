#include <SimulationDisplay.h>

SimulationDisplay::SimulationDisplay(int framesPerSecond, QWidget *parent, char const* name, Simulation* sim)
: SimulationGL(framesPerSecond, parent, name)
{
    m_sim  = sim;
    planetTexturePath = Constants::defaultImgPath.c_str();
    m_camera = NULL;

    if(m_sim != NULL)
    {
        planetTexturePath = m_sim->getPlanet()->getImgPath().c_str();
        sim_Timer = new QTimer(this);
        connect(sim_Timer, SIGNAL(timeout()), this, SLOT(simUpdateSlot()));
        sim_Timer->start(1000*m_sim->dt()/m_sim->speed());

        m_camera = new TrackBallCamera();
        setCursor(QCursor(Qt::OpenHandCursor));
    }
}

void SimulationDisplay::setSimulation(Simulation* sim)
{
    m_sim = sim;
    planetTexturePath = Constants::defaultImgPath.c_str();
    m_camera = NULL;

    if(m_sim != NULL)
    {
        planetTexturePath = m_sim->getPlanet()->getImgPath().c_str();
        sim_Timer = new QTimer(this);
        connect(sim_Timer, SIGNAL(timeout()), this, SLOT(simUpdateSlot()));
        sim_Timer->start(1000*m_sim->dt()/m_sim->speed());

        m_camera = new TrackBallCamera();
        setCursor(QCursor(Qt::OpenHandCursor));
    }
}

SimulationDisplay::~SimulationDisplay()
{
    delete m_sim;
    delete m_camera;
}

void SimulationDisplay::simUpdateSlot(void)
{
    if(m_sim != NULL)
        m_sim->update();

    return;
}

void SimulationDisplay::initializeGL()
{
    loadTexture(Constants::defaultImgPath.c_str(), 0);
    loadTexture("textures/gold_texture.jpg", 1);
    loadTexture("textures/solar_panel_2.jpg", 2);
    glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void SimulationDisplay::resizeGL(int width, int height)
{
    if(height == 0)
        height = 1;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (GLfloat)width/(GLfloat)height, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void SimulationDisplay::loadTexture(QString path, unsigned int i)
{
    QImage qim_Texture;
    QImage qim_TempTexture;
    qim_TempTexture.load(path);
    qim_Texture = QGLWidget::convertToGLFormat( qim_TempTexture );
    glGenTextures( 1, &texture[i] );
    glBindTexture( GL_TEXTURE_2D, texture[i] );
    glTexImage2D( GL_TEXTURE_2D, 0, 3, qim_Texture.width(), qim_Texture.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, qim_Texture.bits() );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
}

void SimulationDisplay::drawEllipse(const Orbit *orbit, double scale, int i, int n)
{
    glDisable(GL_TEXTURE_2D);
    glBegin(GL_LINE_LOOP);

    // hue [0, 360), saturation [0, 100), lightness [0, 100)
    //std::srand(0);
    double h = i*360.0/n;
    double s = 95;// + double(std::rand())/RAND_MAX * 10;
    double l = 100;// + double(std::rand())/RAND_MAX * 10;
    QColor c;
    c.setHsl(h, s, l);

    glColor3d(c.red()/255.0, c.green()/255.0, c.blue()/255.0);
    for(double M=0.0; M<Constants::twopi; M+=0.1)
    {
        PointPol pt = orbit->getPointAt(M);
        glVertex3d(pt.getY()*scale, pt.getZ()*scale, pt.getX()*scale);
    }
    glEnd();
    glEnable(GL_TEXTURE_2D);
}

void SimulationDisplay::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    if(m_sim != NULL)
    {
        m_camera->look();

        float scaleFactor = 25.0;

        //Get maxmimum RA for window scaling
        double ra_max = 0.0;
        for(unsigned int i = 0; i < m_sim->nsat(); ++i)
        {
            double ra   = m_sim->sat(i)->getOrbit()->getRa();
            if(ra_max < ra)
                ra_max = ra;
        }
        float scale = 0.0;
        if(ra_max == 0.0)
            ra_max = m_sim->getPlanet()->getRadius()*3.0;

        scale = float(scaleFactor/ra_max);

            //Get planet apparent size
            float r = m_sim->getPlanet()->getRadius()*scale;

            //Load texture if it has changed
            if(planetTexturePath != m_sim->getPlanet()->getImgPath().c_str())
            {
                planetTexturePath = m_sim->getPlanet()->getImgPath().c_str();
                loadTexture(planetTexturePath, 0);
            }

            glDisable(GL_TEXTURE_2D);
            //Draw axes
            glBegin(GL_LINES);
            // inertial X
            glColor3d(1.0, 0.0, 0.0);
            glVertex3d(0.0, 0.0, 0.0);
            glVertex3d(0.0, 0.0, 3.0*r);
            // inertial Y
            glColor3d(0.0, 1.0, 0.0);
            glVertex3d(0.0, 0.0, 0.0);
            glVertex3d(3.0*r, 0.0, 0.0);
            // inertial Z
            glColor3d(0.0, 0.0, 1.0);
            glVertex3d(0.0, 0.0, 0.0);
            glVertex3d(0.0, 3.0*r, 0.0);
            glEnd();
            glColor3d(1.0, 1.0, 1.0);
            glEnable(GL_TEXTURE_2D);

            //Draw planet
          glPushMatrix();
            GLUquadric* params = gluNewQuadric();
            gluQuadricTexture(params,GL_TRUE);
            glBindTexture(GL_TEXTURE_2D, texture[0]);
            //Rotate to set north pole on top
            glRotatef(-90.0, 1.0, 0.0, 0.0);
            //Rotate around Z axis
            double angle = 360.0*(fmod(m_sim->t(), m_sim->getPlanet()->getDay())/m_sim->getPlanet()->getDay());
            glRotatef(angle, 0.0, 0.0, 1.0);
            gluSphere(params, r, 40, 40);
            gluDeleteQuadric(params);
          glPopMatrix();

        //Draw each satellite
        for(unsigned int i = 0; i < m_sim->nsat(); ++i)
        {
            //Sat size, scaled
            float s = 300.0*scale;

            //Get position
            PointPol pos = m_sim->sat(i)->getCurrentPosition();
            float x = float(pos.getX());
            float y = float(pos.getY());
            float z = float(pos.getZ());
            //Scale
            x *= scale;
            y *= scale;
            z *= scale;

            //Draw orbit ellipse
            drawEllipse(m_sim->sat(i)->getOrbit(), scale, i, m_sim->nsat());
            glColor3d(1.0, 1.0, 1.0);

            //Translate to sat position
            glTranslatef(y, z, x);

            //Draw intertial axes
            glDisable(GL_TEXTURE_2D);
            //Draw axes
            glBegin(GL_LINES);
            // inertial X
            glColor3d(1.0, 0.0, 0.0);
            glVertex3d(0.0, 0.0, 0.0);
            glVertex3d(0.0, 0.0, 3.0*s);
            // inertial Y
            glColor3d(0.0, 1.0, 0.0);
            glVertex3d(0.0, 0.0, 0.0);
            glVertex3d(3.0*s, 0.0, 0.0);
            // inertial Z
            glColor3d(0.0, 0.0, 1.0);
            glVertex3d(0.0, 0.0, 0.0);
            glVertex3d(0.0, 3.0*s, 0.0);
            glEnd();
            glColor3d(1.0, 1.0, 1.0);
            glEnable(GL_TEXTURE_2D);

            //Translate back
            //glTranslatef(-y, -z, -x);

            //Rotate according to satellite attitude
            double angle1 = 180.0/Constants::pi*m_sim->sat(i)->getRy();
            double angle2 = 180.0/Constants::pi*m_sim->sat(i)->getRz();
            double angle3 = 180.0/Constants::pi*m_sim->sat(i)->getRx();
            glRotatef(angle1, 1.0, 0.0, 0.0);
            glRotatef(angle2, 0.0, 1.0, 0.0);
            glRotatef(angle3, 0.0, 0.0, 1.0);

            //Translate to sat position
            //glTranslatef(y, z, x);

            //Draw satellite axes
            glDisable(GL_TEXTURE_2D);
            //Draw axes
            glBegin(GL_LINES);
            // inertial X
            glColor3d(1.0, 0.5, 0.5);
            glVertex3d(0.0, 0.0, 0.0);
            glVertex3d(0.0, 0.0, 3.0*s);
            // inertial Y
            glColor3d(0.5, 1.0, 0.5);
            glVertex3d(0.0, 0.0, 0.0);
            glVertex3d(3.0*s, 0.0, 0.0);
            // inertial Z
            glColor3d(0.5, 0.5, 1.0);
            glVertex3d(0.0, 0.0, 0.0);
            glVertex3d(0.0, 3.0*s, 0.0);
            glEnd();
            glColor3d(1.0, 1.0, 1.0);
            glEnable(GL_TEXTURE_2D);

            //Draw satellite
            //body
            glBindTexture(GL_TEXTURE_2D, texture[1]);
            glBegin(GL_QUADS);
                    // Face Avant
                    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0*s, -1.0*s,  s);
                    glTexCoord2f(s, 0.0f); glVertex3f( s, -1.0*s,  s);
                    glTexCoord2f(s, s); glVertex3f( s,  s,  s);
                    glTexCoord2f(0.0f, s); glVertex3f(-1.0*s,  s,  s);
                    // Face Arrière
                    glTexCoord2f(s, 0.0f); glVertex3f(-1.0*s, -1.0*s, -1.0*s);
                    glTexCoord2f(s, s); glVertex3f(-1.0*s,  s, -1.0*s);
                    glTexCoord2f(0.0f, s); glVertex3f( s,  s, -1.0*s);
                    glTexCoord2f(0.0f, 0.0f); glVertex3f( s, -1.0*s, -1.0*s);
                    // Face Haut
                    glTexCoord2f(0.0f, s); glVertex3f(-1.0*s,  s, -1.0*s);
                    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0*s,  s,  s);
                    glTexCoord2f(s, 0.0f); glVertex3f( s,  s,  s);
                    glTexCoord2f(s, s); glVertex3f( s,  s, -1.0*s);
                    // Face Bas
                    glTexCoord2f(s, s); glVertex3f(-1.0*s, -1.0*s, -1.0*s);
                    glTexCoord2f(0.0f, s); glVertex3f( s, -1.0*s, -1.0*s);
                    glTexCoord2f(0.0f, 0.0f); glVertex3f( s, -1.0*s,  s);
                    glTexCoord2f(s, 0.0f); glVertex3f(-1.0*s, -1.0*s,  s);
                    // Face Droite
                    glTexCoord2f(s, 0.0f); glVertex3f( s, -1.0*s, -1.0*s);
                    glTexCoord2f(s, s); glVertex3f( s,  s, -1.0*s);
                    glTexCoord2f(0.0f, s); glVertex3f( s,  s,  s);
                    glTexCoord2f(0.0f, 0.0f); glVertex3f( s, -1.0*s,  s);
                    // Face Gauche
                    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0*s, -1.0*s, -1.0*s);
                    glTexCoord2f(s, 0.0f); glVertex3f(-1.0*s, -1.0*s,  s);
                    glTexCoord2f(s, s); glVertex3f(-1.0*s,  s,  s);
                    glTexCoord2f(0.0f, s); glVertex3f(-1.0*s,  s, -1.0*s);
            glEnd();
            //solar panels
            glBindTexture(GL_TEXTURE_2D, texture[2]);
            glBegin(GL_QUADS);
                glTexCoord2f(0.0f, 0.0f); glVertex3f(-5.0*s, -0.8*s,  0.0);
                glTexCoord2f(s, 0.0f); glVertex3f( -1.2*s, -0.8*s,  0.0);
                glTexCoord2f(s, s); glVertex3f( -1.2*s,  0.8*s,  0.0);
                glTexCoord2f(0.0f, s); glVertex3f(-5.0*s,  0.8*s,  0.0);
            glEnd();
            glBegin(GL_QUADS);
                glTexCoord2f(0.0f, 0.0f); glVertex3f(1.2*s, -0.8*s,  0.0);
                glTexCoord2f(s, 0.0f); glVertex3f( 5.0*s, -0.8*s,  0.0);
                glTexCoord2f(s, s); glVertex3f( 5.0*s,  0.8*s,  0.0);
                glTexCoord2f(0.0f, s); glVertex3f(1.2*s,  0.8*s,  0.0);
            glEnd();

            //Rotate back to draw next satellite
            glRotatef(-angle3, 0.0, 0.0, 1.0);
            glRotatef(-angle2, 0.0, 1.0, 0.0);
            glRotatef(-angle1, 1.0, 0.0, 0.0);

            //Translate back to draw next satellite
            glTranslatef(-y, -z, -x);
        }
    }
}
