#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QListWidget>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    void   write_to_device(QString, qint32);
    qint32 read_from_device(QString);

    ~MainWindow();

signals:

public slots:
    void connected();
    void disconnected();
    void bytesWritten (qint64 bytes);
    void readyRead();

private slots:
    void on_connect_button_clicked();

    void on_disconnect_button_clicked();

    void on_list_itemDoubleClicked(QListWidgetItem *item);

    void on_clear_log_clicked();

private:
    Ui::MainWindow *ui;
    QTcpSocket     *socket;
    qint8           devices;
};

#endif // MAINWINDOW_H
