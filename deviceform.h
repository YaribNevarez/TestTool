#ifndef DEVICEFORM_H
#define DEVICEFORM_H

#include <QDialog>
#include <QTimer>

namespace Ui {
class DeviceForm;
}

class DeviceForm : public QDialog
{
    Q_OBJECT

public:
    explicit DeviceForm(QWidget *parent = 0);
    ~DeviceForm();

    void set_name(QString);

private slots:

    void on_wrie_button_clicked();

    void on_read_button_clicked();

    void on_timed_read(void);

    void on_checkBox_stateChanged(int arg1);

    void on_horizontalSlider_sliderMoved(int position);

    void on_input_editingFinished();

    void on_finished(int result);

private:
    Ui::DeviceForm * ui;
    QString          device_name;
    QTimer         * timer;
};

#endif // DEVICEFORM_H
