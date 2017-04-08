#include <QApplication>
#include <QPushButton>

#include <MainWindow.h>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    MainWindow window;

    window.showMaximized();

    return app.exec();
}
