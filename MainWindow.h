#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QTimer>
#include "Receiver.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionSettings_triggered();
    void on_actionExit_triggered();
    void slotSetSettings( const Receiver &receiver );
    void slotReceivedData( const QByteArray &data );
    void realtimeDataSlot();

private:
    void runReceiver();

private:
    Ui::MainWindow *ui;
    Receiver *m_receiver;
    QLabel *m_statusLabel;
    QTimer dataTimer;
    double m_value;
};

#endif // MAINWINDOW_H
