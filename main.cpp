#include "mainwindow.h"

#include <QApplication>
#include <QDir>
#include <QLockFile>
#include <QMessageBox>

#include "NanoLog.h"

int main(int argc, char *argv[])
{
    nanolog::initialize(nanolog::GuaranteedLogger(), "./", "LOG_FILE",2);
    nanolog::set_log_level(nanolog::LogLevel::INFO);
    LOG_INFO << "Main begin...";
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
    if(argc == 2)
    {
        QString fileName = argv[1];
    }
    MainWindow w;
    w.show();
    return a.exec();
}
