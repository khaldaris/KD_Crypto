#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QKeyEvent>
#include <QTimer>

#include <encryptocontrol.h>
#include <decryptocontrol.h>

#include <kd_popdialog.h>

enum FileStatus{
    openFile,
    file,
    encryptoFile
};
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    MainWindow(QWidget *parent = nullptr);
    void dropEvent(QDropEvent* event);
    void keyReleaseEvent(QKeyEvent* event);
    void dragEnterEvent(QDragEnterEvent* event);
    bool eventFilter(QObject *obj, QEvent *event);
    ~MainWindow();

signals:

private slots:

    void setFileStatus(FileStatus);

private:

    int fileType;
    QString fileName;
    CryptoControl* cc;
    Ui::MainWindow *ui;
    KD_PopDialog* popDialog;
    QTimer* initAnimationTimer;

};
#endif // MAINWINDOW_H
