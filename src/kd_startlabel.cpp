#include "kd_startlabel.h"

#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>

void initPixmapList(QList<QPixmap> &pixmapList)
{
    for(int i = 0 ; i<100;i++){
        QImage image;
        image.load(":/Resources/LogoAnimation/_Main_Comp/_Main_Comp_000"+QString::number(i)+".png");
        image.scaledToWidth(image.width()/2);
        image.scaledToHeight(image.height()/2);
        pixmapList.push_back(QPixmap::fromImage(image));
    }
}
KD_StartLabel::KD_StartLabel(QWidget  *parent) : QLabel (parent)
{
    initPixmapList(pixmapList);
    switchPNGTimer = new QTimer(this);
    switchPNGTimer->setInterval(40);
    opacTimer = new QTimer(this);
    opacTimer->setInterval(3800);
    opacTimer->setSingleShot(true);
    connect(switchPNGTimer,SIGNAL(timeout()),this,SLOT(switchPNG()));
    connect(opacTimer,SIGNAL(timeout()),this,SLOT(opacAnimation()));
    connect(this,SIGNAL(startAnimation()),this,SLOT(globalAnimation()));
}
void KD_StartLabel::globalAnimation()
{
    switchPNGTimer->start();
    opacTimer->start();
}

void KD_StartLabel::opacAnimation()
{
    QGraphicsOpacityEffect *pButtonOpacity = new QGraphicsOpacityEffect(this);
    pButtonOpacity->setOpacity(1);
    this->setGraphicsEffect(pButtonOpacity);
    QPropertyAnimation *pOpacityAnimation1 = new QPropertyAnimation(pButtonOpacity, "opacity");
    pOpacityAnimation1->setDuration(200);
    pOpacityAnimation1->setStartValue(1);
    pOpacityAnimation1->setEndValue(0);
    pOpacityAnimation1->start();
}

void KD_StartLabel::switchPNG()
{
    if(pixmapList.length()<1)
    {
        return;
    }
    this->setPixmap(pixmapList.front());
    pixmapList.pop_front();
    this->show();
}
