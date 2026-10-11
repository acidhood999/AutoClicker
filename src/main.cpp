#include "include/AutoClicker.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    srand(time(NULL));
    QApplication app(argc, argv);
    AutoClicker window;
    return app.exec();
}
