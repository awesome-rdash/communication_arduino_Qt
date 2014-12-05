#ifndef FENETREPRINCIPALE_H
#define FENETREPRINCIPALE_H

#include <QMainWindow>
#include "E:/Qt/qextserialport-1.2rc/src/qextserialport.h"
#include <QMessageBox>
#include <QChar>
#include <QSlider>

namespace Ui {
class FenetrePrincipale;
}

class FenetrePrincipale : public QMainWindow
{
    Q_OBJECT

public:
    explicit FenetrePrincipale(QWidget *parent = 0);
    ~FenetrePrincipale();

    void init_port(); // port initialisation function

public slots:
    void transmitCmd(char command = 'a', int value = 0);
    void changeLedRougeLuminosite(int luminosite = 127);

private:
    Ui::FenetrePrincipale *ui;
        QextSerialPort *port;
    QSlider *luminosite_ledRouge;

        int *luminosite;
        int *nMyInt = 0;
};

#endif // FENETREPRINCIPALE_H
