#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDesktopServices>
#include <QUrl>
#include <QFileDialog>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QTime>
#include <QFuture>
#include <QCoreApplication>
#include <QtConcurrent/QtConcurrent>

#include "NanoLog.h"
void formatQStringPath(QString fullPath)
{
    fullPath.replace("\\","/");
}
QString getFileName(QString fullPathAndName)
{
    return fullPathAndName.mid(fullPathAndName.lastIndexOf('/'));
}
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_QuitOnClose);
    this->setAcceptDrops(true);
    ui->statusBarLabel->setText("初始化已经完成");
    ui->label->raise();
    setWindowIcon(QIcon(":/Resources/ico/main.ico"));
    fileName = "";
    fileType = -1;
    cc = nullptr;
    popDialog = new KD_PopDialog(this);
    initAnimationTimer = new QTimer(this);
    initAnimationTimer->setInterval(4000);
    initAnimationTimer->setSingleShot(true);

    connect(initAnimationTimer,SIGNAL(timeout()),ui->label,SLOT(lower()));
    emit ui->label->startAnimation();
    initAnimationTimer->start();
    ui->popWidgetEnter->setAttribute(Qt::WA_Hover,true);//开启悬停事件
    ui->popWidgetEnter->installEventFilter(this);       //安装事件过滤器
    QStringList arguments = QCoreApplication::arguments();

    if(arguments.length()==2)
    {
        QString fullFileName = arguments.at(1);
        formatQStringPath(fullFileName);
        QFileInfo fi(fullFileName);
        if(fi.isFile())
        {
            try {
                fileType = CryptoControl::is_decrypto_file(fullFileName.toStdString());
            } catch (std::exception& e) {
                fileType = -1;
                LOG_INFO << e.what();
                return;
            }
            if(1 == fileType)
            {
                setFileStatus(FileStatus::encryptoFile);
                ui->statusBarLabel->setText("待解密的文件已经获取");
                fileName = fullFileName;
                LOG_INFO << "get crypto file";
            }
            if(0 == fileType)
            {
                ui->statusBarLabel->setText("待加密的文件已经获取");
                fileName = fullFileName;
                setFileStatus(FileStatus::file);
                LOG_INFO << "get source file";
            }
        }
    }
}
bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if(obj == ui->popWidgetEnter) {
        if(event->type() == QEvent::HoverEnter) {
            popDialog->move(this->geometry().x(),this->geometry().y()+50);
            emit popDialog->startAnimation();
            return true;
        }
        if(event->type() == QEvent::Leave)
        {
            emit popDialog->closeAnimation();
            return true;
        }
    }
    return QWidget::eventFilter(obj,event);
}
void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls())
        event->acceptProposedAction();
}
void MainWindow::dropEvent(QDropEvent *event)
{
    QList<QUrl> urls = event->mimeData()->urls();
    if(urls.isEmpty())
    {
        ui->statusBarLabel->setText("似乎是空的，请重试");
        LOG_INFO << "enpty try again";
        return;
    }
    if(urls.size()>1)
    {
        ui->statusBarLabel->setText("不支持多个文件");
        LOG_INFO << "no support files";
        return;
    }
    for(auto url=urls.begin();url!=urls.end();url++) {
        QFileInfo fileInfo((*url).toLocalFile());
        if(fileInfo.isDir())
        {
            ui->statusBarLabel->setText("不支持加密文件夹");
            LOG_INFO << "no support folder";
            return;
        }
        else if(fileInfo.isFile())
        {
            try {
                fileType = CryptoControl::is_decrypto_file((*url).toLocalFile().toStdString());
            } catch (std::exception& e) {
                fileType = -1;
                LOG_INFO << e.what();
                return;
            }
            if(1 == fileType)
            {
                setFileStatus(FileStatus::encryptoFile);
                ui->statusBarLabel->setText("待解密的文件已经获取");
                LOG_INFO << "get decrypto file";
                fileName = (*url).toLocalFile();
                return;
            }
            if(0 == fileType)
            {
                ui->statusBarLabel->setText("待加密的文件已经获取");
                LOG_INFO << "get source file";
                fileName = (*url).toLocalFile();
                setFileStatus(FileStatus::file);
                return;
            }
        }
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key()==16777220)//ENTER
    {
        ui->statusBarLabel->setText("选择保存的文件夹");
        QString outPutDirPath = QFileDialog::getExistingDirectory(this, "选择文件夹","/");
        if (outPutDirPath.isEmpty())
            return;
        else
        {
            formatQStringPath(outPutDirPath);
            outPutDirPath += "/";
        }

        if(fileName.isEmpty())
        {
            ui->statusBarLabel->setText("请选择一个文件");
            return;
        }

        if(1 == fileType)
        {
            ui->statusBarLabel->setText("解密中...");
            QFuture<bool> future = QtConcurrent::run([&](){
                delete this->cc;
                this->cc = new DeCryptoControl();
                this->cc->process_data(std::string((const char*)fileName.toLocal8Bit()),std::string((const char*)outPutDirPath.toLocal8Bit()),std::string("password"));
                return true;
            });
            while(!future.isFinished())
            {
                QApplication::processEvents(QEventLoop::AllEvents, 100);
            }
        }
        if(0 == fileType)
        {
            ui->statusBarLabel->setText("加密中...");
            QFuture<bool> future = QtConcurrent::run([&](){
                delete this->cc;
                this->cc = new EnCryptoControl();
                outPutDirPath += QDateTime::fromTime_t(QDateTime::currentDateTime().toTime_t()).toString("yyyy-MM-dd hh-mm-ss-zzz");
                outPutDirPath += ".KD_Crypto";
                this->cc->process_data(std::string((const char*)fileName.toLocal8Bit()),std::string((const char*)outPutDirPath.toLocal8Bit()),std::string("JUNHENGZHIDAOPINGHENGZHIJIAN"));
                return true;
            });
            while(!future.isFinished())
            {
                QApplication::processEvents(QEventLoop::AllEvents, 100);
            }
        }
        ui->statusBarLabel->setText("操作完成");
        LOG_INFO << "finished";
    }
    QWidget::keyReleaseEvent(event);
}

void MainWindow::setFileStatus(FileStatus fileStatus)
{
    ui->fileStatus->setToolTip(this->fileName);
    if(fileStatus == FileStatus::openFile)
    {
        ui->fileStatus->setStyleSheet("QWidget#fileStatus{border-image: url(:/Resources/ico/openFile.png)}");
    }
    else if(fileStatus == FileStatus::file)
    {
        ui->fileStatus->setStyleSheet("QWidget#fileStatus{border-image: url(:/Resources/ico/file.png)}");
    }
    else
    {
        ui->fileStatus->setStyleSheet("QWidget#fileStatus{border-image: url(:/Resources/ico/encryptoFile.png)}");
    }
}
MainWindow::~MainWindow()
{
    delete cc;
    delete ui;
}
