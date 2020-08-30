#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QKeyEvent>

#include <encryptocontrol.h>
#include <decryptocontrol.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void dragEnterEvent(QDragEnterEvent* event);
    void dropEvent(QDropEvent* event);
    void keyReleaseEvent(QKeyEvent* event);
signals:
    void cryptoFinished();
private slots:
    void onSaveBegin();
private:
    bool dropStatus;
    Ui::MainWindow *ui;
    QString fileName;
    CryptoControl* cc;
};
#endif // MAINWINDOW_H
