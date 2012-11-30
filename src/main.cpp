#include <QApplication>
#include <WindowGame.h>

int main(int argc, char* argv[])
{

    QApplication application(argc,argv);

    WindowGame window;
    window.show();

    return application.exec();

}

