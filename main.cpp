#include "mainwindow.h"

#include <QApplication>
#include <vector>
#include <string>
#include <iostream>
#include <QDir>
#include <QLockFile>
#include <QMessageBox>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString path = QDir::temp().absoluteFilePath("KD_crypto_SingleApp.lock");
    QLockFile lockFile(path);
    if (!lockFile.tryLock(100))
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText("The application is already running.\n"
                       "Allowed to run only one instance of the application.");
        msgBox.exec();
        return 1;
    }

    MainWindow w;
    w.show();
    return a.exec();
}
