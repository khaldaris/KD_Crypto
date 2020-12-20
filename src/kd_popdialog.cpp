#include "kd_popdialog.h"
#include <QLabel>
#include <QHBoxLayout>
#include <QCursor>

KD_PopDialog::KD_PopDialog(QWidget *parent) : QDialog(parent)
{
    this->setWindowFlags(windowFlags()&~Qt::WindowContextHelpButtonHint);
    connect(this,&KD_PopDialog::closeAnimation,this,&QWidget::close);
    connect(this,&KD_PopDialog::startAnimation,this,&QWidget::show);
    QLabel* label = new QLabel("操作指南\n"
                               "1、将文件拖至窗口内\n"
                               "2、敲击回车键\n",this);
    this->setStyleSheet("QLabel{font: 24px;}");
    label->setAlignment(Qt::AlignCenter);
    QHBoxLayout* layout = new QHBoxLayout(this);
    this->setMinimumSize(QSize(334,334));
    this->setMaximumSize(QSize(334,334));
    layout->addWidget(label);
    this->setLayout(layout);
}
