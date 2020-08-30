#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QDesktopServices>
#include <QUrl>
#include <QFileDialog>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QTime>
#include <QFuture>
#include <QCoreApplication>
#include <QtConcurrent/QtConcurrent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_QuitOnClose);
    this->setWindowTitle("KD_Parent");
    this->setAcceptDrops(true);
    ui->statusBarLabel->setText("初始化已经完成");
    fileName = "";
    dropStatus = true;
    cc = nullptr;
    QObject::connect(this,SIGNAL(cryptoFinished()),this,SLOT(onSaveBegin()));
}
QString getFileName(QString fullPathAndName)
{
    return fullPathAndName.mid(fullPathAndName.lastIndexOf('/')+1);
}
bool isKD_CryptoFile(QString fileName)
{
    return (fileName.mid(fileName.length()-QString(".KD_Crypto").length())==QString(".KD_Crypto"));
}
void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls())
        event->acceptProposedAction();
}
void MainWindow::dropEvent(QDropEvent *event)
{
    if(!dropStatus)
        return;
    QList<QUrl> urls = event->mimeData()->urls();
    if(urls.isEmpty())
    {
        ui->statusBarLabel->setText("似乎是空的，请重试");
        return;
    }
    if(urls.size()>1)
    {
        ui->statusBarLabel->setText("不支持多个文件");
        return;
    }
    for(auto url=urls.begin();url!=urls.end();url++) {
        QFileInfo fileInfo((*url).toLocalFile());
        if(fileInfo.isDir())
        {
            ui->statusBarLabel->setText("不支持加密文件夹");
            qDebug()<<(*url).toLocalFile()<<endl;
            return;
        }
        else if(fileInfo.isFile())
        {
            fileName = (*url).toLocalFile();
            qDebug()<<fileName<<endl;
            if(fileName.length()>QString(".KD_Crypto").length()){
                if(isKD_CryptoFile(fileName))
                {
                    ui->statusBarLabel->setText("待解密的文件已经获取\n"+getFileName(fileName));
                    return;
                }
            }
            ui->statusBarLabel->setText("待加密的文件已经获取\n"+getFileName(fileName));
        }
    }
    qDebug()<<fileName<<endl;
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    qDebug()<<"PressKey:"<<event->key()<<endl;
    if(event->key()==16777216)//ESC
    {
        QWidget* widget = new QWidget(this,Qt::Window);
        QLabel* label = new QLabel("Author KhalDaris\n"
                                   "Copyright 2020\n",this);
        widget->setStyleSheet("QLabel{font: 24px;}");
        label->setAlignment(Qt::AlignCenter);
        QHBoxLayout* layout = new QHBoxLayout(widget);
        widget->setMinimumSize(QSize(300,200));
        widget->setMaximumSize(QSize(300,200));
        widget->setWindowTitle("About");
        layout->addWidget(label);
        widget->setLayout(layout);
        widget->show();
    }
    if(event->key()==16777220)//ENTER
    {
        if(!dropStatus)
            return;
        dropStatus = false;
        QString inputKey = ui->lineEdit_key->text();
        qDebug()<<inputKey<<endl;
        if(inputKey.isEmpty()||fileName.isEmpty())
        {
            if(inputKey.isEmpty())
            {
                ui->statusBarLabel->setText("密码为空，请输入些内容");
                return;
            }
            if(fileName.isEmpty())
            {
                ui->statusBarLabel->setText("请选择一个文件");
                return;
            }
        }
        if(isKD_CryptoFile(fileName))//解密操作
        {
            ui->statusBarLabel->setText("解密中...");
            QFuture<bool> future = QtConcurrent::run([&](){
                delete this->cc;
                this->cc = new DeCryptoControl(fileName.toStdString(),ui->lineEdit_key->text().toStdString());
                this->cc->decrypt();
                return true;
            });
            while(!future.isFinished())
            {
                QApplication::processEvents(QEventLoop::AllEvents, 100);
            }
        }
        else//加密操作
        {
            ui->statusBarLabel->setText("加密中...");
            QFuture<bool> future = QtConcurrent::run([&](){
                delete this->cc;
                this->cc = new EnCryptoControl(fileName.toStdString(),ui->lineEdit_key->text().toStdString(),1);
                this->cc->encrypt();
                return true;
            });
            while(!future.isFinished())
            {
                QApplication::processEvents(QEventLoop::AllEvents, 100);
            }
        }
        ui->statusBarLabel->setText("操作完成");
        if(cc->isEffective())
        {
            emit cryptoFinished();
        }
        else
        {
            ui->statusBarLabel->setText("发生了错误，错误代码："+QString::number(static_cast<int>(cc->getEffective()),10));
        }
        dropStatus = true;
    }
}

void MainWindow::onSaveBegin()
{
    ui->statusBarLabel->setText("选择保存的文件夹");
    QString outPutDirPath = QFileDialog::getExistingDirectory(this, "choose Directory","/");
    if (outPutDirPath.isEmpty())
        return;
    else
    {
        qDebug() << "outPutDirPath=" << outPutDirPath;
        outPutDirPath += "/";
    }
    {
        ui->statusBarLabel->setText("正在输出...");
        QFuture<bool> future = QtConcurrent::run([&](){
            cc->outFile(outPutDirPath.toStdString());
            return true;
        });
        while(!future.isFinished())
        {
            QApplication::processEvents(QEventLoop::AllEvents, 100);
        }
    }
    if(cc->isEffective())
    {
        ui->statusBarLabel->setText("已完成");
    }
    else
    {
        ui->statusBarLabel->setText("发生了错误，错误代码："+QString::number(static_cast<int>(cc->getEffective()),10));
    }
}
MainWindow::~MainWindow()
{
    delete cc;
    delete ui;
}
