#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "deviceform.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    socket = new QTcpSocket(this);
    connect(socket, SIGNAL(connected()),          this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()),       this, SLOT(disconnected()));
    connect(socket, SIGNAL(readyRead()),          this, SLOT(readyRead()));
    connect(socket, SIGNAL(bytesWritten(qint64)), this, SLOT(bytesWritten(qint64)));
}

void   MainWindow::write_to_device(QString device, qint32 value)
{
    QString command = device + " = " + QString::number(value);

    socket->write(command.toLocal8Bit());

    ui->log_text->setTextColor(QColor::fromRgb(0, 0, 0));
    ui->log_text->append(command);

    if(!socket->waitForBytesWritten(3500))
    {
       ui->log_text->setTextColor(QColor::fromRgb(255, 0, 0));
       ui->log_text->append("Error: " + socket->errorString());
    }

    if(!socket->waitForReadyRead(3500))
    {
        ui->log_text->setTextColor(QColor::fromRgb(255, 0, 0));
        ui->log_text->append("Error: " + socket->errorString());
    }

    ui->log_text->setTextColor(QColor::fromRgb(0, 128, 128));
    ui->log_text->append(socket->readAll());
}

qint32 MainWindow::read_from_device(QString device)
{
    std::string answer = "0";

    socket->write(device.toLocal8Bit());

    ui->log_text->setTextColor(QColor::fromRgb(0, 0, 0));
    ui->log_text->append(device);

    if(!socket->waitForBytesWritten(3500))
    {
        ui->log_text->setTextColor(QColor::fromRgb(255, 0, 0));
        ui->log_text->append("Error: " + socket->errorString());
    }

    if(!socket->waitForReadyRead(3500))
    {
        ui->log_text->setTextColor(QColor::fromRgb(255, 0, 0));
        ui->log_text->append("Error: " + socket->errorString());
    }
    else
    {
        answer = socket->readAll().toStdString();
        ui->log_text->setTextColor(QColor::fromRgb(0, 128, 128));
        ui->log_text->append(QString(answer.c_str()));

        answer.resize(answer.find('\n'));

        answer = answer.substr(answer.find('=') + 1);

        qDebug() << answer.c_str();
    }
    return strtoul(answer.c_str(), NULL, 0);
}

MainWindow::~MainWindow()
{
    if (socket->isOpen())
    {
        qDebug() << "Trying to disconnect...";
        socket->write("EXIT");
        ui->log_text->setTextColor(QColor::fromRgb(0, 0, 0));
        ui->log_text->append("EXIT");

        if(!socket->waitForBytesWritten(3500))
        {
            ui->log_text->setTextColor(QColor::fromRgb(255, 0, 0));
            ui->log_text->append("Error: " + socket->errorString());
        }

        if(!socket->waitForReadyRead(3500))
        {
            ui->log_text->setTextColor(QColor::fromRgb(255, 0, 0));
            ui->log_text->append("Error: " + socket->errorString());
        }

        ui->log_text->setTextColor(QColor::fromRgb(0, 128, 128));
        ui->log_text->append(socket->readAll());

        socket->disconnectFromHost();
        socket->close();
    }
    delete ui;
}

void MainWindow::on_connect_button_clicked()
{
    qDebug() << "Trying to connect to " + ui->host->text() + " : " + ui->port->text();

    socket->connectToHost(ui->host->text(), ui->port->text().toInt());

    if(!socket->waitForConnected(3500))
    {
        ui->log_text->setTextColor(QColor::fromRgb(255, 0, 0));
        ui->log_text->append("Error: " + socket->errorString());
    }
}

void MainWindow::connected()
{
    qDebug() << "Connected!";
    ui->host->setEnabled(false);
    ui->port->setEnabled(false);
    ui->connect_button->setEnabled(false);
    ui->disconnect_button->setEnabled(true);

    socket->write("DEVICES");
    ui->log_text->setTextColor(QColor::fromRgb(0, 0, 0));
    ui->log_text->append("DEVICES");

    if(!socket->waitForBytesWritten(3500))
    {
        ui->log_text->setTextColor(QColor::fromRgb(255, 0, 0));
        ui->log_text->append("Error: " + socket->errorString());
    }

    if(!socket->waitForReadyRead(3500))
    {
        ui->log_text->setTextColor(QColor::fromRgb(255, 0, 0));
        ui->log_text->append("Error: " + socket->errorString());
    }

    std::string answer = socket->readAll().toStdString();

    ui->log_text->setTextColor(QColor::fromRgb(0, 128, 128));
    ui->log_text->append(QString(answer.c_str()));

    if (!answer.empty())
    {
        QString command;
        answer.resize(answer.find('\n'));
        devices = QString(answer.c_str()).toUInt();
        ui->devices->display((int)devices);

        for (qint8 i = 0; i < devices; i ++)
        {
            command = QString().sprintf("DEVICE %d", i);
            socket->write(command.toLocal8Bit());
            ui->log_text->setTextColor(QColor::fromRgb(0, 0, 0));
            ui->log_text->append(command);

            if(!socket->waitForBytesWritten(3500))
            {
                ui->log_text->setTextColor(QColor::fromRgb(255, 0, 0));
                ui->log_text->append("Error: " + socket->errorString());
            }

            if(!socket->waitForReadyRead(3500))
            {
                ui->log_text->setTextColor(QColor::fromRgb(255, 0, 0));
                ui->log_text->append("Error: " + socket->errorString());
            }

            answer = socket->readAll().toStdString();
            ui->log_text->setTextColor(QColor::fromRgb(0, 128, 128));
            ui->log_text->append(QString(answer.c_str()));

            answer.resize(answer.find('\n', 0));

            ui->list->addItem(QString(answer.c_str()));
        }
    }

    ui->status_label->setText("Connected");
}


void MainWindow::disconnected()
{
    qDebug() << "Disconnected!";
    ui->host->setEnabled(true);
    ui->port->setEnabled(true);
    ui->connect_button->setEnabled(true);
    ui->disconnect_button->setEnabled(false);
    ui->status_label->setText("Disconnected");
    ui->list->clear();
    ui->devices->display(0);
}


void MainWindow::bytesWritten (qint64 bytes)
{
    qDebug() << "Bytes written: " << bytes;
}


void MainWindow::readyRead()
{

}

void MainWindow::on_disconnect_button_clicked()
{    
    qDebug() << "Trying to disconnect...";
    socket->write("EXIT");
    ui->log_text->setTextColor(QColor::fromRgb(0, 0, 0));
    ui->log_text->append("EXIT");

    if(!socket->waitForBytesWritten(3500))
    {
        ui->log_text->setTextColor(QColor::fromRgb(255, 0, 0));
        ui->log_text->append("Error: " + socket->errorString());
    }

    if(!socket->waitForReadyRead(3500))
    {
        ui->log_text->setTextColor(QColor::fromRgb(255, 0, 0));
        ui->log_text->append("Error: " + socket->errorString());
    }

    ui->log_text->setTextColor(QColor::fromRgb(0, 128, 128));
    ui->log_text->append(socket->readAll());

    socket->disconnectFromHost();
    socket->close();
}

void MainWindow::on_list_itemDoubleClicked(QListWidgetItem *item)
{
    DeviceForm * device_form = new DeviceForm(this);

    device_form->set_name(item->text());
    device_form->show();
}

void MainWindow::on_clear_log_clicked()
{
    ui->log_text->clear();
}
