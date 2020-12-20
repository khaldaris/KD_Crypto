#ifndef KD_POPDIALOG_H
#define KD_POPDIALOG_H

#include <QDialog>

class KD_PopDialog : public QDialog
{
    Q_OBJECT

signals:
    void startAnimation();
    void closeAnimation();

public:
    explicit KD_PopDialog(QWidget *parent = nullptr);

signals:

};

#endif // KD_POPDIALOG_H
