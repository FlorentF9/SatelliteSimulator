# SatelliteSimulator

A simple Qt/OpenGL satellite orbit simulator.

## Build instructions

To build this project, you will need the **Qt5** and **Qt5 OpenGL** libraries. To install them on a Debian-based system :

```
$ sudo apt install qt5-default qttools5-dev-tools libqt5opengl5-dev
```

Then, generate the Makefile with `qmake`:

```
$ qmake
```

Before compiling, you will have to tell the compiler to use the **GLU** library. To achieve this, **modify the Makefile** and add `-lGLU` at the end of the line of the Makefile which starts with `LIBS`. The line should be:
```
LIBS          = $(SUBLIBS) -L/usr/X11R6/lib64 -lQt5OpenGL -lQt5Widgets -lQt5Gui -lQt5Core -lGL -lpthread -lGLU
```

Finally, compile and run the executable:

```
make && ./SatelliteSimulator
```
