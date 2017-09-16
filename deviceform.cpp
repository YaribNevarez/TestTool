#include "deviceform.h"
#include "ui_deviceform.h"
#include "mainwindow.h"

DeviceForm::DeviceForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DeviceForm)
{
    ui->setupUi(this);
    timer = new QTimer(this);
    timer->setInterval(25);

    connect(timer, SIGNAL(timeout()), this, SLOT(timed_read()));
}

void DeviceForm::timed_read(void)
{
    on_read_button_clicked();
}

void DeviceForm::set_name(QString name)
{
    device_name = name;
    ui->name->setText(name);
    setWindowTitle(name);
}

DeviceForm::~DeviceForm()
{
    delete ui;
}

void DeviceForm::on_wrie_button_clicked()
{
    if (dynamic_cast<MainWindow *>(parent()))
    {
        int value = ui->input->value();
        ((MainWindow *)parent())->write_to_device(device_name, value);
    }
}

void DeviceForm::on_read_button_clicked()
{
    if (dynamic_cast<MainWindow *>(parent()))
    {
        int value = ((MainWindow *)parent())->read_from_device(device_name);
        ui->output->display(value);
        ui->progressBar->setValue(value);
    }
}

void DeviceForm::on_checkBox_stateChanged(int arg1)
{
    if (arg1)
        timer->start();
    else
        timer->stop();
}
