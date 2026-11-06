#include "include/AutoClicker.h"
#include <QtWidgets/QApplication>
#include <QLockFile>
#include <QDir>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    srand(time(NULL));
    QApplication a(argc, argv);

    QLockFile lockFile(QDir::temp().absoluteFilePath("AutoClicker_acidhood999.lock"));

    if (!lockFile.tryLock(100))
    {
        QMessageBox::warning(
            nullptr,
            "Attention",
            "The AutoClicker app is already running!"
        );
        return 1; 
    }

    AutoClicker w;
    w.show();

    return a.exec();
}
