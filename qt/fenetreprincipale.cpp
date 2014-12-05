#include "fenetreprincipale.h"
#include "ui_fenetreprincipale.h"
#include <QSerialPort>

QSerialPort *serial;
FenetrePrincipale::FenetrePrincipale(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FenetrePrincipale)
{
    serial = new QSerialPort(this);
    serial->setPortName("COM11"); //COM-port your Arduino is connected to
    serial->open(QIODevice::ReadWrite);
    serial->setBaudRate(QSerialPort::Baud9600); //must be the same as your arduino-baudrate
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);
    ui->setupUi(this);
    connect(ui->luminosite_ledRouge, SIGNAL(valueChanged(int)), this, SLOT(changeLedRougeLuminosite(int)));
}

FenetrePrincipale::~FenetrePrincipale()
{
    delete ui;
}

void FenetrePrincipale::init_port()
{
    port = new QextSerialPort("COM11"); //we create the port

    port->open(QIODevice::ReadWrite | QIODevice::Unbuffered); //we open the port
    if(!port->isOpen())
    {
        QMessageBox::warning(this, "port error", "Impossible to open the port!");
    }

    //we set the port properties
    port->setBaudRate(BAUD9600);//modify the port settings on your own
    port->setFlowControl(FLOW_OFF);
    port->setParity(PAR_NONE);
    port->setDataBits(DATA_8);
    port->setStopBits(STOP_1);

}

void FenetrePrincipale::changeLedRougeLuminosite(const int luminosite)
{
    this->transmitCmd('a', luminosite);
}

void FenetrePrincipale::transmitCmd(const char command, const int value)
{
    QByteArray com = &command;
    port->write(com);

    QByteArray target;
    QDataStream s(&target, QIODevice::ReadWrite);
    qint32 valeur = value;
    s << valeur;
    port->write(target);
}
