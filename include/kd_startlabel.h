#ifndef KD_STARTLABEL_H
#define KD_STARTLABEL_H

#include <QObject>
#include <QLabel>
#include <QPixmap>
#include <QQueue>
#include <QTimer>

class KD_StartLabel : public QLabel
{
    Q_OBJECT
public:
    explicit KD_StartLabel(QWidget  *parent = nullptr);

signals:
    void startAnimation();

public slots:
    void globalAnimation();
    void opacAnimation();
    void switchPNG();

private:
    QQueue<QPixmap> pixmapList;
    QTimer* switchPNGTimer;
    QTimer* opacTimer;
};

#endif // KD_STARTLABEL_H
